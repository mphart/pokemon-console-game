#include "queue.h"

queue_t* init_queue(){
    queue_t* q = calloc(1, sizeof(queue_t));
    q->size = 0;
    q->head = NULL;
    q->tail = q->head;
    return q;
}

int queue_enqueue(queue_t* q, int e){
    node_t* newNode = calloc(1, sizeof(node_t));
    if(q->head == NULL){
        newNode->next = NULL;
        newNode->prev = NULL;
        newNode->data = e;
        q->head = newNode;
        q->tail = newNode;
    } else {
        q->tail->next = newNode;
        newNode->next = NULL;
        newNode->prev = q->tail;
        newNode->data = e;
        q->tail = newNode;
    }
    q->size++;
    return e;
}

int queue_dequeue(queue_t* q){
    if(q->head == NULL){
        printf("Illegal state exception: dequeueing from an empty queue\n");
        return 0;
    }
    if(q->head == q->tail){
        q->tail = NULL;
    }
    int payload = q->head->data;
    node_t* nodeToRemove = q->head;
    q->head = q->head->next;
    if(q->head != NULL){
        q->head->prev = NULL;
    }
    free(nodeToRemove);
    q->size--;
    return payload;
}

int queue_size(queue_t* q){
    return q->size;
}

void destroy_queue(queue_t* q){
    node_t* curr = q->head;
    if(curr != NULL){
        while(curr->next != NULL){
            curr = curr->next;
            free(curr->prev);
        }
        free(curr);
    }
    free(q);
}

void print_queue(queue_t* q){
    if(q->head == NULL){
        printf("Empty queue\n");
    } else {
        node_t* curr = q->head;
        while(curr != NULL){
            printf("%d -> ", curr->data);
            curr = curr->next;
        }
        printf("\n");
    }
}