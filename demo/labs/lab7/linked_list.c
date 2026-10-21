#include <stdio.h>
#include <stdlib.h>
typedef struct Node {
    int data;
    struct Node* next;
} Node;

typedef struct DNode{
    int data;
    struct DNode* next;
    struct DNode* prev;
} DNode;

typedef struct LinkedList {
    int size;
    Node* dummyhead;
} LinkedList;

Node* create_node(int data)
{
    Node *node = malloc(sizeof(*node));
    // TODO: Check whether malloc succeeded before accessing the new node.
    node->data = data;
    node->next = NULL;
    return node;
}
    
LinkedList* create_list()
{
    Node *node = malloc(sizeof(*node));
    LinkedList *list = malloc(sizeof(*list));
    list->size = 0;
    list->dummyhead = node;
    node->next = NULL;
    return list;
}

void add_at_head(LinkedList *list, int data)
{
    Node *node = create_node(data);
    node->next = list->dummyhead->next;
    list->dummyhead->next = node;
    list->size++;
}

void add_at_tail(LinkedList *list, int data)
{
    Node *node = create_node(data);
    Node *current = list->dummyhead;
    // Alternative: use the stored size to advance from the dummy head to the tail.
    // for (int i = 0; i < list->size; i++)
    // {
    //     current = current->next;
    // }

    while (current->next != NULL) {
        current = current->next;
    } 

    current->next = node;
    list->size++;
}

void add_at_index(LinkedList *list, int data, int index)
{
    if (index < 0 || index > list->size) {
        return;
    }

    if (index == 0) {
        add_at_head(list, data);
        return;
    }

    if (index == list->size) {
        add_at_tail(list, data);
        return;
    }

    Node *node = create_node(data);
    Node *current = list->dummyhead;
    for (int i = 0; i < index - 1; i++) {
        current = current->next;
    }

    node->next = current->next;
    current->next = node;
    list->size++;
}

void remove_at_head(LinkedList *list)
{
    if (list->size == 0) {
        return;
    }

    Node *node = list->dummyhead->next;
    list->dummyhead->next = node->next;
    free(node);
    list->size--;
}

void remove_at_tail(LinkedList *list)
{
    if (list->size == 0) {
        return;
    }

    Node *current = list->dummyhead;
    while (current->next->next != NULL) {
        current = current->next;
    }

    Node *node = current->next;
    current->next = NULL;
    free(node);
    list->size--;
}

void remove_at_index(LinkedList *list, int index)
{
    // TODO: Remove the node at the given zero-based index.
    // Leave the list unchanged if index is outside [0, size).
    // Follow the boundary-case structure of add_at_index, then unlink and free
    // the selected node and update size in place.
}

// TODO: Extract the repeated traversal into a helper that finds a node by index.
// Define how the helper handles the dummy head and out-of-range indices.

// Print the data nodes; the dummy head does not store a list element.
void print_list(LinkedList *list)
{
    Node *current = list->dummyhead->next;
    while (current != NULL) {
        printf("%d -> ", current->data);
        current = current->next;
    }
    printf("NULL (size: %d)\n", list->size);
}

void clear_list(LinkedList *list)
{
    //TODO: Clear all nodes in the list and leave the dummy head intact.
}

void free_list(LinkedList *list)
{
    //Free all nodes in the list and the list structure itself.
    clear_list(list);
    free(list->dummyhead);
    free(list);
}

int main(void)
{
    LinkedList *list = create_list();

    add_at_head(list, 20);
    add_at_head(list, 10);
    add_at_tail(list, 30);
    printf("After adding head and tail (expected: 10 -> 20 -> 30):\n");
    print_list(list);

    remove_at_head(list);
    printf("After removing head (expected: 20 -> 30):\n");
    print_list(list);

    remove_at_tail(list);
    printf("After removing tail (expected: 20):\n");
    print_list(list);

    add_at_tail(list, 30);
    add_at_index(list, 25, 1);
    // This example exposes the existing off-by-one error in add_at_index.
    printf("After inserting at index 1 (expected: 20 -> 25 -> 30):\n");
    print_list(list);

    //TODO: Test remove_at_index, clear_list, and free_list functions.
    return 0;
}
