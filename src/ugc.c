#include "ugc.h"
#include <stdlib.h>
#include <stdio.h>

void* malloc(size_t sz) {
    printf("Hello, World!\n");
    return 0;
}

__attribute__((constructor)) void ugc_init(int argc, char** argv, char** envp) {
    printf("Initializing The Universal Garbage Collector\n");
}

void hello(void) {
    printf("Hi :)\n");
}