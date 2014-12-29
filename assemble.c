#include "assemble.h"
#include <stdio.h>

typedef struct HUFFMAN_TABLE {
	int no, val;
} HUFFMAN_TABLE;


void huffman_init(HUFFMAN_TABLE *huffman_table)
// huffman �迭 �ʱ�ȭ �Լ�. �� ������ index�� ����. 
{
	int i;
	for(i=0;i<256;i++)
	{
		huffman_table[i].no = i;
		huffman_table[i].val = 0;
	}
}
// ������ ���� �˻��Ͽ� ���̸� �о���� �Լ�. 
int get_size(FILE *fp)
{
	fseek(fp,0,SEEK_SET);
	int res;
	if(feof(fp))return 0;
	
	// eof�� �㶧���� ���̸� �˻� 
	for(res=0;fgetc(fp),!feof(fp);res++);
	
	// ���� ��ġ�� ó������ �������´� 
	fseek(fp,0,SEEK_SET);
	
	//���� ���� 
	return res;
}

int return_int(char c)		//���ڷ� ���� ���ڿ� ���� ������ ����
{
	if (c == 'a')				//a=0
		return 0;
	else if (c == 't')			//t=1
		return 1;
	else if (c == 'g')			//g=2
		return 2;
	else if (c == 'c')			//c=3
		return 3;
	return -1;					//���� �ش����� ������ -1 ����
}

