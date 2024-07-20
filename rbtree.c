#include <stdio.h>
#include <stdlib.h>
#include "rbtree.h"

// Função auxiliar para criar um novo nó
static RBNode* rbtree_create_node(RBTree* tree, int key, RBNodeColor color) {
    RBNode* node = (RBNode*)malloc(sizeof(RBNode));
    node->key = key;
    node->color = color;
    node->left = tree->nil;
    node->right = tree->nil;
    node->parent = tree->nil;
    return node;
}

// Função pública para criar uma árvore Red-Black
RBTree* rbtree_create() {
    RBTree* tree = (RBTree*)malloc(sizeof(RBTree));
    tree->nil = (RBNode*)malloc(sizeof(RBNode));
    tree->nil->color = BLACK;
    tree->root = tree->nil;
    return tree;
}

// Função auxiliar para rotação à esquerda
static void rbtree_left_rotate(RBTree* tree, RBNode* x) {
    RBNode* y = x->right;
    x->right = y->left;
    if (y->left != tree->nil) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == tree->nil) {
        tree->root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

// Função auxiliar para rotação à direita
static void rbtree_right_rotate(RBTree* tree, RBNode* y) {
    RBNode* x = y->left;
    y->left = x->right;
    if (x->right != tree->nil) {
        x->right->parent = y;
    }
    x->parent = y->parent;
    if (y->parent == tree->nil) {
        tree->root = x;
    } else if (y == y->parent->right) {
        y->parent->right = x;
    } else {
        y->parent->left = x;
    }
    x->right = y;
    y->parent = x;
}

// Função auxiliar para corrigir a inserção
static void rbtree_insert_fixup(RBTree* tree, RBNode* z) {
    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            RBNode* y = z->parent->parent->right;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    rbtree_left_rotate(tree, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rbtree_right_rotate(tree, z->parent->parent);
            }
        } else {
            RBNode* y = z->parent->parent->left;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rbtree_right_rotate(tree, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rbtree_left_rotate(tree, z->parent->parent);
            }
        }
    }
    tree->root->color = BLACK;
}

// Função pública para inserir um nó na árvore Red-Black
void rbtree_insert(RBTree* tree, int key) {
    RBNode* z = rbtree_create_node(tree, key, RED);
    RBNode* y = tree->nil;
    RBNode* x = tree->root;

    while (x != tree->nil) {
        y = x;
        if (z->key < x->key) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
    z->parent = y;
    if (y == tree->nil) {
        tree->root = z;
    } else if (z->key < y->key) {
        y->left = z;
    } else {
        y->right = z;
    }

    z->left = tree->nil;
    z->right = tree->nil;
    z->color = RED;

    rbtree_insert_fixup(tree, z);
}

// Função auxiliar para corrigir a remoção
static void rbtree_delete_fixup(RBTree* tree, RBNode* x) {
    while (x != tree->root && x->color == BLACK) {
        if (x == x->parent->left) {
            RBNode* w = x->parent->right;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rbtree_left_rotate(tree, x->parent);
                w = x->parent->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    rbtree_right_rotate(tree, w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                rbtree_left_rotate(tree, x->parent);
                x = tree->root;
            }
        } else {
            RBNode* w = x->parent->left;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rbtree_right_rotate(tree, x->parent);
                w = x->parent->left;
            }
            if (w->right->color == BLACK && w->left->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    rbtree_left_rotate(tree, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                rbtree_right_rotate(tree, x->parent);
                x = tree->root;
            }
        }
    }
    x->color = BLACK;
}

// Função auxiliar para substituir um nó por outro
static void rbtree_transplant(RBTree* tree, RBNode* u, RBNode* v) {
    if (u->parent == tree->nil) {
        tree->root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    v->parent = u->parent;
}

// Função auxiliar para remover um nó
static void rbtree_delete_node(RBTree* tree, RBNode* z) {
    RBNode* y = z;
    RBNode* x;
    RBNodeColor y_original_color = y->color;

    if (z->left == tree->nil) {
        x = z->right;
        rbtree_transplant(tree, z, z->right);
    } else if (z->right == tree->nil) {
        x = z->left;
        rbtree_transplant(tree, z, z->left);
    } else {
        y = z->right;
        while (y->left != tree->nil) {
            y = y->left;
        }
        y_original_color = y->color;
        x = y->right;
        if (y->parent == z) {
            x->parent = y;
        } else {
            rbtree_transplant(tree, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        rbtree_transplant(tree, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    free(z);

    if (y_original_color == BLACK) {
        rbtree_delete_fixup(tree, x);
    }
}

// Função pública para remover um nó na árvore Red-Black
void rbtree_delete(RBTree* tree, int key) {
    RBNode* z = tree->root;
    while (z != tree->nil) {
        if (key == z->key) {
            rbtree_delete_node(tree, z);
            return;
        } else if (key < z->key) {
            z = z->left;
        } else {
            z = z->right;
        }
    }
}

// Função auxiliar para desalocar os nós da árvore
static void rbtree_free_node(RBTree* tree, RBNode* node) {
    if (node != tree->nil) {
        rbtree_free_node(tree, node->left);
        rbtree_free_node(tree, node->right);
        free(node);
    }
}

// Função pública para desalocar a árvore Red-Black
void rbtree_free(RBTree* tree) {
    if (tree == NULL) return;
    rbtree_free_node(tree, tree->root);
    free(tree->nil);
    free(tree);
}

// Função auxiliar para impressão em ordem
static void rbtree_print_in_order_node(RBTree* tree, RBNode* node) {
    if (node != tree->nil) {
        rbtree_print_in_order_node(tree, node->left);
        printf("%d ", node->key);
        rbtree_print_in_order_node(tree, node->right);
    }
}

// Função pública para impressão em ordem
void rbtree_print_in_order(RBTree* tree) {
    if (tree == NULL) return;
    rbtree_print_in_order_node(tree, tree->root);
    printf("\n");
}
