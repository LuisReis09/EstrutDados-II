#include <stdio.h>
#include <stdlib.h>
#include "AVLTree.h"

static AVLTree *instance;

static AVLNode *rotateRight(AVLNode *node){
    AVLNode *aux = node->left;
    node->left = aux->right;
    aux->right = node;
    node = aux;

    return node;
}

static AVLNode *rotateLeft(AVLNode *node){
    AVLNode *aux = node->right;
    node->right = aux->left;
    aux->left = node;
    node = aux;

    return node;
}

static int nodeHeight(AVLNode* root){
    if(!root) return 0;
    int leftHeight = nodeHeight(root->left);
    int rightHeight = nodeHeight(root->right);

    return 1 + (leftHeight > rightHeight ? leftHeight:rightHeight);
}

static int balancingFactor(AVLNode *root){
    int leftHeight  = nodeHeight(root->left);
    int rightHeight = nodeHeight(root->right);
    return leftHeight - rightHeight;
}

static void balanceNode(AVLNode **Root){
    // if the tree's empty, does nothing
    AVLNode *root = *Root;
    if(!root) return;

    int fb = balancingFactor(root);

    if(fb > 1){
        if(balancingFactor(root->left) < 0)
            root->left = rotateLeft(root->left);
        root = rotateRight(root);

    }else if(fb < -1){
        if(balancingFactor(root->right) > 0)
            root->right = rotateRight(root->right);
        root = rotateLeft(root);
    }

    *Root = root;
}

boolean insertAux(AVLNode **Root, AVLNode *node){
    AVLNode *root = *Root;
    if(node->data <= root->data){

        if(!root->left)
            root->left = node;
        else
            insertAux(&root->left, node);
        
    }else{

        if(!root->right)
            root->right = node;
        else
            insertAux(&root->right, node);
        
    }

    *Root = root;
    balanceNode(Root);
    return 1;
}


// Returns 0 if the insertion was unsuccessfull or 1 if it was successful.
boolean insert(int data){
    if(!instance) return 0;

    AVLNode *to_insert = (AVLNode*) malloc(sizeof(AVLNode));
    if(!to_insert) return 0;
    to_insert->data = data;
    to_insert->left = to_insert->right = NULL;
    
    if(!instance->root){
        instance->root = to_insert;
        return 1;
    }else{
        return insertAux(&instance->root, to_insert);
    }

    
}

static boolean popAux(AVLNode** root, AVLNode** node){

    if (!root || !(*root) || !node || !(*node)) return 0; // Verifica se os ponteiros são válidos

    if (*node == (*root)->left) {
        // Removendo filho da esquerda

        if (!(*node)->right) {
            (*root)->left = (*node)->left;
            free(*node);
            *node = NULL;
        } else {
            // Encontra o menor nó da subárvore direita
            AVLNode *substitute = (*node)->right;
            AVLNode *aux = *node;

            while (substitute->left) {
                aux = substitute;
                substitute = substitute->left;
            }

            // Ajusta os ponteiros corretamente
            if (aux != *node) {
                aux->left = substitute->right;
            } else {
                aux->right = substitute->right;
            }

            substitute->left = (*node)->left;
            substitute->right = (*node)->right;
            (*root)->left = substitute;

            free(*node);
            *node = NULL;
        }
    } else {
        // Removendo filho da direita

        if (!(*node)->right) {
            (*root)->right = (*node)->left;
            free(*node);
            *node = NULL;
        } else {
            // Encontra o menor nó da subárvore direita
            AVLNode *substitute = (*node)->right;
            AVLNode *aux = *node;

            while (substitute->left) {
                aux = substitute;
                substitute = substitute->left;
            }

            // Ajusta os ponteiros corretamente
            if (aux != *node) {
                aux->left = substitute->right;
            } else {
                aux->right = substitute->right;
            }

            substitute->left = (*node)->left;
            substitute->right = (*node)->right;
            (*root)->right = substitute;

            free(*node);
            *node = NULL;
        }
    }

    // Balancear a subárvore após a remoção
    balanceNode(root);
    return 1;

}

static boolean popInitial(AVLNode **initial){
    if (!initial || !(*initial)) return 0; // Se a árvore está vazia, não há nada para remover

    AVLNode *aux = *initial;

    if ((*initial)->right) { 
        // Se houver um filho à direita, buscar o menor nó da subárvore direita
        AVLNode *substitute = (*initial)->right;
        AVLNode *parent = *initial;

        while (substitute->left) {
            parent = substitute;
            substitute = substitute->left;
        }

        // Ajustar os ponteiros do pai do substituto
        if (parent != *initial) {
            parent->left = substitute->right;
        } else {
            parent->right = substitute->right;
        }

        // Substituir a raiz pelo sucessor in-order
        substitute->left = (*initial)->left;
        substitute->right = (*initial)->right;
        *initial = substitute;

    } else {
        // Se não houver filho à direita, apenas substituir pela subárvore esquerda
        *initial = (*initial)->left;
    }

    free(aux);  // Liberar a raiz original

    // Balancear a nova raiz
    if (*initial) balanceNode(initial);

    return 1; // Remoção bem-sucedida
}

/*
    Returns 0 if:
    - There's no instance selected
    - The tree is empty
    - The element is not in the tree

    Returns 1 if the element is successfully removed from the tree

*/
boolean pop(int data){
    if(!instance || !instance->root) return 0;
    
    AVLNode *aux = instance->root, *aux2 = aux;

    if(data == instance->root->data)
        return popInitial(&instance->root);

    while(aux){
        if(aux->data == data)
            return popAux(&aux2, &aux);
        
        if(data <= aux->data){
            aux2 = aux;
            aux = aux->left;
        }
        else{
            aux2 = aux;
            aux = aux->right;
        }
    }

    printf("Elemento %d nao encontrado!\n");
    return 0;
}


static void showAux(AVLNode *node, int nivel){
    if(!node) return;
    showAux(node->right, nivel+1);
    for(int i=0; i<nivel; i++)
    printf("      ");
    printf("-> %3d\n", node->data);
    showAux(node->left, nivel+1);
}

void show(){
    if(!instance) return;
    puts("");
    showAux(instance->root, 0);
    puts("");
}

void setInstance(AVLTree* tree){
    instance = tree; 
}

static void prefixAux(AVLNode* node){
    if(!node) return;
    printf("%d ", node->data);
    prefixAux(node->left);
    prefixAux(node->right);
}

static void infixAux(AVLNode* node){
    if(!node) return;
    infixAux(node->left);
    printf("%d ", node->data);
    infixAux(node->right);
}

static void posfixAux(AVLNode* node){
    if(!node) return;
    posfixAux(node->left);
    posfixAux(node->right);
    printf("%d ", node->data);
}

void prefix(){
    prefixAux(instance->root);
}

void infix(){
    infixAux(instance->root);
}

void posfix(){
    posfixAux(instance->root);
}

void destructTree(AVLNode *node){
    if(!node) return;
    destructTree(node->left);
    destructTree(node->right);
    free(node);
    node = NULL;
}

void destruct(){
    if(!instance) return;
    destructTree(instance->root);
    free(instance);
    instance = NULL;   
}


AVLTree *new_AVLTree(){
    AVLTree* tree = (AVLTree*) malloc(sizeof(AVLTree));
    
    tree->root      = NULL;
    tree->insert    = insert;
    tree->pop       = pop;
    tree->show      = show;
    tree->prefix    = prefix;
    tree->infix     = infix;
    tree->posfix    = posfix;
    tree->destruct  = destruct;

    return tree;
}
