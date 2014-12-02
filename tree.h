#ifndef TREE_H
#define TREE_H

typedef struct TREE {
	void *val;
	struct TREE *left,*right;
} Tree;

Tree *CreateTree(void *data);

#endif
