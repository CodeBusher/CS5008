#include <stdio.h>
#include <stdbool.h> // bool, true, false

// Store a pointer to characters and the number of characters in the string.
// Unlike a C string, this representation does not need a trailing '\0'.
typedef struct String {
    char *data;
    long count;
} String;

// Expands into two printf arguments: the length and the character pointer.
// The precision argument for %.*s must be an int, so we cast count to int.
#define PRINT_STRING(s) (int)(s).count, (s).data

// Use this with string literals, such as str_lit("Hello"), not char * variables.
// sizeof includes the final '\0', so subtract 1 to count only the text.
// (String){...} creates a String value. Do not modify a string literal's data.
#define str_lit(s) (String){(char *)(s), sizeof(s) - 1}

bool strings_match(String s1, String s2) {
    if (s1.count != s2.count) {
        return false;
    }

    // Equal lengths are not enough: compare each character too.
    for (long i = 0; i < s1.count; ++i) {
        if (s1.data[i] != s2.data[i]) {
            return false;
        }
    }

    return true;
}

int main(void) {
    // Double quotes create strings; single quotes are for characters like 'H'.
    char msg[] = "Hello, World!";

    // .5 prints at most 5 characters; .* takes that limit from an argument.
    printf("%.5s\n", msg);       // Hello
    printf("%.*s\n", 5, msg);    // Hello

    // Point to the first 5 characters without copying or changing msg.
    String s = {msg, 5};
    printf("%.*s\n", (int)s.count, s.data); // Hello
    printf("%.*s\n", PRINT_STRING(s));     // Hello

    // A substring: skip "Hello, " (7 characters), then use the next 5.
    String word = {msg + 7, 5};
    printf("%.*s\n", PRINT_STRING(word));  // World

    String greeting = str_lit("Hello");
    printf("%.*s\n", PRINT_STRING(greeting));

    // %d prints a bool as 1 (true) or 0 (false).
    printf("Same text: %d\n", strings_match(s, greeting));           // 1
    printf("Different text: %d\n", strings_match(s, word));         // 0
    printf("Different lengths: %d\n", strings_match(s, str_lit("Hi"))); // 0

    return 0;
}
