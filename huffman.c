#include "huffman.h"
#include <stdlib.h>

int cmp(HUFFMAN_TABLE *p,HUFFMAN_TABLE *q)
{
	return p->val > q->val ? 1: p->val < q->val ? -1: 0;
}

int min_heap(Tree *p,Tree *q)
// 힙에서 min_heap을 구현하기위한 비교 함수
{ 
	return GETVAL(p) < GETVAL(q);
}

void get_huffman_count(unsigned char *buffer, int size, HUFFMAN_TABLE *huffman_table)
// 문자열의 빈도수를 huffman 배열에 저장하는 함수 
{
	int i;
	
	// 초기화 후 반복문을 통해 저장한다. 
	huffman_init(huffman_table);
	for(i=0;i<size;i++)huffman_table[buffer[i]].val++;
}

void get_huffman_code(Tree *node, int depth)
// 문자마다의 허프만 코드를 저장하는 함수. tree를 dfs로 내려가면서 리프노드가 나타내는 문자에 현재까지의 코드를 저장한다. 
{
	if(node->left || node->right)
	{
		// tree의 왼쪽은 0 , 오른쪽은 1의 코드를 부여한다. 
		code[depth] = '0';
		if(node->left)get_huffman_code(node->left,depth+1);
		code[depth] = '1';
		if(node->right)get_huffman_code(node->right,depth+1);
	}
	else if(node)
	{
		// 리프노드라면 현재 노드가 가리키는 문자에 코드를 저장한다. 
		code[depth]=0;
		strcpy(huffman_code[GETNO(node)].code,code);
		huffman_code[GETNO(node)].len = strlen(code);
		
		// 이 알고리즘에서는 길이를 16미만으로 잡고있어서 만약 넘어간다면 오버플로우 메세지를 출력. 
		// 모든 압축에 오버플로우가 뜨지 않아서 가정은 성립한다. 
		if(huffman_code[GETNO(node)].len > 16)
		printf("[OverFlow!] %d %d\n",GETNO(node),huffman_code[GETNO(node)].len);
	}
}

void get_huffman_tree(HUFFMAN_TABLE *huffman_table)
// 구한 빈도수를 가지고 허프만 트리를 만든다. 
{
	// 힙과 트리를 구성한다. 
	Heap *pq = CreateHeap(256,min_heap);
	Tree tree[513];
	HUFFMAN_TABLE t[256];
	Tree *x,*y;
	
	int i,j;
	
	// 리프노드를 표시하고 힙에 인자를 삽입한다. 
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
		// 힙에서 두개를 꺼낸 다음 두 값을 합치고 다시 힙에 넣는다.
		// 이때 합친 값은 부모노드가 되어 새롭게 트리에 삽입된다. 
		x = hDelete(pq);
		y = hDelete(pq);
		
		t[j].val = GETVAL(x) + GETVAL(y);
		tree[i].val = &t[j];
		tree[i].left = x;
		tree[i].right = y;
		hInsert(pq,&tree[i]);
	}
	
	// 트리를 가지고 코드를 구한다. 
	get_huffman_code(&tree[i-1],0);
}

int huffman_search(char *code,int len)
// 현재 코드와 같은 코드를 가지고있는 문자를 검색한다. 
{
	int i;
	// 길이와 코드를 가지고 비교를한다. 그다지 효율이 좋지 못하다. 
	for(i=0;i<256;i++)
	{
		if(huffman_code[i].len == len && strncmp(code,huffman_code[i].code,len) == 0) return i;
	}
	
	// 발견이 안되면 -1을 리턴 
	return -1;
}

int huffman_encode(char *buffer, int size, char *result)
// 허프만 인코딩 함수 
{
	HUFFMAN_TABLE huffman_table[256];
	
	// 빈도수와 트리를 구성한다. 
	get_huffman_count(buffer,size,huffman_table);
	get_huffman_tree(huffman_table);
	
	
	int i,j;
	int rsize = 0;
	int curLen,curText,curBitPos=0;
	result[rsize]=0;
	
	// 허프만 압축에는 헤더가 필요하다. 가변길이 방식을 이용해 헤더를 저장한다. 
	for(i=0;i<256;i++)
	{
		// 길이는 4비트로, 코드는 가변길이를 통해 삽입한다. 
		curLen = huffman_code[i].len;
		
		// 길이 저장. 
		for(j=3;j>=0;j--)
		{
			// 현재 위치에 길이를 한비트씩 입력한다. 
			result[rsize]<<=1;
			result[rsize] += (curLen >> j) & 1;
			
			// 현재 바이트에 8비트를 모두 저장했으면 다음 바이트로 이동한다. 
			curBitPos++;
			if(curBitPos == 8)
			{ 
				curBitPos=0;
				rsize++;
				result[rsize]=0;
			}
		}
		
		// 코드 저장. 
		for(j=0;j<curLen;j++)
		{
			// 현재 위치에 코드를 한비트씩 입력한다. 
			result[rsize]<<=1;
			result[rsize] += huffman_code[i].code[j] - '0';
			
			// 현재 바이트에 8비트를 모두 저장했으면 다음 바이트로 이동한다. 
			curBitPos++;
			if(curBitPos == 8)
			{
				curBitPos=0;
				rsize++;
				result[rsize]=0;
			}
		}
	}
	
	// 본문 인코딩 
	for(i=0;i<size;i++)
	{
		curText = (unsigned char)buffer[i];
		
		for(j=0; j < huffman_code[curText].len; j++)
		{
			
			// 해당 문자에 대한 코드를 한비트씩 삽입. 
			result[rsize] <<= 1; 
			result[rsize] += huffman_code[curText].code[j] - '0';
			
			// 현재 바이트에 8비트를 모두 저장했으면 다음 바이트로 이동한다. 
			curBitPos++;
			if(curBitPos == 8)
			{
				curBitPos=0;
				rsize++;
				result[rsize]=0;
			}
		}
	}
	// 끝바이트에 잉여비트가 남게 되서 압축결과에 지장을 준다. 따라서 끝 바이트에 그 결과를 저장하여 이를 방지한다.  
	if(curBitPos)
	{
		result[rsize+1]=curBitPos;
		rsize+=2;
	}
	
	// 잉여비트가 없으면 -1을 저장. 
	else result[rsize++]=-1;
	
	// 압축 크기 리턴 
	return rsize;
}

int huffman_decode(char *buffer, int size, char *result)
// 허프만 디코딩 함수 
{
	int i,j,k;
	int rsize = 0;
	int curPos=0,curLen,curText,curBitPos=7;
	
	
	// 헤더를 통해 길이와 코드를 가져온다 
	curText = buffer[curPos];
	for(i=0;i<256;i++)
	{
		curLen = 0;
		
		// 길이 가져오기 
		for(j=0;j<4;j++)
		{
			// 한비트씩 가져옴 
			curLen <<= 1;
			curLen += (curText >> curBitPos) & 1;
			
			// 모든비트를 가져왔다면 다음 바이트로 
			curBitPos--;
			if(curBitPos == -1)
			{
				curBitPos=7;
				curPos++;
				curText = buffer[curPos];
			}
		}
		huffman_code[i].len = curLen;
		
		
		// 코드 가져오기 
		for(j=0;j<curLen;j++)
		{
			huffman_code[i].code[j] = ((curText >> curBitPos) & 1) + '0';
			
			// 모든비트를 가져왔다면 다음 바이트로
			curBitPos--;
			if(curBitPos == -1)
			{
				curBitPos=7;
				curPos++;
				curText = buffer[curPos];
			}
		}
	}
	
	// 본문 디코딩하기 
	i=0;
	for(;curPos < size-1;)
	{
		// 한비트씩 읽은다음 현재 코드가 트리에 존재하는지 검색. 
		code[i] = ((curText >> curBitPos) & 1) + '0';
		int search = huffman_search(code,i+1);
		
		// 검색결과가 있다면 검색된 문자를 저장하고 코드를 초기화 
		if(search != -1)
		{
			result[rsize++] = search;
			i=0;
			code[i+1]=0;
		}
		// 없다면 코드를 늘림 
		else
		{
			i++;
			code[i]=0;
		}
		
		// 모든비트를 가져왔다면 다음 바이트로
		curBitPos--;
		if(curBitPos == -1)
		{
			curPos++;
			curText = buffer[curPos];
			curBitPos = 7;
			
			//인코딩시 처리한 마지막 바이트 처리 
			if(curPos == size-2 && buffer[size-1] != -1)
			{
				curBitPos = buffer[size-1] - 1;
			}
		}
	}
	
	//디코딩 크기 리턴 
	return rsize;
}
