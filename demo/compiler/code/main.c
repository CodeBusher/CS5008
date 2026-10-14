#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

// Unity build - include all source files
#include "string.c"
#include "lexer.c"
// #include "parser.c" ... We will add more components in future labs

int main(int arg_count, const char **args)
{
	if (arg_count < 2)
	{
		printf("Usage: %s <source_file.jive>\n", args[0]);
		return 1;
	}
	
	const char *file_name = args[1];
	
	//
	// Step 1 of compilation: Lexical Analysis
	//
	
	Token_Array tokens = lex_file(file_name);
	// Successful lexing always includes an EOF token, even for an empty file.
	if (tokens.count == 0) {
		return 1;
	}
	
	bool test_lexer = true;
	if (test_lexer)
	{
		printf("Lexer output:\n");
		print_token_array(tokens);
	}
	
	return 0;
}
