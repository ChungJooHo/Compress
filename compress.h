#ifndef COMPRESS_H
#define COMPRESS_H

#include "LZSS.h"
#include "huffman.h"
#include "runlength.h"
#include "substitution.h"
#include <stdio.h>

char *get_encode_name(char *path);
char *get_decode_name(char *path);
void get_buffer(FILE *fp,char *buffer);
int get_name(char *path);
void set_buffer(FILE *fp,char *buffer, int size);
void buffer_cpy(char *buffer, int size, char *result);

void compress(char *path, int option);

int encode(char *buffer, int size, char *result, int name);
int decode(char *buffer, int size, char *result, int name);
#endif
