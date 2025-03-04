#ifndef _TABELAENCADEADA_H_
#define _TABELAENCADEADA_H_

#include "Arte.h"

typedef tArte tDado;
typedef struct rotTabEnc tTabelaEnc;
struct rotTabEnc {
    tDado *dado;   
    struct rotTabEnc *prox;
};

extern void CriaTabelaEnc(tTabelaEnc **tabela);
extern void DestroiTabelaEnc(tTabelaEnc * tabela);
extern int ComprimentoEnc(tTabelaEnc * tabela);
extern tDado * RemoveElementoEnc(tTabelaEnc ** tabela, int id);
extern int EstaVaziaEnc(tTabelaEnc * tabela);
extern void AcrescentaElementoEnc(tTabelaEnc ** tabela, tDado *elemento);
extern tDado * BuscaSequencialEnc(tTabelaEnc *tab, int chave);
extern tDado * BuscaDedilhadaEnc(tTabelaEnc *tab, int chave);
extern tDado * BuscaTransposicaoEnc(tTabelaEnc **tab, int chave);
extern tDado * BuscaMovimentacaoInicioEnc(tTabelaEnc **tab, int chave);
extern tTabelaEnc * BuscaMovimentacaoInicioEncNo(tTabelaEnc **tabela, int chave);
extern int QuantOperacoesEnc();
#endif