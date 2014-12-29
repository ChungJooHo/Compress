#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "heap.h"
#include "tree.h"
#include "assemble.h"

#define GETVAL(p) (((HUFFMAN_TABLE *)(p->val))->val)
#define GETNO(p) (((HUFFMAN_TABLE *)(p->val))->no)

typedef struct HUFFMAN_TABLE {
	int no, val;
} HUFFMAN_TABLE;

typedef struct HUFFMAN_CODE {
	char code[32];
	int len;
} HUFFMAN_CODE;

char code[32];
HUFFMAN_CODE huffman_code[256];

int huffman_search(char *code, int len);
int huffman_encode(char *buffer, int size, char *result);
int huffman_decode(char *buffer, int size, char *result);
void get_huffman_count(unsigned char *buffer, int size, HUFFMAN_TABLE *huffman_table);
void get_huffman_tree(HUFFMAN_TABLE *huffman_table);
void get_huffman_code(Tree *node, int depth);
#endif
