void generate_preamble(FILE *out_file)
{
	fprintf(out_file, "global _start\n");
	fprintf(out_file, "\n");
	fprintf(out_file, "_start:\n");
	fprintf(out_file, "    call main\n");
	fprintf(out_file, "    mov rdi, rax\n");
	fprintf(out_file, "    mov rax, 60\n");
	fprintf(out_file, "    syscall\n");
	fprintf(out_file, "\n");
}

// TODO: Implement additional helper functions as desired

bool generate_asm_for_fn(AST_Node *fn_node, FILE *out_file)
{
	// TODO: Implement this
	// TODO: Print the function name as a label
	// TODO: Iterate over the body of the function
	//       TODO: For each statement, generate the asm
	//       NOTE: Only return statements need to be supported for now
	//             and the only expression you need to handle is an integer literal
	// TODO: Return true on successful generation, and false on error
	
	return true;
}

bool generate_asm(AST_Node *ast, FILE *out_file)
{
	if (ast == NULL || ast->kind != AST_PROGRAM)
	{
		printf("ERROR: Root AST node was not PROGRAM. Got kind %s\n", ast_kind_as_cstr(ast->kind));
		return false;
	}
	
	generate_preamble(out_file);
	
	for (AST_Node *fn_node = ast->program.first; fn_node != NULL; fn_node = fn_node->next)
	{
		bool success = generate_asm_for_fn(fn_node, out_file);
		if (!success) return false;
	}
	
	return true;
}