#include <stdio.h>
#include "TaperedTree.h"


int main(){
    TaperedTree *tree = NULL;
    tree = insert(tree, 3);
    tree = insert(tree, 9);
    // tree = insert(tree, 11);
    // tree = insert(tree, 4);
    // tree = insert(tree, 11);
    // tree = insert(tree, 18);
    // tree = insert(tree, 7);
    // tree = insert(tree, 9);
    // tree = insert(tree, 4);
    // tree = insert(tree, 9);
    // tree = insert(tree, 14);
    inOrder(tree); puts("");
    show(tree);

    // tree = search(tree, 20);
    // show(tree);

    tree = pop(tree, 9);
    show(tree);

    tree = destruct(tree);
    show(tree);
    return 0;
}