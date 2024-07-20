#include <stdio.h>
#include <stdlib.h>
#include "avltree.h"

// Função auxiliar para obter a altura do nó
static int avl_height(AVLNode* node) {
    if (node == NULL)
        return 0;
    return node->height;
}

// Função auxiliar para obter o máximo entre dois inteiros
static int avl_max(int a, int b) {
    return (a > b) ? a : b;
}

// Função auxiliar para criar um novo nó
static AVLNode* avl_create_node(int key) {
    AVLNode* node = (AVLNode*)malloc(sizeof(AVLNode));
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    node->height = 1; // Novo nó é inicialmente adicionado na folha
    return node;
}

// Função auxiliar para rotacionar para a direita a subárvore com raiz y
static AVLNode* avl_right_rotate(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;

    // Realiza a rotação
    x->right = y;
    y->left = T2;

    // Atualiza as alturas
    y->height = avl_max(avl_height(y->left), avl_height(y->right)) + 1;
    x->height = avl_max(avl_height(x->left), avl_height(x->right)) + 1;

    // Retorna a nova raiz
    return x;
}

// Função auxiliar para rotacionar para a esquerda a subárvore com raiz x
static AVLNode* avl_left_rotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;

    // Realiza a rotação
    y->left = x;
    x->right = T2;

    // Atualiza as alturas
    x->height = avl_max(avl_height(x->left), avl_height(x->right)) + 1;
    y->height = avl_max(avl_height(y->left), avl_height(y->right)) + 1;

    // Retorna a nova raiz
    return y;
}

// Função auxiliar para obter o fator de balanceamento do nó
static int avl_get_balance(AVLNode* node) {
    if (node == NULL)
        return 0;
    return avl_height(node->left) - avl_height(node->right);
}

// Função auxiliar para inserir um nó na subárvore com raiz node e retorna a nova raiz da subárvore
static AVLNode* avl_insert_node(AVLNode* node, int key) {
    if (node == NULL)
        return avl_create_node(key);

    if (key < node->key)
        node->left = avl_insert_node(node->left, key);
    else if (key > node->key)
        node->right = avl_insert_node(node->right, key);
    else // Chaves iguais não são permitidas na BST
        return node;

    // Atualiza a altura deste nó ancestral
    node->height = 1 + avl_max(avl_height(node->left), avl_height(node->right));

    // Obtém o fator de balanceamento deste nó ancestral para verificar se este nó ficou desbalanceado
    int balance = avl_get_balance(node);

    // Se este nó ficou desbalanceado, existem 4 casos

    // Caso Esquerda-Esquerda
    if (balance > 1 && key < node->left->key)
        return avl_right_rotate(node);

    // Caso Direita-Direita
    if (balance < -1 && key > node->right->key)
        return avl_left_rotate(node);

    // Caso Esquerda-Direita
    if (balance > 1 && key > node->left->key) {
        node->left = avl_left_rotate(node->left);
        return avl_right_rotate(node);
    }

    // Caso Direita-Esquerda
    if (balance < -1 && key < node->right->key) {
        node->right = avl_right_rotate(node->right);
        return avl_left_rotate(node);
    }

    // Retorna o ponteiro do nó (inalterado)
    return node;
}

// Função pública para inserir um nó na árvore AVL
void avl_insert(AVLTree* tree, int key) {
    if (tree == NULL) return;
    tree->root = avl_insert_node(tree->root, key);
}

// Função auxiliar para encontrar o nó com menor valor (nó mais à esquerda)
static AVLNode* avl_min_value_node(AVLNode* node) {
    AVLNode* current = node;
    while (current->left != NULL)
        current = current->left;
    return current;
}

// Função auxiliar para deletar um nó da subárvore com raiz root e retorna a nova raiz da subárvore
static AVLNode* avl_delete_node(AVLNode* root, int key) {
    if (root == NULL)
        return root;

    if (key < root->key)
        root->left = avl_delete_node(root->left, key);
    else if (key > root->key)
        root->right = avl_delete_node(root->right, key);
    else {
        if ((root->left == NULL) || (root->right == NULL)) {
            AVLNode* temp = root->left ? root->left : root->right;
            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else
                *root = *temp;
            free(temp);
        } else {
            AVLNode* temp = avl_min_value_node(root->right);
            root->key = temp->key;
            root->right = avl_delete_node(root->right, temp->key);
        }
    }

    if (root == NULL)
        return root;

    root->height = 1 + avl_max(avl_height(root->left), avl_height(root->right));

    int balance = avl_get_balance(root);

    if (balance > 1 && avl_get_balance(root->left) >= 0)
        return avl_right_rotate(root);

    if (balance > 1 && avl_get_balance(root->left) < 0) {
        root->left = avl_left_rotate(root->left);
        return avl_right_rotate(root);
    }

    if (balance < -1 && avl_get_balance(root->right) <= 0)
        return avl_left_rotate(root);

    if (balance < -1 && avl_get_balance(root->right) > 0) {
        root->right = avl_right_rotate(root->right);
        return avl_left_rotate(root);
    }

    return root;
}

// Função pública para deletar um nó na árvore AVL
void avl_delete(AVLTree* tree, int key) {
    if (tree == NULL) return;
    tree->root = avl_delete_node(tree->root, key);
}

// Função auxiliar para buscar um elemento na árvore AVL
static AVLNode* avl_search_node(AVLNode* node, int key) {
    // Se o nó atual é nulo ou contém a chave procurada, retorna o nó
    if (node == NULL || node->key == key)
        return node;

    // Se a chave é menor que a chave do nó atual, busca na subárvore esquerda
    if (key < node->key)
        return avl_search_node(node->left, key);
    // Se a chave é maior que a chave do nó atual, busca na subárvore direita
    else
        return avl_search_node(node->right, key);
}

// Função pública para buscar um elemento na árvore AVL
AVLNode* avl_search(AVLTree* tree, int key) {
    if (tree == NULL) return NULL; // Se a árvore é nula, retorna nulo
    return avl_search_node(tree->root, key); // Chama a função auxiliar de busca recursiva
}

// Função auxiliar para desalocar os nós da árvore
static void avl_free_node(AVLNode* node) {
    if (node != NULL) {
        avl_free_node(node->left);
        avl_free_node(node->right);
        free(node);
    }
}

// Função pública para desalocar a árvore AVL
void avl_free(AVLTree* tree) {
    if (tree == NULL) return;
    avl_free_node(tree->root);
    free(tree);
}

// Função auxiliar para impressão em ordem
static void avl_print_in_order_node(AVLNode* node) {
    if (node != NULL) {
        avl_print_in_order_node(node->left);
        printf("%d ", node->key);
        avl_print_in_order_node(node->right);
    }
}

// Função pública para impressão em ordem
void avl_print_in_order(AVLTree* tree) {
    if (tree == NULL) return;
    avl_print_in_order_node(tree->root);
    printf("\n");
}

// Função pública para criar uma árvore AVL
AVLTree* avl_create() {
    AVLTree* tree = (AVLTree*)malloc(sizeof(AVLTree));
    tree->root = NULL;
    return tree;
}
