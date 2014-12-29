#include "huffman.h"
#include <stdlib.h>

int cmp(HUFFMAN_TABLE *p,HUFFMAN_TABLE *q)
{
	return p->val > q->val ? 1: p->val < q->val ? -1: 0;
}

int min_heap(Tree *p,Tree *q)
// ������ min_heap�� �����ϱ����� �� �Լ�
{ 
	return GETVAL(p) < GETVAL(q);
}

void get_huffman_count(unsigned char *buffer, int size, HUFFMAN_TABLE *huffman_table)
// ���ڿ��� �󵵼��� huffman �迭�� �����ϴ� �Լ� 
{
	int i;
	
	// �ʱ�ȭ �� �ݺ����� ���� �����Ѵ�. 
	huffman_init(huffman_table);
	for(i=0;i<size;i++)huffman_table[buffer[i]].val++;
}

void get_huffman_code(Tree *node, int depth)
// ���ڸ����� ������ �ڵ带 �����ϴ� �Լ�. tree�� dfs�� �������鼭 ������尡 ��Ÿ���� ���ڿ� ��������� �ڵ带 �����Ѵ�. 
{
	if(node->left || node->right)
	{
		// tree�� ������ 0 , �������� 1�� �ڵ带 �ο��Ѵ�. 
		code[depth] = '0';
		if(node->left)get_huffman_code(node->left,depth+1);
		code[depth] = '1';
		if(node->right)get_huffman_code(node->right,depth+1);
	}
	else if(node)
	{
		// ��������� ���� ��尡 ����Ű�� ���ڿ� �ڵ带 �����Ѵ�. 
		code[depth]=0;
		strcpy(huffman_code[GETNO(node)].code,code);
		huffman_code[GETNO(node)].len = strlen(code);
		
		// �� �˰��򿡼��� ���̸� 16�̸����� ����־ ���� �Ѿ�ٸ� �����÷ο� �޼����� ���. 
		// ��� ���࿡ �����÷ο찡 ���� �ʾƼ� ������ �����Ѵ�. 
		if(huffman_code[GETNO(node)].len > 16)
		printf("[OverFlow!] %d %d\n",GETNO(node),huffman_code[GETNO(node)].len);
	}
}

void get_huffman_tree(HUFFMAN_TABLE *huffman_table)
// ���� �󵵼��� ������ ������ Ʈ���� �����. 
{
	// ���� Ʈ���� �����Ѵ�. 
	Heap *pq = CreateHeap(256,min_heap);
	Tree tree[513];
	HUFFMAN_TABLE t[256];
	Tree *x,*y;
	
	int i,j;
	
	// ������带 ǥ���ϰ� ���� ���ڸ� �����Ѵ�. 
	for(i=0;i<256;i++)
	{
		if(huffman_table[i].val)
		{
			tree[i].val = &huffman_table[i];
			tree[i].left = tree[i].right = 0;
			hInsert(pq,&tree[i]);
		}
	}
	
	
	for(j=0;pq->count > 1;j++,i++)
	{
		// ������ �ΰ��� ���� ���� �� ���� ��ġ�� �ٽ� ���� �ִ´�.
		// �̶� ��ģ ���� �θ��尡 �Ǿ� ���Ӱ� Ʈ���� ���Եȴ�. 
		x = hDelete(pq);
		y = hDelete(pq);
		
		t[j].val = GETVAL(x) + GETVAL(y);
		tree[i].val = &t[j];
		tree[i].left = x;
		tree[i].right = y;
		hInsert(pq,&tree[i]);
	}
	
	// Ʈ���� ������ �ڵ带 ���Ѵ�. 
	get_huffman_code(&tree[i-1],0);
}

int huffman_search(char *code,int len)
// ���� �ڵ�� ���� �ڵ带 �������ִ� ���ڸ� �˻��Ѵ�. 
{
	int i;
	// ���̿� �ڵ带 ������ �񱳸��Ѵ�. �״��� ȿ���� ���� ���ϴ�. 
	for(i=0;i<256;i++)
	{
		if(huffman_code[i].len == len && strncmp(code,huffman_code[i].code,len) == 0) return i;
	}
	
	// �߰��� �ȵǸ� -1�� ���� 
	return -1;
}

int huffman_encode(char *buffer, int size, char *result)
// ������ ���ڵ� �Լ� 
{
	HUFFMAN_TABLE huffman_table[256];
	
	// �󵵼��� Ʈ���� �����Ѵ�. 
	get_huffman_count(buffer,size,huffman_table);
	get_huffman_tree(huffman_table);
	
	
	int i,j;
	int rsize = 0;
	int curLen,curText,curBitPos=0;
	result[rsize]=0;
	
	// ������ ���࿡�� ����� �ʿ��ϴ�. �������� ����� �̿��� ����� �����Ѵ�. 
	for(i=0;i<256;i++)
	{
		// ���̴� 4��Ʈ��, �ڵ�� �������̸� ���� �����Ѵ�. 
		curLen = huffman_code[i].len;
		
		// ���� ����. 
		for(j=3;j>=0;j--)
		{
			// ���� ��ġ�� ���̸� �Ѻ�Ʈ�� �Է��Ѵ�. 
			result[rsize]<<=1;
			result[rsize] += (curLen >> j) & 1;
			
			// ���� ����Ʈ�� 8��Ʈ�� ��� ���������� ���� ����Ʈ�� �̵��Ѵ�. 
			curBitPos++;
			if(curBitPos == 8)
			{ 
				curBitPos=0;
				rsize++;
				result[rsize]=0;
			}
		}
		
		// �ڵ� ����. 
		for(j=0;j<curLen;j++)
		{
			// ���� ��ġ�� �ڵ带 �Ѻ�Ʈ�� �Է��Ѵ�. 
			result[rsize]<<=1;
			result[rsize] += huffman_code[i].code[j] - '0';
			
			// ���� ����Ʈ�� 8��Ʈ�� ��� ���������� ���� ����Ʈ�� �̵��Ѵ�. 
			curBitPos++;
			if(curBitPos == 8)
			{
				curBitPos=0;
				rsize++;
				result[rsize]=0;
			}
		}
	}
	
	// ���� ���ڵ� 
	for(i=0;i<size;i++)
	{
		curText = (unsigned char)buffer[i];
		
		for(j=0; j < huffman_code[curText].len; j++)
		{
			
			// �ش� ���ڿ� ���� �ڵ带 �Ѻ�Ʈ�� ����. 
			result[rsize] <<= 1; 
			result[rsize] += huffman_code[curText].code[j] - '0';
			
			// ���� ����Ʈ�� 8��Ʈ�� ��� ���������� ���� ����Ʈ�� �̵��Ѵ�. 
			curBitPos++;
			if(curBitPos == 8)
			{
				curBitPos=0;
				rsize++;
				result[rsize]=0;
			}
		}
	}
	// ������Ʈ�� �׿���Ʈ�� ���� �Ǽ� �������� ������ �ش�. ���� �� ����Ʈ�� �� ����� �����Ͽ� �̸� �����Ѵ�.  
	if(curBitPos)
	{
		result[rsize+1]=curBitPos;
		rsize+=2;
	}
	
	// �׿���Ʈ�� ������ -1�� ����. 
	else result[rsize++]=-1;
	
	// ���� ũ�� ���� 
	return rsize;
}

int huffman_decode(char *buffer, int size, char *result)
// ������ ���ڵ� �Լ� 
{
	int i,j,k;
	int rsize = 0;
	int curPos=0,curLen,curText,curBitPos=7;
	
	
	// ����� ���� ���̿� �ڵ带 �����´� 
	curText = buffer[curPos];
	for(i=0;i<256;i++)
	{
		curLen = 0;
		
		// ���� �������� 
		for(j=0;j<4;j++)
		{
			// �Ѻ�Ʈ�� ������ 
			curLen <<= 1;
			curLen += (curText >> curBitPos) & 1;
			
			// ����Ʈ�� �����Դٸ� ���� ����Ʈ�� 
			curBitPos--;
			if(curBitPos == -1)
			{
				curBitPos=7;
				curPos++;
				curText = buffer[curPos];
			}
		}
		huffman_code[i].len = curLen;
		
		
		// �ڵ� �������� 
		for(j=0;j<curLen;j++)
		{
			huffman_code[i].code[j] = ((curText >> curBitPos) & 1) + '0';
			
			// ����Ʈ�� �����Դٸ� ���� ����Ʈ��
			curBitPos--;
			if(curBitPos == -1)
			{
				curBitPos=7;
				curPos++;
				curText = buffer[curPos];
			}
		}
	}
	
	// ���� ���ڵ��ϱ� 
	i=0;
	for(;curPos < size-1;)
	{
		// �Ѻ�Ʈ�� �������� ���� �ڵ尡 Ʈ���� �����ϴ��� �˻�. 
		code[i] = ((curText >> curBitPos) & 1) + '0';
		int search = huffman_search(code,i+1);
		
		// �˻������ �ִٸ� �˻��� ���ڸ� �����ϰ� �ڵ带 �ʱ�ȭ 
		if(search != -1)
		{
			result[rsize++] = search;
			i=0;
			code[i+1]=0;
		}
		// ���ٸ� �ڵ带 �ø� 
		else
		{
			i++;
			code[i]=0;
		}
		
		// ����Ʈ�� �����Դٸ� ���� ����Ʈ��
		curBitPos--;
		if(curBitPos == -1)
		{
			curPos++;
			curText = buffer[curPos];
			curBitPos = 7;
			
			//���ڵ��� ó���� ������ ����Ʈ ó�� 
			if(curPos == size-2 && buffer[size-1] != -1)
			{
				curBitPos = buffer[size-1] - 1;
			}
		}
	}
	
	//���ڵ� ũ�� ���� 
	return rsize;
}
