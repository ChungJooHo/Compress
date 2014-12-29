#include "assemble.h"
#include <stdio.h>

typedef struct HUFFMAN_TABLE {
	int no, val;
} HUFFMAN_TABLE;


void huffman_init(HUFFMAN_TABLE *huffman_table)
// huffman 배열 초기화 함수. 각 문자의 index를 저장. 
{
	int i;
	for(i=0;i<256;i++)
	{
		huffman_table[i].no = i;
		huffman_table[i].val = 0;
	}
}
// 파일을 전부 검사하여 길이를 읽어오는 함수. 
int get_size(FILE *fp)
{
	fseek(fp,0,SEEK_SET);
	int res;
	if(feof(fp))return 0;
	
	// eof가 뜰때까지 길이를 검사 
	for(res=0;fgetc(fp),!feof(fp);res++);
	
	// 파일 위치를 처음으로 돌려놓는다 
	fseek(fp,0,SEEK_SET);
	
	//길이 리턴 
	return res;
}

int return_int(char c)		//인자로 받은 문자에 따라 정수로 리턴
{
	if (c == 'a')				//a=0
		return 0;
	else if (c == 't')			//t=1
		return 1;
	else if (c == 'g')			//g=2
		return 2;
	else if (c == 'c')			//c=3
		return 3;
	return -1;					//위에 해당하지 않을때 -1 리턴
}

