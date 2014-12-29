#include "heap.h"

// 힙의 부모를 찾는 함수 
int hParent(Heap *h, int i)
{
  if(i<=0||i>=h->count)return -1;
  return (i-1)/2;
}

// 힙의 왼쪽 자식을 찾는 함수 
int hLeftChild(Heap *h, int i)
{
  int t=2*i+1;
  if(t>=h->count)return -1;
  return t;
}

// 힙의 오른쪽 자식을 찾는 함수 
int hRightChild(Heap *h, int i)
{
  int t=2*i+2;
  if(t>=h->count)return -1;
  return t;
}

// 힙을 생성하는 함수 
Heap *CreateHeap(int capacity, int (*compare)(hDATA p,hDATA q))
{
  Heap *h=(Heap *)malloc(sizeof(Heap));
  
  // 힙을 초기화해줌 
  h->count=0;
  h->capacity=capacity;
  h->compare=compare;
  
  // 힙의 비교함수 저장. min , max를 구분하기위해 쓰임 
  h->arr=(hDATA *)malloc(sizeof(hDATA)*capacity);
  return h;
}

// 힙구조에서 부모가 내려와야할때 호출되는 함수 
void DownHeap(Heap *h, int i)
{
  int l,r,max;
  hDATA t;
  l=hLeftChild(h,i);
  r=hRightChild(h,i);
  
  if(l!=-1&&h->compare(h->arr[l],h->arr[i]))max=l;
  else max=i;
  if(r!=-1&&h->compare(h->arr[r],h->arr[max]))max=r;
  
  // 부모가 내려와야 하여 재귀함 
	if(max!=i)
  {
    t=h->arr[i];
    h->arr[i]=h->arr[max];
    h->arr[max]=t;
    DownHeap(h,max);
  }
}

// 자식이 올라와야할 때 호출되는 함수 
void UpHeap(Heap *h, int i)
{
  hDATA data=h->arr[i];
  int p;
  for(p=hParent(h,i);p!=-1&&h->compare(data,h->arr[p]);p=hParent(h,i))
  {
    h->arr[i]=h->arr[p];
    i=p;
  }
  h->arr[i]=data;
}

// 힙의 첫번째 값을 빼오는 함수. 첫번째 값을 리턴한다음 리프노드에서 하나 가져와 내려가는 작업을 한다. 
hDATA hDelete(Heap *h)
{
  hDATA data={0};
  if(h->count==0)return data;
  data=h->arr[0];
  h->arr[0]=h->arr[h->count-1];
  h->count--;
  DownHeap(h,0);
  return data;
}

// 힙에 삽입하는 함수. 힙의 리프노드에 삽입하고 올리는 작업을 한다. 
int hInsert(Heap *h, hDATA data)
{
  h->arr[h->count++]=data;
  UpHeap(h,h->count-1);
}
