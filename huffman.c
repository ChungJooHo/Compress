#include "huffman.h"
#include <stdlib.h>

int cmp(HUFFMAN_TABLE *p,HUFFMAN_TABLE *q)
{
	return p->val > q->val ? 1: p->val < q->val ? -1: 0;
}

int min_heap(Tree *p,Tree *q)
{
	return GETVAL(p) < GETVAL(q);
}

void huffman_init(HUFFMAN_TABLE *huffman_table)
{
	int i;
	for(i=0;i<256;i++)
	{
		huffman_table[i].no = i;
		huffman_table[i].val = 0;
	}
}

void get_huffman_count(unsigned char *buffer, int size, HUFFMAN_TABLE *huffman_table)
{
	int i;
	huffman_init(huffman_table);
	for(i=0;i<size;i++)huffman_table[buffer[i]].val++;
}

void get_huffman_tree(HUFFMAN_TABLE *huffman_table)
{
	puts("huffman_ready");
	Heap *pq = CreateHeap(256,min_heap);
	Tree tree[513];
	HUFFMAN_TABLE t[256];
	Tree *x,*y;
	
	int i,j;
	for(i=0;i<256;i++)
	{
		if(huffman_table[i].val)
		{
			tree[i].val = &huffman_table[i];
			tree[i].left = tree[i].right = 0;
			hInsert(pq,&tree[i]);
			printf("%d %d\n",huffman_table[i].no, huffman_table[i].val);
		}
	}
	for(j=0;pq->count > 1;j++,i++)
	{
		x = hDelete(pq);
		y = hDelete(pq);
		
		t[j].val = GETVAL(x) + GETVAL(y);
		tree[i].val = &t[j];
		tree[i].left = x;
		tree[i].right = y;
		hInsert(pq,&tree[i]);
	}
	get_huffman_code(&tree[i-1],0);
	
	puts("huffman_ok");
}


int huffman_encode(char *buffer, int size, char *result)
{
	HUFFMAN_TABLE huffman_table[256];
	get_huffman_count(buffer,size,huffman_table);
	get_huffman_tree(huffman_table);
	int i,j = 0;
	int rsize = 0, curText,curBitPos=0;
	result[rsize]=0;
	for(i=0;i<size;i++)
	{
		curText = (unsigned char)buffer[i];
		for(j=0; j < huffman_code[curText].len; j++)
		{
			result[rsize] <<= 1; 
			result[rsize] += huffman_code[curText].code[j] - '0';
			curBitPos++;
			if(curBitPos == 8)
			{
				curBitPos=0;
				rsize++;
				result[rsize]=0;
			}
		}
	}
	return rsize;
}

void get_huffman_code(Tree *node, int depth)
{
	if(node->left || node->right)
	{
		code[depth] = '0';
		if(node->left)get_huffman_code(node->left,depth+1);
		code[depth] = '1';
		if(node->right)get_huffman_code(node->right,depth+1);
	}
	else if(node)
	{
		code[depth]=0;
		strcpy(huffman_code[GETNO(node)].code,code);
		huffman_code[GETNO(node)].len = strlen(code);
		printf("%d %d\n",GETNO(node),huffman_code[GETNO(node)].len);
	}
}
