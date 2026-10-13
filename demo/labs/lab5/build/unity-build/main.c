#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Include implementations to compile the calculator as one translation unit.
 * Compile only main.c; do not also compile these included .c files. */
#include "add.c"
#include "subtract.c"
#include "multiply.c"
#include "divide.c"

int main(int argc, char *argv[]) {
    /* argv[0] is the program name; argv[1] is the operation. */
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <add|subtract|multiply|divide> <number> <number>\n", argv[0]);
        return 1;
    }

    // For this demo, assume the arguments are valid numbers.
    double a = atof(argv[2]);
    double b = atof(argv[3]);
    double result;

    if (strcmp(argv[1], "add") == 0) {
        result = add(a, b);
    } else if (strcmp(argv[1], "subtract") == 0) {
        result = subtract(a, b);
    } else if (strcmp(argv[1], "multiply") == 0) {
        result = multiply(a, b);
    } else if (strcmp(argv[1], "divide") == 0) {
        if (b == 0.0) {
            fprintf(stderr, "Error: cannot divide by zero.\n");
            return 1;
        }
        result = divide(a, b);
    } else {
        fprintf(stderr, "Error: supported operations are add, subtract, multiply, and divide.\n");
        return 1;
    }

    printf("%.10g\n", result);
    return 0;
}
