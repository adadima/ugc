//
// Created by Alexandra Dima on 2020-12-28.
//

#include <stdio.h>
#include <stdlib.h>
#include <ugc.h>

int main(int argc, char** argv) {
    hello();
    printf("Result of malloc: %p\n", malloc(17));
    printf("ALL TESTS PASSED\n");
    return 0;
}