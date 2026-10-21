#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

void print_Node(Node node)
{
    printf("print_Node\n");
    printf("node: address = %p, data = %d, next = %p\n", (void*)&node, node.data, (void*)node.next);
}
int main()
{
    Node node1 = {1, NULL};
    Node node2 = {2, NULL};
    node1.next = &node2;

    print_Node(node1);
    print_Node(node2);
    printf("----\n");
    printf("node1: address = %p, data = %d, next = %p\n", (void*)&node1, node1.data, (void*)node1.next);
    printf("node2: address = %p, data = %d, next = %p\n", (void*)&node2, node2.data, (void*)node2.next);
    return 0;
}