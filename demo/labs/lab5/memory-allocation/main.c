#include <stdio.h>
#include <stdlib.h> // malloc, calloc, realloc, free

// Build: gcc -Wall main.c -o allocation_demo
// Run:   ./allocation_demo
// There is no standard C function named alloc; allocation means reserving memory.

int main(void) {
    // 1. malloc: reserve enough bytes for 3 integers.
    // sizeof(int) gives the number of bytes needed for one integer.
    // malloc does NOT initialize the memory. Assign values before reading it.
    int *numbers = malloc(3 * sizeof(int));
    if (!numbers) {
        fprintf(stderr, "Error: malloc failed.\n");
        return 1;
    }

    // Use the allocated memory like an array.
    for (int i = 0; i < 3; i++) {
        numbers[i] = (i + 1) * 10;
    }

    printf("malloc:");
    for (int i = 0; i < 3; i++) {
        printf(" %d", numbers[i]);
    }
    printf("\n"); // 10 20 30

    // 2. realloc: resize the allocation from 3 integers to 5 integers.
    // It preserves the existing values, but may move the memory elsewhere.
    // Use a temporary pointer: on failure, the original allocation is still valid.
    int *larger = realloc(numbers, 5 * sizeof(int));
    if (!larger) {
        fprintf(stderr, "Error: realloc failed.\n");
        free(numbers);
        return 1;
    }
    numbers = larger;

    // The added memory is NOT initialized. Set the new values before reading.
    numbers[3] = 40;
    numbers[4] = 50;

    printf("realloc:");
    for (int i = 0; i < 5; i++) {
        printf(" %d", numbers[i]);
    }
    printf("\n"); // 10 20 30 40 50

    // free releases the allocation. Do not access numbers after this call.
    free(numbers);

    // 3. calloc: reserve memory for 3 integers and initialize all bytes to zero.
    // Unlike malloc, it takes two arguments: element count and element size.
    // For this integer array, each element starts as 0.
    int *zeros = calloc(3, sizeof(int));
    if (!zeros) {
        fprintf(stderr, "Error: calloc failed.\n");
        return 1;
    }

    printf("calloc:");
    for (int i = 0; i < 3; i++) {
        printf(" %d", zeros[i]);
    }
    printf("\n"); // 0 0 0

    // Allocated memory stays allocated until we free it; leaving a block of
    // code does not automatically free it. Release each allocation once.
    free(zeros);
    return 0;
}
