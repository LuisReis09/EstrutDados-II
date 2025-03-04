#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ArvoreFunil.h"

int main(){
    tNoFunil * arvore;
    srand(time(NULL));

    /*
    while(1){
        QuestaoProva();
    }
    */

    IniciaArvoreFunil(&arvore);
    
    printf("Insercoes: \n");

    InsereArvoreFunil(&arvore, 3);
    InsereArvoreFunil(&arvore, 9);
    InsereArvoreFunil(&arvore, 11);
    InsereArvoreFunil(&arvore, 4);  
    InsereArvoreFunil(&arvore, 11);
    InsereArvoreFunil(&arvore, 18);
    InsereArvoreFunil(&arvore, 7);
    InsereArvoreFunil(&arvore, 9);
    InsereArvoreFunil(&arvore, 4);
    InsereArvoreFunil(&arvore, 9);
    InsereArvoreFunil(&arvore, 14);
    ImprimeInfixaFunil(arvore); puts("");

    // for(int i = 0; i < 13; i++){
    //     int chave = rand() % 20;
    //     printf("inserir %d (enter para continuar)\n", chave);
    //     scanf("%*c");
    //     int r = InsereArvoreFunil(&arvore, chave);
    //     if (r == 0){
    //         printf("\tInserida %d\n", chave);
    //     }else{
    //         printf("\tErro ao inserir %d (duplicada?)\n", chave);
    //     }
    //     printf("infixa: ");
    //     ImprimeInfixaFunil(arvore);
    //     puts("");

    //     printf("prefixa: ");
    //     ImprimePrefixaFunil(arvore);
    //     puts("");
    // }

    system("pause");
    printf("Buscas: \n");
    for(int i = 0; i < 3; i++){
        int chave = rand() % 20;
        if ( chave != BuscaArvoreFunil(&arvore, chave) ){
            printf("\tChave %d NAO encontrada\n", chave);
        }else{
            printf("\tChave %d encontrada\n", chave);
        }
        printf("infixa: ");
        ImprimeInfixaFunil(arvore);
        puts("");

        printf("prefixa: ");
        ImprimePrefixaFunil(arvore);
        puts("");
    }

    printf("Remocoes: \n");
    for(int i = 0; i < 4; i++){
        int chave = rand() % 20;
        printf("\tRemover %d (enter para continuar)\n", chave);
        scanf("%*c");
        int r = RemoveNoArvoreFunil(&arvore, chave);
        if (r == 0){
            printf("\tRemovida %d\n", chave);
        }else{
            printf("\tErro ao remover %d (nao existia ou arvore ficou vazia?)\n", chave);
        }
        printf("infixa: ");
        ImprimeInfixaFunil(arvore);
        puts("");

        printf("prefixa: ");
        ImprimePrefixaFunil(arvore);
        puts("");
    }

    printf("infixa: ");
    ImprimeInfixaFunil(arvore);
    puts("");

    printf("prefixa: ");
    ImprimePrefixaFunil(arvore);
    puts("");

    return 0;
}