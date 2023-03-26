#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h> // C99
#include "../Header/pos.h"

typedef Position Item;
typedef struct queue *Queue;

Queue create();
void destroy(Queue queue);
void make_empty(Queue stack);
bool is_empty(Queue queue);
void enqueue(Queue queue, Item item);
Item dequeue(Queue queue);
Item peek(Queue queue);
int get_size(Queue queue);

#endif // QUEUE_H