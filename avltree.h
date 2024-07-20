#ifndef AVLTREE_H
#define AVLTREE_H

typedef struct AVLNode {
    int key;
    struct AVLNode *left;
    struct AVLNode *right;
    int height;
} AVLNode;

typedef struct AVLTree {
    AVLNode *root;
} AVLTree;

// Funções para manipulação da AVL Tree
AVLTree* avl_create();
void avl_insert(AVLTree* tree, int key);
AVLNode* avl_search(AVLTree* tree, int key);
void avl_delete(AVLTree* tree, int key);
void avl_free(AVLTree* tree);
void avl_print_in_order(AVLTree* tree);

#endif // AVLTREE_H
