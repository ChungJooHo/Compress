#ifndef LZSS_H
#define LZSS_H

#include "huffman.h"
#include <stdio.h>
#include <stdlib.h>

int min(int a,int b);
int max(int a,int b);

int bcmp(const char *p,const char *q);
int LZSS_encode(unsigned char *buffer, int size, unsigned char *result);
int LZSS_decode(char *buffer, int size, char *result);
#endif
