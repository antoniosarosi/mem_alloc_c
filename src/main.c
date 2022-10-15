#include <stdio.h>
#include "alloc.h"
#include <stdlib.h>

// 16 Bytes total size
struct Point {
    double x;
    double y;
};

void fail(char *message) {
    fprintf(stderr, message);
    exit(1);
}

int main() {
    printf("Allocating memory block for struct Point { double x, y; }\n");
    struct Point *block1 = alloc(sizeof(struct Point));

    if (block1 == NULL) {
        fail("Failed allocating memory, got NULL pointer\n");
    }

    printf("Address of allocated memory: %p\n", block1);
    printf("Requested size: %lu bytes\n", sizeof(struct Point));

    printf("Writing content to allocated memory (x = 1.1, y = 2.2)\n");
    block1->x = 1.1;
    block1->y = 2.2;

    printf("Reading written values: Point { x = %f, y = %f }\n", block1->x, block1->y);

    printf("\nAllocating memory block for 3 extra struct Point\n");
    struct Point *block2 = alloc(sizeof(struct Point) * 3);

    if (block2 == NULL) {
        fail("Failed allocating memory, got NULL pointer\n");
    }

    printf("Addresses:\n- Point 1: %p\n- Point 2: %p\n- Point 3: %p\n", block2, &block2[1], &block2[2]);
    printf("Requested size: %lu bytes\n", sizeof(struct Point) * 3);

    printf("\nDeallocating memory for first allocated block\n");
    dealloc(block1);
    printf("Allocating again, should return the previously deallocated address\n");
    block1 = alloc(sizeof(struct Point));
    printf("Returned address: %p\n", block1);

    printf("\nDeallocating second block\n");
    dealloc(block2);
    printf("Allocating another block, should have the same address as second block\n");
    struct Point *block3 = alloc(sizeof(struct Point));
    printf("Returned address: %p\n", block3);
}
