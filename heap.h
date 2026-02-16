#ifndef HEAP_H
#define HEAP_H

#include <memory.h>

typedef struct {
    int* key;
    int* data; // should be unique
    int size;
    int max;
} heap_t;

heap_t* heap_init(int max);
int heap_insert(heap_t* h, int k, int v);
int heap_updatePriority(heap_t* h, int newK, int v);
int heap_removeMin(heap_t* h);
void heap_destroy(heap_t*);
void heap_print(heap_t*);

#endif