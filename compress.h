#ifndef COMPRESS_H
#define COMPRESS_H

#include "LZ77.h"
#include <stdio.h>

char *get_encode_name(char *path);
char *get_decode_name(char *path);
int get_size(FILE *fp);
void get_buffer(FILE *fp,char *buffer);
void set_buffer(FILE *fp,char *buffer, int size);
void compress(char *path);
void decompress(char *path);

#endif
