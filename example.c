//
// Created by Alexandra Dima on 2020-12-28.
//

#include <stdio.h>
#include <stdlib.h>
#include <ugc.h>

int main(int argc, char** argv) {
    void* p = malloc(512);
    printf("Malloc 512: %p\n", p);
    freelist_print();
    printf("Ghost location %p\n", p - sizeof(size_t));
    printf("Value at ghost location %zu\n", *((size_t*)(p - sizeof(size_t))));

    p = malloc(32);
    printf("Malloc 32: %p\n", p);
    freelist_print();
    printf("Ghost location %p\n", p - sizeof(size_t));
    printf("Value at ghost location %zu\n", *((size_t*)(p - sizeof(size_t))));


    p = malloc(4000);
    printf("Malloc 4000: %p\n", p);
    freelist_print();
    printf("Ghost location %p\n", p - sizeof(size_t));
    printf("Value at ghost location %zu\n", *((size_t*)(p - sizeof(size_t))));


    p = malloc(4000);
    printf("Malloc 4000: %p\n", p);
    freelist_print();
    printf("Ghost location %p\n", p - sizeof(size_t));
    printf("Value at ghost location %zu\n", *((size_t*)(p - sizeof(size_t))));


    p = malloc(4096);
    printf("Malloc 4096: %p\n", p);
    freelist_print();
    printf("Ghost location %p\n", p - sizeof(size_t));
    printf("Value at ghost location %zu\n", *((size_t*)(p - sizeof(size_t))));


    p = malloc(50000);
    printf("Malloc 50000: %p\n", p);
    freelist_print();
    printf("Ghost location %p\n", p - sizeof(size_t));
    printf("Value at ghost location %zu\n", *((size_t*)(p - sizeof(size_t))));


    printf("ALL TESTS PASSED\n");
    return 0;
}