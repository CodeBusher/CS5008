#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

// This is set up for unity build
#include "string.c"
#include "lexer.c"
#include "parser.c"
#include "codegen.c"

typedef struct Options
{
	const char *out_file_name;
	const char *in_file_name;
} Options;

void print_usage(const char *program_name)
{
	printf("Usage: %s input_file.jive [-o output_file.asm]\n", program_name);
}

int main(int arg_count, const char **args)
{
	//
	// Parse command line arguments
	//
	
	Options options = {
		.out_file_name = "out.asm", // Default output file name
		.in_file_name  = NULL,
	};
	
	int arg_index = 0;
	const char *program_name = args[arg_index++];
	
	while (arg_index < arg_count)
	{
		const char *arg = args[arg_index++];
		if (strcmp(arg, "-o") == 0) // Set output file name
		{
			if (arg_index < arg_count)
			{
				options.out_file_name = args[arg_index++];
			}
			else
			{
				printf("ERROR: Missing output file name after -o flag.\n");
				return 1; // Exit with error
			}
		}
		else if (options.in_file_name == NULL) // If no flag, set input file name
		{
			options.in_file_name = arg;
		}
		else
		{
			printf("WARNING: Unrecognized command line argument %s\n", arg);
		}
	}
	
	if (options.in_file_name == NULL)
	{
		printf("ERROR: No input file supplied.\n");
		print_usage(program_name);
		return 1; // Exit with error
	}
	
	//
	// Step 1 of compilation: Lexical Analysis
	//
	
	Token_Array tokens = lex_file(options.in_file_name);
	
	bool test_lexer = true;
	if (test_lexer)
	{
		printf("Lexer output:\n");
		print_token_array(tokens);
	}
	
	//
	// Step 2 of compilation: Parsing tokens into an Abstract Syntax Tree (AST)
	//
	
	Parse_Result parse_result = parse_program(tokens);
	if (!parse_result.success)
	{
		printf("ERROR: Failed to parse.\n");
		return 1; // Exit with error
	}
	
	bool test_parser = true;
	if (test_parser)
	{
		printf("Parser output:\n");
		print_ast(parse_result.ast);
	}
	
	//
	// Step 3 of compilation: Generate asm code by traversing AST
	//
	
	FILE *out_file = fopen(options.out_file_name, "w");
	if (!out_file)
	{
		printf("ERROR: Could not open %s for writing.\n", options.out_file_name);
		return 1; // Exit with error
	}
	
	generate_asm(parse_result.ast, out_file);
	
	fclose(out_file);
	
	return 0; // Exit with success
}