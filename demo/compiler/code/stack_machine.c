// For any struct that has fields
//   items
//   count
//   capacity
// use this to append items to the dynamic array
#define da_append(da, item) \
	do { \
		if ((da)->count >= (da)->capacity) \
		{ \
			(da)->capacity *= 2; \
			if ((da)->capacity == 0) \
			{ \
				(da)->capacity = 16; \
			} \
			(da)->items = realloc((da)->items, (da)->capacity*sizeof((da)->items[0])); \
		} \
		(da)->items[(da)->count++] = (item); \
	} \
	while (false)

#define da_pop(da) (da)->items[--(da)->count]

typedef enum SM_Kind
{
	SM_NONE,
	
	SM_FN,
	SM_FN_END,
	
	SM_PUSH,
	SM_ADD,
	SM_SUB,
	SM_MUL,
	SM_DIV,
	SM_MOD,
} SM_Kind;

typedef struct SM_Instruction
{
	SM_Kind kind;
	long int_value; // For instance PUSH instructions have this property
	// TODO: Add more fields as needed (perhaps make a union?)
} SM_Instruction;

typedef struct Stack_Machine_Instructions
{
	SM_Instruction *items;
	long count;
	long capacity;
} Stack_Machine_Instructions;

SM_Instruction make_push(long value)
{
	SM_Instruction result = {
		.kind = SM_PUSH,
		.int_value = value,
	};
	return result;
}

SM_Instruction make_add(void)
{
	SM_Instruction result = {
		.kind = SM_ADD,
	};
	return result;
}

SM_Instruction make_sub(void)
{
	SM_Instruction result = {
		.kind = SM_SUB,
	};
	return result;
}

SM_Instruction make_mul(void)
{
	SM_Instruction result = {
		.kind = SM_MUL,
	};
	return result;
}

SM_Instruction make_div(void)
{
	SM_Instruction result = {
		.kind = SM_DIV,
	};
	return result;
}

SM_Instruction make_mod(void)
{
	SM_Instruction result = {
		.kind = SM_MOD,
	};
	return result;
}

Stack_Machine_Instructions generate_stack_machine_instructions(AST_Node *ast)
{
	Stack_Machine_Instructions result = {0};
	
	// TODO: Generate stack machine instruction list out of the ast
	
	return result;
}

//
// Simulation of the Stack Machine
// NOTE: This can be used to interpret jive rather than compile it.
//       This step is not necessary when compiling to assembly code.
//       But it could be useful for evaluating constant expressions at compile time to generate more optimized code.
//       However, don't do that for this assignment. Please generate all of the code as assembly.
//

typedef struct Stack
{
	long *items;
	long count;
	long capacity;
} Stack;

long simulate_stack_machine(Stack_Machine_Instructions *instructions)
{
	Stack stack_ = {0};
	Stack *stack = &stack_;
	
	for (long ip = 0; ip < instructions->count; ++ip)
	{
		SM_Instruction instruction = instructions->items[ip];
		switch (instruction.kind)
		{
		case SM_PUSH:
			{
				da_append(stack, instruction.int_value);
			} break;
		case SM_ADD:
			{
				long b = da_pop(stack);
				long a = da_pop(stack);
				long result = a + b;
				da_append(stack, result);
			} break;
		case SM_SUB:
			{
				long b = da_pop(stack);
				long a = da_pop(stack);
				long result = a - b;
				da_append(stack, result);
			} break;
		case SM_MUL:
			{
				long b = da_pop(stack);
				long a = da_pop(stack);
				long result = a * b;
				da_append(stack, result);
			} break;
		case SM_DIV:
			{
				long b = da_pop(stack);
				long a = da_pop(stack);
				long result = a / b;
				da_append(stack, result);
			} break;
		case SM_MOD:
			{
				long b = da_pop(stack);
				long a = da_pop(stack);
				long result = a % b;
				da_append(stack, result);
			} break;
		default:
			{
				printf("ERROR: Encountered unhandled stack machine instruction: %d\n", instruction.kind);
			} break;
		}
	}
	
	return stack->items[0];
}