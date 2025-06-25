#ifndef AVLTREE_H
#define AVLTREE_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define __MAX(a,b) (((a)>(b))?(a):(b))

#define _update_parent(a,b) { struct avlnode *parent = **pstack; if(pstack == stack) **pstack = (b); else if(parent->left == (a)) parent->left = (b); else parent->right = (b); }
#define _get_key(a) *(int*) (((char*) *(a)) + sizeof(struct avlnode)) /// return type must be changed according to key type
#define _get_value(a) *(int*) (((char*) *(a)) + sizeof(struct avlnode) + sizeof(int)) /// return type must be changed according to key & value type

struct avlnode {
    struct avlnode *left;
    struct avlnode *right;
    int balance;
};

extern void avl_reset_stack();
extern struct avlnode* avl_Del_Leaf();
extern struct avlnode* avl_Del_In_Order();
extern void avl_Insert(struct avlnode**, void*, int (*) (const void*, const void*));
extern void avl_Del_Node(void*, int (*) (const void*, const void*));
extern void avl_PrintTreeVisual(struct avlnode**, int);//, int (*) (struct avlnode*));

#endif // AVLTREE_H



