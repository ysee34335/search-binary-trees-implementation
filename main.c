#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "avltree.h"
#include "rbtree.h"


// Função para imprimir uma árvore AVL ou Red-Black
void print_tree(AVLNode* root) {
    if (root != NULL) {
        printf("%d ", root->key);
        print_tree(root->left);
        print_tree(root->right);
    }
}


int main() {
    int n = 1000;
    clock_t start, end;
    double cpuTimeUsed;

    // Inicializar árvores AVL e Red-Black
    AVLTree* avl_tree = avl_create();
    RBTree* rb_tree = rbtree_create();

    // Preenche a arvore com valores aleatorios entre 1 e n
    srand(time(0)); 
    for (int i = 0; i < n; i++) {
        int randomValue = rand() % (n + 1);
        avl_insert(avl_tree, randomValue);
    }
    int y = 3;
    int x = 4;
    for (int i = 0; i < 50; i++) {
        clock_t start = clock();
        avl_search(avl_tree, rand() % (n + 1));
        clock_t end = clock();
        cpuTimeUsed = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("%f\n", cpuTimeUsed);
    }


    avl_free(avl_tree);
    rbtree_free(rb_tree);

    return 0;
}
