#include "avl_tree.h"

struct avlnode **stack[32]; /// a good hack!!
struct avlnode ***pstack = stack;

void avl_reset_stack() {
    pstack = stack;
}

static inline int _abs(int n) {
    int mask = n >> ((sizeof(int)*8) -1);
    return (mask + n)^mask;
}


static inline void avl_copy(struct avlnode *src, struct avlnode *dst) {

    int key = *(int*) (((char*) (src)) + sizeof(struct avlnode));
    int val = *(int*) (((char*) (src)) + sizeof(struct avlnode) + sizeof(int));

    int *p_dest = (int*) (((char*) (dst)) + sizeof(struct avlnode));

    *p_dest++ = key;
    *p_dest = val;
}


void avl_PrintTreeVisual(struct avlnode **index, int depth) {

    if (!*index) return;

    // Right child first (so tree is upright)
    avl_PrintTreeVisual(&(*index)->right, depth + 1);

    // Print current node with indentation
    for (int i = 0; i < depth; i++) printf("    ");
    printf("[%d %d]\n", _get_key(index) , (*index)->balance);
    if(_abs((*index)->balance) > 1)
        printf("-------------------------Imbalance Occurred!-----------------------------\n");

    // Then left child
    avl_PrintTreeVisual(&(*index)->left, depth + 1);
}


static inline struct avlnode* rRight(struct avlnode *node) { ///Single Rotate Right


    struct avlnode *left_child = node->left;
    struct avlnode *parent = **--pstack;

    if(pstack == stack) {
        **pstack = node->left;
    }else if(parent->left == node)
        parent->left = node->left;
    else
        parent->right = node->left;


    node->left = left_child->right;


    left_child->right = node;

    if (left_child->balance == -1) {
        node->balance = 0;
        left_child->balance = 0;
    } else {
        node->balance = -1;
        left_child->balance = 1;
    }

    return left_child;
}

static inline struct avlnode* rRRight(struct avlnode *node) {

    struct avlnode *parent = **--pstack;
    struct avlnode *left = node->left;
    struct avlnode *lr = left->right;

    left->right = lr->left;
    node->left = lr->right;
    lr->left = left;
    lr->right = node;

    if(pstack == stack)
        **pstack = lr;
    else if(parent->left == node)
        parent->left = lr;
    else
        parent->right = lr;


    if (lr->balance == -1) {
        node->balance = 1;
        left->balance = 0;
    } else if (lr->balance == 1) {
        node->balance = 0;
        left->balance = -1;
    } else {
        node->balance = 0;
        left->balance = 0;
    }
    lr->balance = 0;

    return lr;
}


static inline struct avlnode* rLeft(struct avlnode *node) { ///Single Rotate Left

    struct avlnode *right_child = node->right;
    struct avlnode *parent = **--pstack;

    if(pstack == stack)
        **pstack = node->right;
    else if(parent->left == node)
        parent->left = node->right;
    else
        parent->right = node->right;

    node->right = right_child->left;


    right_child->left = node;

    if (right_child->balance == 1) {
        node->balance = 0;
        right_child->balance = 0;
    } else {
        node->balance = 1;
        right_child->balance = -1;
    }

    return right_child;
}



static inline struct avlnode* rLLeft (struct avlnode *node) {

    struct avlnode *parent = **--pstack;
    struct avlnode *right = node->right;
    struct avlnode *rl = right->left;

    right->left = rl->right;
    node->right = rl->left;
    rl->right = right;
    rl->left = node;

    if(pstack == stack)
        **pstack = rl;
    else if(parent->left == node)
        parent->left = rl;
    else
        parent->right = rl;


    if (rl->balance == 1) {
        node->balance = -1;
        right->balance = 0;
    } else if (rl->balance == -1) {
        node->balance = 0;
        right->balance = 1;
    } else {
        node->balance = 0;
        right->balance = 0;
    }
    rl->balance = 0;

    return rl;

}


struct avlnode* fix_tree(struct avlnode *parent) {

    if (parent->balance < 0) {  // Left heavy
        if ((parent->left)->balance <= 0)
            parent = rRight(parent); // single right
        else
            parent = rRRight(parent);
    }else { // Right heavy
        if ((parent->right)->balance >= 0)
            parent = rLeft(parent); // single left
         else
            parent = rLLeft(parent);
    }

    return parent;
}

struct avlnode* avl_Del_In_Order() {

    struct avlnode **node = *pstack;

    if(pstack == stack)
        *++pstack = *stack;

    while(*node) {

        node = &(*node)->left;

        if(*node)
            *++pstack = node;

    }

    node = *pstack--;

    struct avlnode ***save = pstack;

    if(pstack == stack && (*node)->right) {
        _update_parent(*node, (*node)->right)
        return **++save;
    }

    struct avlnode *_node = *node;


    while(pstack != stack) {

        struct avlnode *parent = **pstack;

        if(parent->left == _node)
            parent->balance++;
        else
            parent->balance--;

        if(_node == *node) {

            if(_node->right)
                _update_parent(*node, (*node)->right)
            else
                _update_parent(*node, NULL)

            free(*node);
        }


        int balance = parent->balance;


        if(_abs(balance) == 1)
            break;
        else if(!balance) {
            _node = parent;
            --pstack;
            continue;
        }

        _node = fix_tree(parent);

        if(_node->balance)
            break;

    }

    return (save == stack) ? NULL : **save;
}

struct avlnode* avl_Del_Leaf() {

    struct avlnode **node = *pstack;

    if(pstack == stack)
        *++pstack = *stack;

    while(*node) {

        node = (*node)->left ? &(*node)->left : &(*node)->right;

        if(*node)
            *++pstack = node;
    }

    node = *pstack--;

    struct avlnode ***save = pstack;
    struct avlnode *_node = *node;

    while(pstack != stack) {

        struct avlnode *parent = **pstack;

        if(parent->left == _node)
            parent->balance++;
        else
            parent->balance--;

        if(_node == *node) {
            _update_parent(*node, NULL)
            free(*node);
        }


        int balance = parent->balance;


        if(_abs(balance) == 1)
            break;
        else if(!balance) {
            _node = parent;
            --pstack;
            continue;
        }


        _node = fix_tree(parent);

        if(_node->balance)
            break;

    }

    return (save == stack) ? NULL : **save;
}


void avl_Del_Node(void *data, int (*_func) (const void *a, const void *b)) {

    int _cmp = 0;
    pstack = stack;
    struct avlnode **node = *pstack;

    while(*node) {

        _cmp = _func(data, *node);
        *++pstack = node;

        if(!_cmp)
            break;

        if(_cmp < 0)
            node = &(*node)->left;
        else
            node = &(*node)->right;

    }

    if(!*node)
        return;

    node = *pstack;

    //printf("[node %d %d] \n", *(int*) (((char*) *node) + sizeof(struct avlnode)), (int) (pstack - stack));

    struct avlnode ***save = pstack;

    node = &(*node)->left;

    while(*node) {
        *++pstack = node;
        node = &(*node)->right;
    }

    node = *pstack;

    if(pstack > save)  // node has Left Child
        avl_copy(*node, **save);
    else if ((*node)->right) { // node has only Right Child
        node = &(*node)->right;
        avl_copy(*node, **save);
        *++pstack = node;
    }

    if(--pstack == stack) {
        _update_parent(*node, NULL)
        return;
    }

    struct avlnode *_node = *node;

    while(pstack != stack) {

        struct avlnode *parent = **pstack;

        if(parent->left == _node)
            parent->balance++;
        else
            parent->balance--;

        if(_node == *node) {
            _update_parent(_node, _node->left)
            free(_node);
        }

        int balance = parent->balance;

        if(_abs(balance) == 1)
            return;
        else if(!balance) {
            _node = parent;
            --pstack;
            continue;
        }

        _node = fix_tree(parent);

        if(_node->balance)
            return;
    }

}


void avl_Insert(struct avlnode **tree, void* data, int (*_func) (const void *a, const void *b)) {

    int _cmp = 0;
    pstack = stack;
    struct avlnode **temp_it = tree;
    *pstack = tree;

    while(*temp_it) {

        *++pstack = temp_it;
        _cmp = _func(data, *temp_it);

        if(_cmp < 0) {

            temp_it = &(*temp_it)->left;

        }else if(_cmp > 0) {

            temp_it = &(*temp_it)->right;

        }else{
            free(data);
            return;
        }
    }

    *temp_it = (struct avlnode*) data;
    (*temp_it)->left = (*temp_it)->right = NULL;
    (*temp_it)->balance = 0;

    struct avlnode *node = *temp_it;

    while(pstack != stack){

        struct avlnode *parent = **pstack;

        if(parent->left == node)
            parent->balance--;
        else
            parent->balance++;

        int balance = parent->balance;

        if(!balance)
            return;
        else if(_abs(balance) < 2) {
            node = parent;
            pstack--;
            continue;
        }

        fix_tree(parent);
        return;
    }

}




