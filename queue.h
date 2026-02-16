#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>

typedef struct node_t node_t;
struct node_t{
    int data;
    node_t* next;
    node_t* prev;
};

typedef struct{
    int size;
    node_t* head;
    node_t* tail;
} queue_t;

queue_t* init_queue();
int queue_enqueue(queue_t* q, int e);
int queue_dequeue(queue_t* q);
int queue_size(queue_t* q);
void destroy_queue(queue_t* q);
void print_queue(queue_t* q);

#endif