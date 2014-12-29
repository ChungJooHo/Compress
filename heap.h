#ifndef HEAP_H
#define HEAP_H

#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

typedef void* hDATA;
typedef struct Heap{
  hDATA *arr;
  int count;
  int capacity;
  int (*compare)(hDATA p,hDATA q);
}Heap;

int hParent(Heap *h, int i);
int hLeftChild(Heap *h, int i);
int hRightChild(Heap *h, int i);
Heap *CreateHeap(int capacity, int (*compare)(hDATA p,hDATA q));
void DownHeap(Heap *h, int i);
void UpHeap(Heap *h, int i);
hDATA hDelete(Heap *h);
int hInsert(Heap *h, hDATA data);

#endif
