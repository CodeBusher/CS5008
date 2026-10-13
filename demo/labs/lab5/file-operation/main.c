#include <stdio.h>   // FILE, fopen, fseek, ftell, fread, fclose
#include <stdlib.h>  // malloc, free

// Build: gcc -Wall main.c -o file_demo
// Run:   ./file_demo main.c
// This small demo assumes a regular text file on macOS/Linux and successful
// seeking and reading. A fuller version would check fseek, ftell, and fread.
int main(int argc, char *argv[]) {
    // argc counts all arguments, including the program name.
    // argv[0] is "./file_demo"; argv[1] is the file name supplied by the user.
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file_name>\n", argv[0]);
        return 1;
    }

    const char *file_name = argv[1];

    // "r" opens an existing text file for reading; it does not modify the file.
    // FILE * lets the standard library track the open file and its position.
    // fopen returns NULL if it cannot open the file.
    FILE *file = fopen(file_name, "r");
    if (!file) {
        fprintf(stderr, "Error: could not open '%s'.\n", file_name);
        return 1;
    }

    // fseek(file, offset, origin) moves the current position without reading.
    // SEEK_END means the end; offset 0 means move exactly to that position.
    fseek(file, 0, SEEK_END);

    // ftell reports the current position. At the end, this gives the file size
    // in bytes for this demo: a file containing "Hello" has an end position of 5.
    long file_size = ftell(file);

    // SEEK_SET means the beginning. Reset the position so fread starts there.
    // Otherwise, we would still be at the end and there would be nothing to read.
    fseek(file, 0, SEEK_SET);
    
    // Allocate space for the file's bytes plus one extra byte for '\0'.
    // malloc returns NULL if allocation fails.
    char *buffer = malloc(file_size + 1);
    if (!buffer) {
        fprintf(stderr, "Error: could not allocate memory for the file buffer.\n");
        fclose(file); // Close the file even when allocation fails.
        return 1;
    }

    // fread(destination, bytes_per_item, item_count, file) reads into memory.
    // Here we request file_size items of 1 byte each, including spaces/newlines.
    // It returns the number of items actually read; this demo assumes a full read.
    fread(buffer, 1, file_size, file);
    
    // fread does not append '\0'. Add it ourselves for use as a C string.
    // An empty file is valid: buffer[0] becomes '\0'.
    buffer[file_size] = '\0';

    // Close the file when finished reading. The allocated buffer still exists.
    fclose(file);

    // Use buffer here, for example: printf("%s", buffer);
    // This demo does not print the contents unless you add that line.

    // Release the allocated memory. Do not use buffer after free.
    free(buffer);

    // 0 indicates success; the earlier return 1 statements indicate errors.
    return 0;
}
