#include <stdio.h>
#include <stdlib.h>
#include "AVLTree.h"

int main(){
    int x;
    AVLTree *tree = new_AVLTree();
    setInstance(tree);

    printf("\nInsercoes:\n");
    do{
        scanf("%d", &x);
        tree->insert(x);
        puts("");
        tree->show();
        puts("");
    }while(x != 0);


    printf("\nRemocoes:\n");
    do{
        scanf("%d", &x);
        tree->pop(x);
        tree->show();
    }while(x != 0);

    tree->destruct();
    return 0;
}