#include <stddef.h>

#define ALIGNMENT _Alignof(max_align_t)

int SIZE_OF_SIZE = sizeof(size_t);

struct freelist {
    uint64_t blocksz;
    struct freelist* next;
};