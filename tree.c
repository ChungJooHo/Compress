#include "tree.h"

Tree *CreateTree(void *data)
{
	Tree *t = (Tree *)malloc(sizeof(Tree));
	t->val = data;
	t->left = t->right = 0;
	return t;
}
