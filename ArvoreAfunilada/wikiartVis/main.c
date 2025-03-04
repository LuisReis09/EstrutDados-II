#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ArvoveAVL.h"

int main(){
    tNoAVL *arvore;

    srand(time(NULL));
    IniciaArvoreAVL(&arvore);
    
    printf("Insercoes: \n");
    for(int i = 0; i < 10; i++){
        int chave = rand() % 20;
        printf("inserir %d (enter para continuar)\n", chave);
        scanf("%*c");
        arvore = InsereArvoreAVL(arvore, chave);
        if (arvore){
            printf("\tInserida %d (mas pode ser duplicada)\n", chave);
        }else{
            printf("\tErro ao inserir %d (duplicada?)\n", chave);
        }
        printf("infixa: ");
        ImprimeInfixa(arvore);
        puts("");

        printf("prefixa: ");
        ImprimePrefixa(arvore);
        puts("");
    }

    printf("Buscas: \n");
    for(int i = 0; i < 10; i++){
        int chave = rand() % 20;
        const tNoAVL *no;
        if ( !(no = BuscaArvoreBB(arvore, chave)) ){
            printf("\tChave %d NAO encontrada\n", chave);
        }else{
            printf("\tChave %d encontrada: %d\n", chave, no->chave);
        }
    }

    printf("Remocoes: \n");
    tNoAVL *retornoRemocao;
    for(int i = 0; i < 5; i++){
        int chave = rand() % 20;
        printf("\tRemover %d (enter para continuar)\n", chave);
        scanf("%*c");
        retornoRemocao = RemoveArvoreAVL(arvore, chave);
        if (retornoRemocao){
            printf("\tRemovida %d\n", chave);
            arvore = retornoRemocao; //a arvore pode ter sido alterada pelas rotacoes
        }else{
            printf("\tErro ao remover %d (nao existia ou arvore ficou vazia?)\n", chave);
        }
        printf("infixa: ");
        ImprimeInfixa(arvore);
        puts("");

        printf("prefixa: ");
        ImprimePrefixa(arvore);
        puts("");
    }

    printf("infixa: ");
    ImprimeInfixa(arvore);
    puts("");

    printf("prefixa: ");
    ImprimePrefixa(arvore);
    puts("");

    return 0;
}