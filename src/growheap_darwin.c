#include "growheap.h"
#include <sys/mman.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>


void* growheap(void* addr, size_t bytes) {
    int prot = PROT_READ | PROT_WRITE;
    int flags = MAP_PRIVATE | MAP_ANONYMOUS;
//    printf("Bytes %zu\n", bytes);
//    printf("protection %d\n", prot);
//    printf("flags %d\n", flags);
//    printf("Addr %p\n", addr);
    if((addr = mmap(addr, bytes, prot, flags, -1, 0)) == MAP_FAILED) {
        perror("mmap failed");
        abort();
    }

    return addr;
}