#include "ugc.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "defs.h"
#include "growheap.h"

// global lock on the free memory, each thread calling
// `malloc` needs to acquire this
static int PGSIZE;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static struct freelist* freemem = 0;
void* heap_start;


static size_t roundup(size_t bytes, size_t quot) {
    return (bytes + quot - 1) / quot * quot;
}

void freelist_print(void) {
    struct freelist* curr_block = freemem;
    while (curr_block) {
        printf("BLOCK: START %p SIZE %llu NEXT %p\n", curr_block, curr_block->blocksz, curr_block->next);
        curr_block = curr_block->next;
    }
}

void* malloc(size_t sz) {
    struct freelist* curr_block = freemem;
    struct freelist* prev_block = 0;

    // addresses returned from malloc need to be aligned
    // to the max alignment any C built-in type can have
    size_t actualsz = roundup(sz, ALIGNMENT) + ALIGNMENT;
    char* addr = 0;

    // critical section below
    pthread_mutex_lock(&mutex);
    // 1. go through the list of free blocks and see if any has at least sz bytes
    while (curr_block) {
        if (curr_block->blocksz >= actualsz) {
            break;
        }
        prev_block = curr_block;
        curr_block = curr_block->next;
    }

    // 2. If yes, go to 3a. Else, go to 3b.
    if (curr_block) {
        // 3a. Break the block up into 2 ( one with size sz, the other one the remaining ).
        //     Release lock and return address
        if (curr_block->blocksz - actualsz < sizeof(struct freelist)) {
            *((size_t*) ((char*)curr_block + ALIGNMENT - SIZE_OF_SIZE)) = (size_t) (curr_block->blocksz - ALIGNMENT);
            addr = ((char*)curr_block + ALIGNMENT);

            // if previous exists, make its next point to the next block after curr_block
            if (prev_block) {
                prev_block->next = curr_block->next;
            }
            // if curr_block is the first block, make freemem point to the next
            else {
                freemem = curr_block->next;
            }
        } else if (actualsz < curr_block->blocksz) {
            printf("Allocating from the same block\n");
            size_t leftoversz = curr_block->blocksz - actualsz;
            curr_block->blocksz = leftoversz;
            addr =  ((char*)curr_block + leftoversz + ALIGNMENT);
            *((size_t*) (addr - SIZE_OF_SIZE)) = (size_t) (actualsz - ALIGNMENT);

        } else {
            fprintf(stderr, "Should not reach this point: selected block needs to have >= actualsz bytes\n");
            abort();
        }
    } else {
        // 3b. Grow process heap. If mmap succeeds go to 4, else abort
        void* addr_ = prev_block ? prev_block + prev_block->blocksz: 0;
        addr = growheap(addr_, actualsz);
        if (!addr_) heap_start = addr;

        size_t totalsz = roundup(actualsz, (size_t) PGSIZE);
        // 4. Add a new block to freelist if sz is not multiple of PGSIZE.
        //     Release lock and return address
        size_t leftoversz = totalsz - actualsz;
        // if leftover memory, create a new block and add it to the freelist
        if (leftoversz >= sizeof(struct freelist)) {
            struct freelist* new_block = (struct freelist*) addr;
            new_block->blocksz = leftoversz;
            new_block->next = 0;
            size_t* ghost_loc = (size_t*)(addr + leftoversz + ALIGNMENT - SIZE_OF_SIZE);
            *(ghost_loc) = (size_t) actualsz - ALIGNMENT;
            addr += leftoversz + ALIGNMENT;
            if (prev_block) prev_block->next = new_block;
            else freemem = new_block;
        } else {
            fprintf(stderr, "Should not reach this point: blocks always keep their freelist struct\n");
            abort();
        }
    }
    pthread_mutex_unlock(&mutex);
    return (void*) addr;
}


void free(void* addr) {
    if (!addr) return;

    if (addr < heap_start) {
        perror("Can not free address because it hasn't been allocated using `malloc`\n");
        abort();
    }

    struct freelist* curr_block = freemem;
    struct freelist* prev_block = 0;

    size_t* ghost_location = (size_t*) (addr - SIZE_OF_SIZE);
    size_t  allocated = *(ghost_location);

    struct freelist* new_block = (struct freelist*) addr;
    new_block->blocksz = allocated;
    new_block->next = 0;


    while (curr_block) {
        if ((char*) curr_block >= (char*) (addr + allocated)) {
            break;
        }
        prev_block = curr_block;
        curr_block = curr_block->next;
    }

    if (curr_block) new_block->next = curr_block;

    if (prev_block) prev_block->next = new_block;

    if (!curr_block && !prev_block) freemem = new_block;
}

__attribute__((constructor)) void ugc_init(int argc, char** argv, char** envp) {
    printf("Initializing The Universal Garbage Collector\n");
    PGSIZE = getpagesize();
}