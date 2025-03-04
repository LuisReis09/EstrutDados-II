#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "TaperedTree.h"

int nodeHeight(TaperedTree *node){
    if(!node) return 0;
    int r = nodeHeight(node->right);
    int l = nodeHeight(node->left);
    return r > l? r+1 : l+1;
}

TaperedTree *rotateRight(TaperedTree *node){
    TaperedTree *aux = node;
    node = node->left;
    aux->left = node->right;
    node->right = aux;
    return node;
}

TaperedTree *rotateLeft(TaperedTree *node){
    TaperedTree *aux = node;
    node = node->right;
    aux->right = node->left;
    node->left = aux;
    return node;
}

TaperedTree *insert(TaperedTree *self, int data){
    TaperedTree *newNode = (TaperedTree *)malloc(sizeof(TaperedTree));
    
    if(!newNode){
        printf("Erro ao alocar novo elemento!");
        return self;
    }

    newNode->data = data;
    newNode->left = newNode->right = NULL;

    if(!self){
        return newNode;
    }

    TaperedTree *aux = self;

    if(data < aux->data){
        if(!aux->left){
            aux->left = newNode;
            aux = rotateRight(aux);
        }else{
            aux->left = insert(aux->left, data);
            aux = rotateRight(aux);
        }

    }else if (data > aux->data){
        if(!aux->right){
            aux->right = newNode;
            aux = rotateLeft(aux);
        }else{
            aux->right = insert(aux->right, data);
            aux = rotateLeft(aux);
        }
    }

    
    return aux;
}

void inOrder(TaperedTree *self){
    if(!self) return;
    inOrder(self->left);
    printf(" %d ", self->data);
    inOrder(self->right);
}

TaperedTree *destruct(TaperedTree *self){
    if(!self) return NULL;
    destruct(self->left);
    destruct(self->right);
    free(self);
    self = NULL;
    return self;
}

int pot2(int n){
    return 1 << n;
}


void showAux(int level, TaperedTree *aux){
    if(!aux) return;
    showAux(level+1, aux->right);
    for(int i=0; i<level; i++)
        printf("     ");
    printf("->%3d\n", aux->data);
    showAux(level+1, aux->left);
}

void show(TaperedTree *self){
    if (!self) return;
    showAux(0, self);
    puts("---------------------");
}


/* ============================= FUNCOES PARA BUSCA ============================= */

static TaperedTree *path[100] = {NULL};
static int lastIn = 0;

void addOnPath(TaperedTree *node){
    path[lastIn++] = node;
}

void showPath(){
    printf("Caminho: ");
    for(int i=0; i<lastIn; i++){
        printf("-> %d ", path[i]->data);
    }
    puts("");
}

TaperedTree *search(TaperedTree *self, int data){
    if(!self) return NULL;
    
    if(self->data == data){
        addOnPath(self);
        printf("\nElemento %d encontrado!\n", data);
        showPath();
        lastIn = 0;
        return self;

    }else if(data < self->data){
        addOnPath(self);
        
        if(!self->left){
            printf("Elemento %d nao encontrado!\n", data);
            lastIn = 0;
            return self;
        }else{
            self->left = search(self->left, data);
            self = rotateRight(self);
            return self;
        }

    }else{
        addOnPath(self);
        
        if(!self->right){
            printf("Elemento %d nao encontrado!\n", data);
            lastIn = 0;
            return self;
        }else{
            self->right = search(self->right, data);
            self = rotateLeft(self);
            return self;
        }
    }
}

TaperedTree *pop(TaperedTree *self, int data){
    if(!self) return NULL;

    // Promoveremos a busca para que o nó a ser removido seja o nó raiz
    self = search(self, data);
    if(self->data != data){
        printf("Elemento %d nao encontrado!\n", data);
        return self;
    }

    // Agora, promoveremos a raíz o nó mais a esquerda da subárvore direita
    TaperedTree *aux = self->right;
    TaperedTree *substituto;

    if(!self->right){
        // 1° Caso: Nó raiz nao tem subárvore direita
        substituto = self->left;
        free(self);
        return substituto;

    }else if(!self->right->left){
        // 2° Caso: Nó raiz tem subárvore direita, mas o nó mais a esquerda é imediatamente o filho direito
        self->right->left = self->left;
        substituto = self->right;
        free(self);
        return substituto;

    }else{
        // 3° Caso: Nó raiz tem subárvore direita e o nó mais a esquerda nao é imediatamente o filho direito
        while(aux->left->left){
            aux = aux->left;
        }

        substituto = aux->left;
        aux->left = substituto->right;
        substituto->left = self->left;
        substituto->right = self->right;
        free(self);
        return substituto;
    }
}