#ifndef LZ77_H
#define LZ77_H

#include "huffman.h"
#include <stdio.h>
#include <stdlib.h>
typedef struct kmp_ret{int m,ret;}kmp_ret;
int min(int a,int b);
int max(int a,int b);
void make_pi(char *p,int len);
kmp_ret kmp(char *p,int len,char *q,int pi_len);

int bcmp(const char *p,const char *q,int len);
int LZ77_encode(char *buffer, int size, char *result);

#endif
