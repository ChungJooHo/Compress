#include "LZ77.h"

#define WINDOW_SIZE 65535
#define MAX_LEN 259
#define MIN_MATCH 3

int min(int a,int b){return a>b?b:a;}
int max(int a,int b){return a>b?a:b;}

int pi[MAX_LEN+1];

void make_pi(char *p,int len)
{
  int begin=1,matched=0;
  pi[0]=0;
  for(;begin+matched<len;)
  {
    if(p[begin+matched]==p[matched])
    {
      matched++;
      pi[begin+matched-1]=matched;
    }
    else
    {
      if(matched == 0) begin++;
      else 
      {
        begin+=matched - pi[matched - 1];
        matched = pi[matched - 1];
      }
    }
  }
}
kmp_ret kmp(char *p,int len,char *q,int pi_len)
{
  int begin,matched;
  kmp_ret ret;
  ret.m=ret.ret=-1;
  
  make_pi(q,pi_len);
  
  for(begin=matched=0;begin+matched<len;)
  {
    if(p[begin+matched]==q[matched])matched++;
    else
    {
      if(matched == 0)begin++;
      else
      {
        begin+=matched-pi[matched-1];
        matched=pi[matched-1];
      }
    }
    if(ret.m < matched)
    {
      ret.m=matched;
      ret.ret = begin;
    }
    if(matched==pi_len)return ret;
  }
  return ret;
}


int bcmp(const char *p,const char *q,int len)
{
  while(len--)
  {
    if(*p != *q)return 0;
    p++;
    q++;
  }
  return 1;
}

int LZ77_encode(char *buffer, int size, char *result)
{
	puts("LZ77_encode Ready");
	int i;
  int rsize = 1;
  char *winPos = buffer;
  int keyCount;
  char *keyPos = result;
  kmp_ret ret;
  int len, pos;
  
  int loading = 0;
  int scale = size / 100;
  keyCount = 0;
  
  for(i=0;i<size;)
  {
  	if(i > WINDOW_SIZE) winPos = buffer + i - WINDOW_SIZE;
    ret = kmp(winPos,min(i,WINDOW_SIZE),buffer+i,min(MAX_LEN,size-i));
    pos = ret.ret;
    len = ret.m;
          
    if(len > MIN_MATCH)
    {
      *keyPos |= 1 << keyCount;
      *(result+rsize)=len-MIN_MATCH-1;
      *(short *)(result+rsize+1)=pos;
      rsize+=MIN_MATCH;
      i+=len;
    }
    else
    {
      *(result+rsize)=buffer[i];
      rsize++;
      i++;
    }
    keyCount++;
    if(keyCount == 8)
    {
      keyCount = 0;
      keyPos = result + rsize;
      rsize++;
    }
    
    loading++;
    if(loading == scale)
    {
      loading = 0;
      printf("Loading ... %2d%\n",i / scale);
    }
  }
  puts("LZ77_encode ok");
  return rsize;
}
void LZ77_decode(char *path)
{
}
