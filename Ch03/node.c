#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    char *data;
    struct node *next;
} Node;


int main(){
    Node *head = (Node *)malloc(sizeof(Node)); // head: Tuesday
    head->data = "Tuesday";
    head->next = NULL;
    
    Node *q = (Node *)malloc(sizeof(Node));  // Tuesday -> Thursday
    q->data = "Thursday";
    q->next = NULL;
    head->next = q;

    q = (Node *)malloc(sizeof(Node)); // head: Monday, Monday -> Tuesday -> Thursday
    q->data = "Monday";
    q->next = head;
    head = q;

    Node *p = head;
    while(p != NULL){
        printf("%s\n", p->data);
        p = p->next;
    }
    return 0;
}