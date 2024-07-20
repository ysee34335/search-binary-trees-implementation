#ifndef RBTREE_H
#define RBTREE_H

typedef enum { RED, BLACK } RBNodeColor;

typedef struct RBNode {
    int key;
    RBNodeColor color;
    struct RBNode *left, *right, *parent;
} RBNode;

typedef struct RBTree {
    RBNode *root;
    RBNode *nil;  // Nó sentinela NIL
} RBTree;

// Funções para manipulação da Red-Black Tree
RBTree* rbtree_create();
void rbtree_insert(RBTree* tree, int key);
void rbtree_delete(RBTree* tree, int key);
void rbtree_free(RBTree* tree);
void rbtree_print_in_order(RBTree* tree);

#endif // RBTREE_H
