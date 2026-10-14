#include <stdio.h>

// A macro to calculate the square of a number, but without proper parentheses
// This macro can lead to unexpected results due to operator precedence issues.
// For example, int res = BAD_SQUARE(3 + 1); would evaluate as 3 + 1 * 3 + 1 = 7, not 16.
#define BAD_SQUARE(x) x * x
// A better macro to avoid precedence issues
#define SQUARE(x) ((x) * (x))

int main() {
    int res1 = BAD_SQUARE(3 + 1);
    int res2 = SQUARE(3 + 1);
    printf("BAD_SQUARE(3 + 1) = %d\n", res1);
    printf("SQUARE(3 + 1) = %d\n", res2);
    return 0;
}