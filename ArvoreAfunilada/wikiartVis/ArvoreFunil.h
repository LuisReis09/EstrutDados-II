#ifndef _ArvoreFunil_
#define _ArvoreFunil_

#include <stdio.h>
#include "Arte.h"   

typedef struct rotNoFunil {
        struct rotNoFunil *esquerda, *direita;
        int chave;
        const tArte *arte;
    } tNoFunil;


extern tNoFunil * ConstroiNo(int chave);
extern void IniciaArvoreFunil(tNoFunil **arvore);
extern int NumeroNosArvoreFunil(tNoFunil *arvore);
extern int ProfundidadeArvoreFunil(tNoFunil *arvore);
extern int BuscaArvoreFunil(tNoFunil **arvore, int chave);
extern tNoFunil * BuscaNoArvoreFunil(tNoFunil **arvore, int chave);
extern int InsereArvoreFunil( tNoFunil **arvore, int chave);
extern int InsereArteArvoreFunil( tNoFunil **arvore, const tArte *arte);
extern int RemoveNoArvoreFunil(tNoFunil **arvore, int chave);
extern int EstaVaziaLSE(tNoFunil *arvore);
extern void DestroiArvoreBin(tNoFunil *arvore);
extern void ImprimeInfixaFunil(tNoFunil *arvore);
extern void ImprimePrefixaFunil(tNoFunil *arvore);
extern int MenorChave(tNoFunil *p);
extern int MaiorChave(tNoFunil *p);
extern int QuantOperacoesFunil();
extern const tNoFunil ** ConsultaPercursoFunil();


#endif
