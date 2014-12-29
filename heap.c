#include "heap.h"

// ���� �θ� ã�� �Լ� 
int hParent(Heap *h, int i)
{
  if(i<=0||i>=h->count)return -1;
  return (i-1)/2;
}

// ���� ���� �ڽ��� ã�� �Լ� 
int hLeftChild(Heap *h, int i)
{
  int t=2*i+1;
  if(t>=h->count)return -1;
  return t;
}

// ���� ������ �ڽ��� ã�� �Լ� 
int hRightChild(Heap *h, int i)
{
  int t=2*i+2;
  if(t>=h->count)return -1;
  return t;
}

// ���� �����ϴ� �Լ� 
Heap *CreateHeap(int capacity, int (*compare)(hDATA p,hDATA q))
{
  Heap *h=(Heap *)malloc(sizeof(Heap));
  
  // ���� �ʱ�ȭ���� 
  h->count=0;
  h->capacity=capacity;
  h->compare=compare;
  
  // ���� ���Լ� ����. min , max�� �����ϱ����� ���� 
  h->arr=(hDATA *)malloc(sizeof(hDATA)*capacity);
  return h;
}

// ���������� �θ� �����;��Ҷ� ȣ��Ǵ� �Լ� 
void DownHeap(Heap *h, int i)
{
  int l,r,max;
  hDATA t;
  l=hLeftChild(h,i);
  r=hRightChild(h,i);
  
  if(l!=-1&&h->compare(h->arr[l],h->arr[i]))max=l;
  else max=i;
  if(r!=-1&&h->compare(h->arr[r],h->arr[max]))max=r;
  
  // �θ� �����;� �Ͽ� ����� 
	if(max!=i)
  {
    t=h->arr[i];
    h->arr[i]=h->arr[max];
    h->arr[max]=t;
    DownHeap(h,max);
  }
}

// �ڽ��� �ö�;��� �� ȣ��Ǵ� �Լ� 
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

// ���� ù��° ���� ������ �Լ�. ù��° ���� �����Ѵ��� ������忡�� �ϳ� ������ �������� �۾��� �Ѵ�. 
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

// ���� �����ϴ� �Լ�. ���� ������忡 �����ϰ� �ø��� �۾��� �Ѵ�. 
int hInsert(Heap *h, hDATA data)
{
  h->arr[h->count++]=data;
  UpHeap(h,h->count-1);
}
