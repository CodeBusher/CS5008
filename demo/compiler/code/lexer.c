typedef enum Token_Kind
{
	TOKEN_KEYWORD,
	TOKEN_IDENT,
	TOKEN_OPEN_PAREN,
	TOKEN_CLOSE_PAREN,
	TOKEN_ARROW,
	TOKEN_TYPE,
	TOKEN_OPEN_BRACE,
	TOKEN_CLOSE_BRACE,
	TOKEN_INTEGER,
	TOKEN_STRING,
	TOKEN_EOF,
} Token_Kind;

typedef enum Keyword
{
	KEYWORD_FN,
	KEYWORD_RETURN,
} Keyword;

typedef enum Type
{
	TYPE_INT,
	TYPE_BOOL,
} Type;

typedef struct Loc
{
	const char *file_name;
	long line;
	long col;
} Loc;

typedef struct Token
{
	Token_Kind kind;
	String source; // Identifier name is stored here
	Loc loc;
	
	union
	{
		long long_value;
		Keyword keyword;
		Type type;
	};
} Token;

typedef struct Token_Array
{
	Token *items;
	long count;
	long capacity;
} Token_Array;

typedef struct Lexer
{
	String source;
	long index;
	Loc loc;
} Lexer;

void token_array_append(Token_Array *arr, Token token)
{
	if (arr->count >= arr->capacity)
	{
		arr->capacity = arr->capacity == 0 ? 16 : arr->capacity << 1;
		arr->items = realloc(arr->items, arr->capacity * sizeof(Token));
		if (!arr->items) {
			fprintf(stderr, "Error: Failed to allocate memory for tokens\n");
			exit(1);
		}
	}
	arr->items[arr->count++] = token;
}

Token lex_identifier_or_keyword(Lexer *lexer)
{
	// This is a naive implementation, please improve it as needed
	Token token = {0};
	// You will need to update token.source, token.loc in your implementation
	char *start = lexer->source.data + lexer->index;
	long length = 0;
	char c = lexer->source.data[lexer->index];
	while ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_') {
		lexer->index++;
		length++;
		c = lexer->source.data[lexer->index];
	}
	token.source.data = start;
	token.source.count = length;

	if (length == 2 && strings_match(token.source, str_lit("fn"))) {
		token.kind = TOKEN_KEYWORD;
		token.keyword = KEYWORD_FN;
		return token;
	}
	
	if (length == 6 && strings_match(token.source, str_lit("return"))) {
		token.kind = TOKEN_KEYWORD;
		token.keyword = KEYWORD_RETURN;
		return token;
	} 
	
	token.kind = TOKEN_IDENT;
	return token;
}

Token lex_number(Lexer *lexer)
{
	// This is a naive implementation, please improve it as needed
	Token token = {0};
	char *start = lexer->source.data + lexer->index;
	long length = 0;

	char c = lexer->source.data[lexer->index];
	while (c >= '0' && c <= '9'){
		lexer->index++;
		length++;
		c = lexer->source.data[lexer->index];
	}
	token.kind = TOKEN_INTEGER;
	token.source.data = start;
	token.source.count = length;
	return token;
}

// Functions to consider:
// void skip_whitespace()
// void skip_comments()
// Token next_token()
Token next_token(Lexer *lexer)
{
	// This is a naive implementation, please improve it as needed

	// We assume there is only one whitespace or newline between tokens for in-class demo purposes
	// Hint: please skip all whitespace and comments before trying to find the next token
	// Hint: Directly accessing lexer->source.data[lexer->index] works here,
	//       but a helper function like peek_char(lexer) would:
	// 			- Improve readability
	// 			- Centralize bounds checking
	// 			- Make lookahead operations easier
	if (lexer->source.data[lexer->index] == ' ' || lexer->source.data[lexer->index] == '\n') {
		lexer->index++;
	}

	Token token = {0};
	// You will need to update token.source, token.loc in your implementation
	// For now, we assume in the jive file, there will only be keywords, identifiers, numbers, and EOF
	char c = lexer->source.data[lexer->index];
	
	if (c == '\0') {
		token.kind = TOKEN_EOF;
		return token;
	}
	
	// We can also use isalpha(c) from <ctype.h> to improve readability
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_') {
		// Identifier or keyword
		return lex_identifier_or_keyword(lexer);
	}
	
	// Similarly, we can use isdigit(c) from <ctype.h> to improve readability
	if (c >= '0' && c <= '9') {
		return lex_number(lexer);
	}

	// simplified the unknown character case for in-class demo purposes
    lexer->index++;
    token.kind = TOKEN_EOF;
    return token;
}

// TODO: Print Token_Array to check the output of your lexer
void print_token(Token token)
{
	// This is a naive implementation, please improve it as needed
	switch (token.kind)
		{
		case TOKEN_KEYWORD:
			printf("KEYWORD  %.*s\n", PRINT_STRING(token.source));
			break;
		case TOKEN_TYPE:
			printf("TYPE\n");
			break;
		case TOKEN_IDENT:
			printf("IDENTIFIER %.*s\n", PRINT_STRING(token.source));
			break;
		case TOKEN_INTEGER:
			printf("INTEGER %.*s \n", PRINT_STRING(token.source));
			break;
		case TOKEN_EOF:
			printf("EOF\n");
			break;
		default:
			printf("OTHER\n");
			break;
		}
}

void print_token_array(Token_Array tokens)
{ 
	// Here is a sample output, but you don't have to follow this format exactly. 
	// If you have fun getting things to align, be my guest, but don't stress about it!

	// simple.jive:1:1    KEYWORD      fn
	// simple.jive:1:4    IDENTIFIER   main
	// simple.jive:1:8    (
	// simple.jive:1:9    )
	// simple.jive:1:11   ->
	// simple.jive:1:14   TYPE         int
	// simple.jive:2:1    {
	// simple.jive:3:5    KEYWORD      return
	// simple.jive:3:12   INTEGER      42
	// simple.jive:4:1    }
	// simple.jive:4:2    EOF
	
	for (long i = 0; i < tokens.count; i++) {
		print_token(tokens.items[i]);
	}

}

Token_Array lex_file(const char *file_name)
{	
	Token_Array result = {0};
	// TODO: Read the file into memory
	// Start skipping whitespace and comments
	// Find the next token in a loop
	// Push these onto result Token_Array

	// This is a naive implementation, please improve it as needed
	FILE *file = fopen(file_name, "r");
	if (!file) {
		fprintf(stderr, "Error: Could not open file '%s'\n", file_name);
		return result;
	}

	// This is a naive implementation, which reads one character at a time until a white space/new line/EOF is found.
	// Please improve it as needed.
	
	fseek(file, 0, SEEK_END);
	long file_size = ftell(file);
	fseek(file, 0, SEEK_SET);

	char *buffer = malloc(file_size + 1);
	if (!buffer) {
		fprintf(stderr, "Error: Could not allocate memory for file buffer\n");
		fclose(file);
		return result;
	}

	fread(buffer, 1, file_size, file);
	buffer[file_size] = '\0';

	Lexer lexer = {
		.source = {.data = buffer, .count = file_size},
		.index = 0,
		.loc = {.file_name = file_name, .line = 1, .col = 1},
	};

	while (true) {
		// TODO: Find the next token in buffer and append it to result
		Token token = next_token(&lexer);
		token_array_append(&result, token);
		if (token.kind == TOKEN_EOF) {
			break;
		}
	}

	fclose(file);
	
	// free(buffer); what about the tokens that reference this buffer? what about memory leaks?
	return result;
}
