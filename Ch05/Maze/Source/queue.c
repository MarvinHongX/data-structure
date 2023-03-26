#include <stdio.h>
#include <stdlib.h>
#include "../Header/queue.h"

typedef struct node {
    Item data;
    struct node *next;
} Node;

struct queue {
    struct node *front;
    struct node *rear;
    int size;
};

Node *create_node_instance(Item item);

static void terminate(const char *message){
    printf("%s\n", message);
    exit(EXIT_FAILURE);
}

int get_size(Queue queue){
    return queue->size;
}

Queue create(){
    Queue new_queue = (Queue)malloc(sizeof(struct queue));
    if (new_queue == NULL)
        terminate("Error in create: queue couldn't be created.");
    new_queue->front = NULL;
    new_queue->rear = NULL;
    new_queue->size = 0;
    return new_queue;
}

void destroy(Queue queue){
    make_empty(queue);
    free(queue);
}

void make_empty(Queue queue){
    while (!is_empty(queue))
        dequeue(queue);
    queue->size = 0;
} 

bool is_empty(Queue queue){
    return queue->front == NULL;
}

Node *create_node_instance(Item item){
    Node *node = (Node *)malloc(sizeof(Node));

    if (node == NULL)
        terminate("Error in push: queue is full.");
        
    node->data = item;

    return node;
}

void enqueue(Queue queue, Item item){
    Node *node = create_node_instance(item);
    node->next = NULL;
    if (queue->front == NULL){ // queue is empty
        queue->front = node;
        queue->rear = node;
    } else {
        queue->rear->next = node;
        queue->rear = node;
    }
    queue->size++;
}

Item dequeue(Queue queue){
    if (is_empty(queue))
        terminate("Error in dequeue: queue is empty.");

    Item item;
    Node *front = queue->front;
    item = front->data;
    queue->front = front->next;
    if (queue->front == NULL)
        queue->rear = NULL;
    free(front);
    queue->size--;
    return item;
}

Item peek(Queue queue){
    if (is_empty(queue))
        terminate("Error in dequeue: queue is empty.");
    return queue->front->data;
}