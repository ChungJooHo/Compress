#ifndef COMPRESS_H
#define COMPRESS_H

#include "LZ77.h"
#include "huffman.h"
#include "runlength.h"
#include "substitution.h"
#include <stdio.h>

char *get_encode_name(char *path);
char *get_decode_name(char *path);
int get_size(FILE *fp);
void get_buffer(FILE *fp,char *buffer);
int get_name(char *path);

void set_buffer(FILE *fp,char *buffer, int size);
void compress(char *path, int option);

int encode(char *buffer, int size, char *result, int name);

#endif
