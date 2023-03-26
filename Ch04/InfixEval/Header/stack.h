#ifndef STACK_H
#define STACK_H

#include <stdbool.h> // C99

typedef int Item;
typedef struct stack *Stack;

Stack create();
void destroy(Stack stack);
void make_empty(Stack stack);
bool is_empty(Stack stack);
void push(Stack stack, Item item);
Item pop(Stack stack);
Item peek(Stack stack);

#endif // STACK_H