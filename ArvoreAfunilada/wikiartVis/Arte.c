#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Arte.h"

tArte * CriaArte(int id, const char *artista, const char *descricao, const char *genero, const char *nomeArquivo, int largura, int altura){
    tArte *novaArte = malloc(sizeof(tArte));
    if (!novaArte){
        puts("Erro ao tentar alocar nova arte.");
        return NULL;
    }

    novaArte->id = id;

    if ( (novaArte->artista = malloc(strlen(artista)+1)) == NULL ){
        puts("Erro ao tentar alocar \'artista\' de nova arte.");
        free(novaArte);
        return NULL;
    }else{
        strcpy(novaArte->artista, artista);
    }

    if ( (novaArte->descricao = malloc(strlen(descricao)+1)) == NULL ){
        puts("Erro ao tentar alocar \'descricao\' de nova arte.");
        free(novaArte->artista);
        free(novaArte);
        return NULL;
    }else{
        strcpy(novaArte->descricao, descricao);
    }

    if ( (novaArte->genero = malloc(strlen(genero)+1)) == NULL ){
        puts("Erro ao tentar alocar \'genero\' de nova arte.");
        free(novaArte->descricao);
        free(novaArte->artista);
        free(novaArte);
        return NULL;
    }else{
        strcpy(novaArte->genero, genero);
    }

    if ( (novaArte->nomeArquivo = malloc(strlen(nomeArquivo)+1)) == NULL ){
        puts("Erro ao tentar alocar \'nomeArquivo\' de nova arte.");
        free(novaArte->genero);
        free(novaArte->descricao);
        free(novaArte->artista);
        free(novaArte);
        return NULL;
    }else{
        strcpy(novaArte->nomeArquivo, nomeArquivo);
    }

    return novaArte;
}

void LiberaArte(tArte **arte){
    free((*arte)->artista);
    free((*arte)->descricao);
    free((*arte)->genero);
    free((*arte)->nomeArquivo);
    
    free(*arte);
    *arte = NULL;
}

void ImprimeArte(const tArte *umaArte){
    printf("%s, %s (%s)\n", umaArte->descricao, umaArte->artista, umaArte->genero);
}

void ConcatenaHtml(char *destino, const tArte *umaArte, int ehLink, const char *detalheLink){
    char *tempHtml = malloc(1024);
    if (!tempHtml){
        puts("Erro ao tentar alocar HTML temporario.");
        exit(1);
    }
    
    if (ehLink){
        //printf("[debug] detalhes link: %s\n", detalheLink);
        sprintf(tempHtml, "<a href=\"/%s/%d\"><b>%s</b></a> | %s<br>\n"
                        "<i>%s</i>\n", detalheLink, umaArte->id, umaArte->descricao, umaArte->artista, umaArte->genero);
    }else{
        sprintf(tempHtml, "<b>%s</b> | %s<br>\n"
                        "<i>%s</i>\n", umaArte->descricao, umaArte->artista, umaArte->genero);
    }
    strcpy(destino, tempHtml);

    free(tempHtml);
}