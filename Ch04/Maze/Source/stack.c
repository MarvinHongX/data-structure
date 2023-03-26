#include <stdio.h>
#include <stdlib.h>
#include "../Header/stack.h"

typedef struct node {
    Item data;
    struct node *next;
} Node;

struct stack {
    struct node *top;
};

Node *create_node_instance(Item item);

static void terminate(const char *message){
    printf("%s\n", message);
    exit(EXIT_FAILURE);
}

Stack create(){
    Stack new_stack = (Stack)malloc(sizeof(struct stack));
    if (new_stack == NULL)
        terminate("Error in create: stack couldn't be created.");
    new_stack->top = NULL;
    return new_stack;
}

void destroy(Stack stack){
    make_empty(stack);
    free(stack);
}

void make_empty(Stack stack){
    while (!is_empty(stack))
        pop(stack);
} 

bool is_empty(Stack stack){
    return stack->top == NULL;
}

Node *create_node_instance(Item item){
    Node *node = (Node *)malloc(sizeof(Node));

    if (node == NULL)
        terminate("Error in push: stack is full.");
        
    node->data = item;

    return node;
}

void push(Stack stack, Item item){
    Node *node = create_node_instance(item);
    node->next = stack->top;
    stack->top = node;
}

Item pop(Stack stack){
    if (is_empty(stack))
        terminate("Error in pop: stack is empty.");

    Item item;
    Node *top = stack->top;
    item = top->data;
    stack->top = top->next;
    free(top);

    return item;
}

Item peek(Stack stack){
    if (is_empty(stack))
        terminate("Error in pop: stack is empty.");
    return stack->top->data;
}

