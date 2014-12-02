#include "compress.h"

char *get_encode_name(char *path)
{
  int len = strlen(path);
  char *ret = (char *)malloc(len + 1);
  int i;
  strcpy(ret,path);
	for(i=len-1;ret[i] != '.';i--);
  strcpy(ret+i,".cpd");
  return ret;
}

char *get_decode_name(char *path)
{
  int len = strlen(path);
  char *ret = (char *)malloc(len + 1);
  int i;
  strcpy(ret,path);
	for(i=len-1;ret[i] != '.';i--);
  strcpy(ret+i,".dcpd");
  return ret;
}

int get_size(FILE *fp)
{
	fseek(fp,0,SEEK_SET);
	int res;
	for(res=0;fgetc(fp),!feof(fp);res++);
	fseek(fp,0,SEEK_SET);
	return res;
}
void get_buffer(FILE *fp,char *buffer)
{
	for(;*(buffer++)=fgetc(fp),!feof(fp););
}

void set_buffer(FILE *fp,char *buffer, int size)
{
  fwrite(&size,1,sizeof(int),fp);
  for(;size--;)fputc(*(buffer++),fp);	
}
void compress(char *path)
{
	FILE *fp;
  char *buffer, *result;
  int size,rsize,i;
  
  // get buffer from file
  fp = fopen(path,"rb");
  if(fp == 0){puts("[!] fopen error");return;}
  size = get_size(fp);
  buffer = (char *)malloc(size+0xFFFFFF);
  get_buffer(fp,buffer);
  fclose(fp);
  
  // encode
  result = (char *)malloc(size+0xFFFFFF);
  rsize = LZ77_encode(buffer,size,result);
  printf("LZ77 size %d\n",rsize/1024);
  rsize = huffman_encode(result,rsize,buffer);
  printf("huffman size %d\n",rsize/1024);
  
  // set buffer to file
  printf("%d to %d\n",size,rsize);
  fp = fopen(get_encode_name(path),"wb");
  set_buffer(fp,result,rsize);
  fclose(fp);  
}
