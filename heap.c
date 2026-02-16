#include "heap.h"

int swap(heap_t* h, int i, int j){
    int temp = h->data[i];
    h->data[i] = h->data[j];
    h->data[j] = temp;
    temp = h->key[i];
    h->key[i] = h->key[j];
    h->key[j] = temp;
}

heap_t* heap_init(int max){
    heap_t* h = calloc(1, sizeof(heap_t));
    h->data = calloc(max, sizeof(int));
    h->key = calloc(max, sizeof(int));
    h->max = max;
    h->size = 0;
    return h;
}

int heapifyUp(heap_t* h, int i){
    while(i > 0 && h->key[(i-1)/2] > h->key[i]){
        swap(h, (i-1)/2, i);
        i = (i-1)/2;
    }
}

int heap_insert(heap_t* h, int k, int v){
    if(h->size == h->max){
        printf("error adding to heap: heap is full\n");
        return 1;
    }
    h->data[h->size] = v;
    h->key[h->size] = k;
    heapifyUp(h, h->size);
    h->size++;
    return 0;
}

int heap_updatePriority(heap_t* h, int newK, int v){
    for(int i = 0; i < h->size; i++){
        if(h->data[i] == v){
            h->key[i] = newK;
            heapifyUp(h, i);
        }
    }
}

int heap_removeMin(heap_t* h){
    if(h->size == 0){
        printf("error removing from heap: heap was empty\n");
        return 0;
    }
    int root = h->data[0];
    swap(h, 0, h->size-1);
    h->size--;
    int i = 0;
    while(2*i + 1 < h->size){
      int left = 2*i + 1;
      int right = 2*i + 2;
      int min = left;
      if(right < h->size && h->key[right] < h->key[left]){
        min = right;
      }
      if(h->key[i] > h->key[min]){
        swap(h, i, min);
        i = min;
      }
      else{
        break;
      }
    }
    return root;
}

void heap_destroy(heap_t* h){
    free(h->data);
    free(h);
}

void heap_print(heap_t* h){
    if(h == NULL || h->size == 0){
        printf("Empty heap\n");
        return;
    }
    for(int i = 0; i < h->size; i++){
        printf("%d:%d, ", h->key[i], h->data[i]);
    }
    printf("\n");
}