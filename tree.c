#include "tree.h"

// 트리구조를 할당하는 함수 
Tree *CreateTree(void *data)
{
	Tree *t = (Tree *)malloc(sizeof(Tree));
	t->val = data;
	t->left = t->right = 0;
	return t;
}
