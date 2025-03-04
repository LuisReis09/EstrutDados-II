#include <stdio.h>
#include <stdlib.h>
#include "tabelaEncadeada.h"

static int qOperacoes = 0;

void CriaTabelaEnc(tTabelaEnc **tabela){
    *tabela = NULL;
}

void DestroiTabelaEnc(tTabelaEnc * tabela){
    tTabelaEnc * temp;
    while(tabela != NULL){
        temp = tabela;
        tabela = tabela->prox;

        free(temp);
    }
}

int ComprimentoEnc(tTabelaEnc * tabela){
    int c = 0;
    
    while(tabela != NULL){
        c++;
        tabela = tabela->prox;
    }

    return c;
}

tDado * RemoveElementoEnc(tTabelaEnc ** tabela, int id){
    tTabelaEnc *anterior = NULL, *atual = *tabela, *removido = NULL;
    while(atual != NULL){
        if (atual->dado->id == id){
            //elemento encontrado
            removido = atual;
            break;
        }
        anterior = atual;
        atual = atual->prox;
    }

    if (removido != NULL){
        if (anterior != NULL){
            anterior->prox = removido->prox;
        }else{
            //primeiro removido
            (*tabela)->prox = removido->prox;
        }
        tDado *retorno = malloc(sizeof(tDado));
        if (!retorno){
            puts("Erro ao alocar elemento para o retorno.");
            exit(1);
        }
        *retorno = *removido->dado;
        free(removido);
        return retorno;
    }

    return NULL;
}

int EstaVaziaEnc(tTabelaEnc * tabela){
    return tabela == NULL;
}

void AcrescentaElementoEnc(tTabelaEnc **tabela, tDado *elemento){
    tTabelaEnc *novo = malloc(sizeof(tTabelaEnc));
    if (!novo){
        puts("Erro ao alocar novo elemento.");
        exit(1);
    }
    novo->dado = elemento;
    novo->prox = *tabela;
    *tabela = novo;
}

tDado * BuscaSequencialEnc(tTabelaEnc *tabela, int id){
    qOperacoes = 0;
    while(tabela != NULL){
        qOperacoes++;
        if (tabela->dado->id == id){
            return tabela->dado;
        }
        tabela = tabela->prox;
    }

    return NULL;
}

tDado * BuscaDedilhadaEnc(tTabelaEnc *tab, int id){ 
    static tTabelaEnc *ultimoNo = NULL;
    if (ultimoNo == NULL){
        ultimoNo = tab;
    }

    qOperacoes = 0;
    while(ultimoNo != NULL){
        if (ultimoNo->dado->id == id){
            return ultimoNo->dado;
        }
        ultimoNo = ultimoNo->prox;
    }

    return NULL;
}

tDado * BuscaTransposicaoEnc(tTabelaEnc **tabela, int chave){
    tTabelaEnc *anterior1 = NULL, *anterior2 = NULL, *atual = *tabela;
    qOperacoes = 0;
    while(atual != NULL){
        qOperacoes++;
        if (atual->dado->id == chave){
            if (anterior1 != NULL){
                anterior1->prox = atual->prox;
                atual->prox = anterior1;
                if (anterior2 == NULL){
                    //atual sera cabeca
                    *tabela = atual;
                }else{
                    anterior2->prox = atual;
                }
            }else{
                //ja eh primeiro
            }
            return atual->dado;
        }
        anterior2 = anterior1;
        anterior1 = atual;
        atual = atual->prox;
    }

    return NULL;
}

tDado * BuscaMovimentacaoInicioEnc(tTabelaEnc **tabela, int chave){
    tTabelaEnc *anterior = NULL, *atual = *tabela;
    qOperacoes = 0;
    while(atual != NULL){
        qOperacoes++;
        if (atual->dado->id == chave){
            if (anterior != NULL){
                anterior->prox = atual->prox;
                atual->prox = *tabela;
                *tabela = atual;
            }else{
                //ja eh primeiro
            }
            return atual->dado;
        }
        anterior = atual;
        atual = atual->prox;
    }

    return NULL;
}

tTabelaEnc * BuscaMovimentacaoInicioEncNo(tTabelaEnc **tabela, int chave){
    tTabelaEnc *anterior = NULL, *atual = *tabela;
    qOperacoes = 0;
    while(atual != NULL){
        qOperacoes++;
        if (atual->dado->id == chave){
            if (anterior != NULL){
                anterior->prox = atual->prox;
                atual->prox = *tabela;
                *tabela = atual;
            }else{
                //ja eh primeiro
            }
            return atual;
        }
        anterior = atual;
        atual = atual->prox;
    }

    return NULL;
}

int QuantOperacoesEnc(){
    return qOperacoes;
}