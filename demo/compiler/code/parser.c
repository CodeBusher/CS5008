typedef enum AST_Kind
{
	AST_NONE,
	
	AST_PROGRAM,
	AST_FN,
	AST_TYPE,
	AST_RETURN,
	AST_INTEGER,
	// TODO: Add more as needed
} AST_Kind;

const char *ast_kind_as_cstr(AST_Kind kind)
{
	switch (kind)
	{
	case AST_NONE:    return "NONE (ERROR!)";
	case AST_PROGRAM: return "PROGRAM";
	case AST_FN:      return "FN";
	case AST_TYPE:    return "TYPE";
	case AST_RETURN:  return "RETURN";
	case AST_INTEGER: return "INTEGER";
		// TODO: Handle additional cases as you add kinds
	default:          return "UNKNOWN (ERROR!)";
	}
}

typedef struct AST_Node AST_Node;

typedef struct AST_List // Doubly linked list of AST_Nodes
{
	long count;
	AST_Node *first;
	AST_Node *last;
} AST_List;

typedef struct AST_Fn_Data
{
	String name;
	AST_List parameters;
	Type return_type;
	AST_List body;
} AST_Fn_Data;

struct AST_Node
{
	AST_Kind kind;
	AST_Node *prev;
	AST_Node *next; // Used to make nodes into a doubly-linked list
	
	union // This stores the extra data for the particular kind of node
	{
		AST_List    program;   // Data for AST_PROGRAM
		AST_Fn_Data fn;        // Data for AST_FN
		// Type        type;      // Data for AST_TYPE
		AST_Node   *ret_expr;  // Data for AST_RETURN
		long        int_value; // Data for AST_INTEGER
	};
};

typedef struct Parser
{
	Token_Array tokens;
	long tok_index;
	bool has_error; // Keep track of if we've encountered an error
} Parser;

AST_Node *make_ast_node(AST_Kind kind)
{
	AST_Node *result = calloc(1, sizeof(AST_Node));
	result->kind = kind;
	return result;
}

void report_error(Parser *parser, Token *tok, const char *message)
{
	print_loc(tok->loc);
	printf(": %s", message);
	parser->has_error = true;
}

Token *peek_token(Parser *parser, int offset)
{
	long index = parser->tok_index + offset;
	if (index >= parser->tokens.count)
	{
		Token *EOF_Token = &parser->tokens.items[parser->tokens.count - 1];
		return EOF_Token;
	}
	return &parser->tokens.items[index];
}

Token *expect_token(Parser *parser, Token_Kind expected_kind)
{
	Token *actual = peek_token(parser, 0);
	
	if (actual->kind != expected_kind) {
		report_error(parser, actual, "ERROR: Unexpected token\n");
		printf("ERROR: Expected ");
		print_token_kind(expected_kind);
		printf(", got ");
		print_token_kind(actual->kind);
		printf("\n");
		return actual;
	}
	
	++parser->tok_index; // Advance to next token
	
	return actual;
}

Token *expect_keyword(Parser *parser, Keyword expected_keyword)
{
	Token *actual = expect_token(parser, TOKEN_KEYWORD);
	if (parser->has_error) return actual;
	
	if (actual->keyword != expected_keyword)
	{
		report_error(parser, actual, "ERROR: Unexpected keyword\n");
		printf("Expected keyword %.*s, got ", PRINT_STRING(keyword_names[expected_keyword]));
		print_token(actual);
		printf("\n");
	}
	
	return actual;
}

void ast_list_append(AST_List *list, AST_Node *node)
{
	// TODO: Implement appending to a doubly-linked list
}

AST_List parse_block(Parser *parser)
{
	AST_List result = {0};
	
	expect_token(parser, '{');
	if (parser->has_error) return result;
	
	Token *tok = peek_token(parser, 0);
	while (tok->kind != '}' && tok->kind != TOKEN_EOF)
	{
		// TODO: Parse the statements of the block
		// TODO: Add them to the result
		// NOTE: At this point in the code, we should expect tok to be the first
		//       token of a statement, and we have not advanced the parser past
		//       this token
	}
	expect_token(parser, '}');
	
	return result;
}

AST_Node *parse_fn_def(Parser *parser)
{
	AST_Node *result = make_ast_node(AST_FN);
	
	expect_keyword(parser, KEYWORD_fn);
	if (parser->has_error) return result;
	
	Token *name = expect_token(parser, TOKEN_IDENT);
	if (parser->has_error) return result;
	
	expect_token(parser, '(');
	if (parser->has_error) return result;
	
	// Parse parameters (TODO later)
	
	expect_token(parser, ')');
	if (parser->has_error) return result;
	
	// Check for ->
	Type return_type = TYPE_NONE;
	Token *maybe_arrow = peek_token(parser, 0);
	
	if (maybe_arrow->kind == TOKEN_ARROW)
	{
		++parser->tok_index; // Advance past arrow
		
		Token *ret_type_token = expect_token(parser, TOKEN_TYPE);
		if (parser->has_error) return result;
		
		return_type = ret_type_token->type; // Since it's a type, it has type information
	}
	
	// For parsing the body of the function, we should probably have a dedicated
	// parse_block function that we can reuse for other places blocks occur,
	// like if and while statements.
	AST_List body = parse_block(parser);
	if (parser->has_error) return result;
	// TODO: Fill this result out with the information we gathered above
	
	return result;
}

// We could just return NULL on error, but it might be nice to see
// the partial result that was created before the error ocurred
typedef struct Parse_Result
{
	AST_Node *ast;
	bool success;
} Parse_Result;

Parse_Result parse_program(Token_Array tokens)
{
	Parse_Result result = {
		.ast = make_ast_node(AST_PROGRAM),
		.success = true,
	};
	
	Parser parser = {
		.tokens = tokens,
		.tok_index = 0,
	};
	while (parser.tok_index < tokens.count)
	{
		// I'm parsing a program
		// So I should expect a list of function definition
		// So at the top of the while loop I'll assume we are at the start of a function definition
		AST_Node *fn_def = parse_fn_def(&parser);
		
		// append this to the list that the program keeps
		ast_list_append(&result.ast->program, fn_def);
	}
	result.success = !parser.has_error;
	return result;
}

void print_ast_with_indent(AST_Node *node, int depth)
{
	switch (node->kind)
	{
	case AST_NONE: {
		printf("%*sAST_NONE (ERROR!)\n", 2*depth, "");
	} break;
	
	case AST_PROGRAM: {
		printf("%*sprogram\n", 2*depth, "");
		for (AST_Node *fn_node = node->program.first; fn_node != NULL; fn_node = fn_node->next)
		{
			print_ast_with_indent(fn_node, depth + 1);
		}
	} break;
	
	case AST_FN: {
		printf("%*sfn %.*s(\n", 2*depth, "", PRINT_STRING(node->fn.name));
		// TODO: Print function parameters
		printf(")\n");
		for (AST_Node *body_node = node->fn.body.first; body_node != NULL; body_node = body_node->next)
		{
			print_ast_with_indent(body_node, depth + 1);
		}
	} break;
	
	case AST_TYPE: {
		printf("%*stype %.*s\n", 2*depth, "", PRINT_STRING(type_names[node->type]));
	} break;
	
	case AST_RETURN: {
		printf("%*sreturn\n", 2*depth, "");
		if (node->ret_expr != NULL)
		{
			print_ast_with_indent(node->ret_expr, depth + 1);
		}
	} break;
	
	case AST_INTEGER: {
		printf("%*sinteger %ld\n", 2*depth, "", node->int_value);
	} break;
	
	default: {
		printf("%*sUNHANDLED AST_KIND: %d\n", 2*depth, "", node->kind);
	} break;
	}
}

void print_ast(AST_Node *node)
{
	print_ast_with_indent(node, 0);
}