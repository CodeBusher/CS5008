#include <stdio.h>
#include <stdlib.h>

#define T 2

typedef struct String {
    char *data;
    long count;
} String;

#define PRINT_STRING(s) (int)(s).count, (s).data
#define str_lit(s) (String){(char *)(s), sizeof(s) - 1}

typedef enum {
    AST_NONE,

    AST_PROGRAM,
    AST_FN,
    AST_TYPE,
    AST_RETURN,
    AST_INTEGER
} AST_Kind;

typedef enum {
    TYPE_NONE,

    TYPE_INT,
    TYPE_BOOL
} Type;

static const String type_names[] = {
    [TYPE_NONE] = {"none", 4},
    [TYPE_INT] = {"int", 3},
    [TYPE_BOOL] = {"bool", 4}
};

typedef struct AST_Node AST_Node;

typedef struct AST_List {
    long count;
    AST_Node *first;
    AST_Node *last;
} AST_List;

typedef struct AST_Fn_Data {
    String name;
    AST_List parameters;
    Type return_type;
    AST_List body;
} AST_Fn_Data;

struct AST_Node {
    AST_Kind kind;
    AST_Node *prev;
    AST_Node *next; /* The next sibling in a program or function body. */

    /* The kind tells us which member of this union to use. */
    union {
        AST_List program;
        AST_Fn_Data fn;
        Type type;
        AST_Node *ret_expr;
        long int_value;
    };
};

/* The caller owns the allocated node and must free it when finished. */
AST_Node *create_ast_node(AST_Kind kind)
{
    AST_Node *node = calloc(1, sizeof *node);
    if (node == NULL) {
        fprintf(stderr, "Failed to allocate an AST node\n");
        exit(EXIT_FAILURE);
    }

    /* calloc zeroes the allocated memory; set the node's kind afterward. */
    node->kind = kind;
    return node;
}

void print_ast_with_indent(AST_Node *node, int depth)
{
    if (node == NULL) {
        return;
    }

    switch (node->kind) {
    case AST_NONE: {
        printf("%*sAST_NONE (ERROR!)\n", T * depth, "");
    } break;

    case AST_PROGRAM: {
        printf("%*sprogram\n", T * depth, "");
        for (AST_Node *fn_node = node->program.first;
             fn_node != NULL; fn_node = fn_node->next) {
            print_ast_with_indent(fn_node, depth + 1);
        }
    } break;

    case AST_FN: {
        printf("%*sfn %.*s\n", T * depth, "", PRINT_STRING(node->fn.name));
        for (AST_Node *body_node = node->fn.body.first;
             body_node != NULL; body_node = body_node->next) {
            print_ast_with_indent(body_node, depth + 1);
        }
    } break;

    case AST_TYPE: {
        if (node->type == TYPE_NONE) {
            printf("%*stype none\n", T * depth, "");
        } else {
            printf("%*stype %.*s\n", T * depth, "", PRINT_STRING(type_names[node->type]));
        }
    } break;

    case AST_RETURN: {
        printf("%*sreturn\n", T * depth, "");
        if (node->ret_expr != NULL) {
            print_ast_with_indent(node->ret_expr, depth + 1);
        }
    } break;

    case AST_INTEGER: {
        printf("%*sinteger %ld\n", T * depth, "", node->int_value);
    } break;

    default: {
        printf("%*sUNHANDLED AST_KIND: %d\n", T * depth, "", node->kind);
    } break;
    }
}

void demo(void)
{
    /*
     * Example source:
     *   fn main() -> int { return 40; }
     *   fn foo() -> int { return 42; }
     *
     * Expected tree:
     * program
     *   fn main
     *     return
     *       integer 40
     *   fn foo
     *     return
     *       integer 42
     *
     * Build from the program downward, filling in links as nodes are created.
     * Nodes live on the heap until we explicitly free them after printing.
     */

    /* 1. Create the program before it has any functions. */
    AST_Node *prog = create_ast_node(AST_PROGRAM);

    /* 2. Expand main: function -> data -> body -> return -> integer. */
    AST_Node *fn_main = create_ast_node(AST_FN);
    AST_Fn_Data fn_main_data = {
        .name = str_lit("main"),
        .return_type = TYPE_INT
    };
    /* A body is an AST_List of statements, not an AST_Node. */
    AST_List fn_main_body = {0};

    AST_Node *return_40 = create_ast_node(AST_RETURN);
    AST_Node *integer_40 = create_ast_node(AST_INTEGER);
    integer_40->int_value = 40;
    return_40->ret_expr = integer_40;

    /* Fill the body, then copy the completed data into the function node. */
    fn_main_body.count = 1;
    fn_main_body.first = return_40;
    fn_main_body.last = return_40;
    fn_main_data.body = fn_main_body;
    fn_main->fn = fn_main_data;

    /* 3. Expand foo in the same order. */
    AST_Node *fn_foo = create_ast_node(AST_FN);
    AST_Fn_Data fn_foo_data = {
        .name = str_lit("foo"),
        .return_type = TYPE_INT
    };
    AST_List fn_foo_body = {0};

    AST_Node *return_42 = create_ast_node(AST_RETURN);
    AST_Node *integer_42 = create_ast_node(AST_INTEGER);
    integer_42->int_value = 42;
    return_42->ret_expr = integer_42;

    fn_foo_body.count = 1;
    fn_foo_body.first = return_42;
    fn_foo_body.last = return_42;
    fn_foo_data.body = fn_foo_body;
    fn_foo->fn = fn_foo_data;

    /* 4. Connect the completed functions and attach them to the program. */
    fn_main->next = fn_foo;
    fn_foo->prev = fn_main;
    AST_List program_functions = {.count = 2, .first = fn_main, .last = fn_foo};
    prog->program = program_functions;

    /* 5. Walk the completed tree from its root. */
    printf("Example source:\n"
           "  fn main() -> int { return 40; }\n"
           "  fn foo() -> int { return 42; }\n"
           "\nAST tree:\n");
    print_ast_with_indent(prog, 0);

    /* 6. Free each allocated node once, after all traversal is finished.
     * The list/data structs are local values; the names are string literals. */
    free(integer_40);
    free(return_40);
    free(fn_main);
    free(integer_42);
    free(return_42);
    free(fn_foo);
    free(prog);
}

int main(void)
{
    demo();
    return 0;
}
