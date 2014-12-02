#ifndef HEAP_H
#define HEAP_H

#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_HEAP hMaxCompare
#define MIN_HEAP hMinCompare

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
hDATA hTop(Heap *h);
int hMaxCompare(hDATA p,hDATA q);
int hMinCompare(hDATA p,hDATA q);
Heap *CreateHeap(int capacity, int (*compare)(hDATA p,hDATA q));
void DownHeap(Heap *h, int i);
void UpHeap(Heap *h, int i);
hDATA hDelete(Heap *h);
void hDeleteIndex(Heap *h,int i);
void ResizeHeap(Heap *h);
int hInsert(Heap *h, hDATA data);
void DestroyHeap(Heap *h);
void BuildHeap(Heap *h,hDATA A[], int n);

#endif
