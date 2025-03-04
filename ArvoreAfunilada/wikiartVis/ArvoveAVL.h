#ifndef _ArvoveAVL_H_
#define _ArvoveAVL_H_

#include "Arte.h"

typedef struct rotNoAVL {
        int chave;
        int altura;
        tArte *arte;
        struct rotNoAVL *esquerda, *direita;
    } tNoAVL;

extern void IniciaArvoreAVL(tNoAVL **arvore);
extern const tNoAVL *BuscaArvoreBB(const tNoAVL *arvore, int chave);
extern tNoAVL * InsereArvoreAVL(tNoAVL *arvore, int chave);
extern tNoAVL * InsereNoArvoreAVL(tNoAVL *arvore, tNoAVL *no);
extern tNoAVL * RemoveArvoreAVL(tNoAVL *arvore, int chave);
extern void ImprimeInfixa(tNoAVL *arvore);
extern void ImprimePrefixa(tNoAVL *arvore);
extern int AlturaAVL(tNoAVL *arvore);
extern int QuantOperacoesAVL();
extern const tNoAVL ** ConsultaPercursoAvl();

#endif