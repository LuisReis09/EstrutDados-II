#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ArvoveAVL.h"

#define MAIOR(a, b) (((a) > (b)) ? (a) : (b))
#define DEBUG 0

static int qOperacoes = 0;
static const tNoAVL *percurso[50];

static tNoAVL * RotacaoEsquerdaAVL(tNoAVL *raiz){
    tNoAVL *pivo = raiz->direita;

   raiz->direita = pivo->esquerda;
   pivo->esquerda = raiz;

      /* Atualiza as alturas */
   raiz->altura = MAIOR(AlturaAVL(raiz->esquerda), AlturaAVL(raiz->direita)) + 1;
   pivo->altura = MAIOR(AlturaAVL(pivo->esquerda), AlturaAVL(pivo->direita)) + 1;

   return pivo; /* Retorna a nova raiz */
}

static tNoAVL * RotacaoDireitaAVL(tNoAVL *raiz){
    tNoAVL *pivo = raiz->esquerda;

   raiz->esquerda = pivo->direita;
   pivo->direita = raiz;

      /* Atualiza as alturas */
   raiz->altura = MAIOR(AlturaAVL(raiz->esquerda), AlturaAVL(raiz->direita)) + 1;
   pivo->altura = MAIOR(AlturaAVL(pivo->esquerda), AlturaAVL(pivo->direita)) + 1;

      /* Retorna a nova raiz */
   return pivo;
}

static int BalanceamentoAVL(tNoAVL *pNo){
   return pNo ? AlturaAVL(pNo->esquerda) - AlturaAVL(pNo->direita) : 0;
}

int AlturaAVL(tNoAVL *arvore){
   return arvore ? arvore->altura : 0;
}

void IniciaArvoreAVL(tNoAVL **arvore){
   *arvore = NULL;
}

const tNoAVL *BuscaArvoreBB(const tNoAVL *arvore, int chave){ //mesmo que ABB
    const tNoAVL *noAtual = arvore;
    
    qOperacoes = 0;
    
    while(noAtual){
        //printf("[debug] registrar passo #%d: %p\n", qOperacoes, noAtual);
        percurso[qOperacoes] = noAtual;
        qOperacoes++;

        if (chave == noAtual->chave){
            return noAtual;
        }

        if (chave < noAtual->chave){
            noAtual = noAtual->esquerda;
        }else{
            noAtual = noAtual->direita;
        }
    }

    return NULL;
}

static tNoAVL *NovoNo(int chave){
    tNoAVL *novo = malloc(sizeof(tNoAVL));
    if (!novo)
        return NULL;
    
    novo->chave = chave;
    novo->esquerda = novo->direita = NULL;
    novo->altura = 1; //folha

    return novo;
}

tNoAVL * InsereNoArvoreAVL(tNoAVL *arvore, tNoAVL *no){
    if (!arvore){ //arvore vazia (ou chegou na folha pela recursao)
        no->altura = 1;
        no->esquerda = no->direita = NULL;
        arvore = no;
        return arvore;
    }

    if (no->chave == arvore->chave){ //como eh recursivo, arvore aqui eh a subarvore sendo visitada
        return arvore; //chave é primária (atencao, no livro estah NULL)
    }

    /* se chegou aqui, a arvore original nao esta vazia, entao sera inserido recursivamente */
    if (no->chave < arvore->chave){
        arvore->esquerda = InsereNoArvoreAVL(arvore->esquerda, no);
    }else{
        arvore->direita = InsereNoArvoreAVL(arvore->direita, no);
    }

    //esse ponto eh atingido apos a insercao do novo no e retorno das recursoes 

    //atualizar altura da [sub]arvore 
    arvore->altura = MAIOR(AlturaAVL(arvore->esquerda), AlturaAVL(arvore->direita)) + 1;
    int balanceamento = BalanceamentoAVL(arvore); //balanceamento de um nó, diferenca das alturas

    //examinar os 4 casos de desbalanceamento 
    if (balanceamento > 1){
        //esquerda-esquerda ou esquerda-direita
        
        if (no->chave < arvore->esquerda->chave){
            //esquerda-esquerda
            return RotacaoDireitaAVL(arvore);
        }

        if (no->chave > arvore->esquerda->chave){
            //esquerda-direita
            arvore->esquerda = RotacaoEsquerdaAVL(arvore->esquerda);
            return RotacaoDireitaAVL(arvore);
        }
    }

    if (balanceamento < -1){
        //direita-direita ou direita-esquerda

        if (no->chave > arvore->direita->chave){
            //direita-direita
            return RotacaoEsquerdaAVL(arvore);
        }

        if (no->chave < arvore->direita->chave){
            //direita-esquerda
            arvore->direita = RotacaoDireitaAVL(arvore->direita);
            return RotacaoEsquerdaAVL(arvore); 
        }
    }

    return arvore; //se chegou aqui nao precisou rebalanceamento    
}

tNoAVL * InsereArvoreAVL(tNoAVL *arvore, int chave){
    if (!arvore){ //arvore vazia
        arvore = NovoNo(chave);
        return arvore;
    }

    if (chave == arvore->chave){ //como eh recursivo, arvore aqui eh a subarvore sendo visitada
        return arvore; //chave é primária (atencao, no livro estah NULL)
    }

    /* se chegou ate aqui, a arvore nao esta vazia, entao sera inserido recursivamente */
    if (chave < arvore->chave){
        arvore->esquerda = InsereArvoreAVL(arvore->esquerda, chave);
    }else{
        arvore->direita = InsereArvoreAVL(arvore->direita, chave);
    }

    //esse ponto eh atingido apos a insercao do novo no e retorno das recursoes 

    //atualizar altura da [sub]arvore 
    arvore->altura = MAIOR(AlturaAVL(arvore->esquerda), AlturaAVL(arvore->direita)) + 1;
    int balanceamento = BalanceamentoAVL(arvore); //balanceamento de um nó, diferenca das alturas

    //examinar os 4 casos de desbalanceamento 

    if (balanceamento > 1){
        //esquerda-esquerda ou esquerda-direita

        if (chave < arvore->esquerda->chave){
            //esquerda-esquerda
            return RotacaoDireitaAVL(arvore);
        }

        if (chave > arvore->esquerda->chave){
            //esquerda-direita
            arvore->esquerda = RotacaoEsquerdaAVL(arvore->esquerda);
            return RotacaoDireitaAVL(arvore);
        }
    }

    if (balanceamento < -1){
        //direita-direita ou direita-esquerda

        if (chave > arvore->direita->chave){
            //direita-direita
            return RotacaoEsquerdaAVL(arvore);
        }

        if (chave < arvore->direita->chave){
            //direita-esquerda
            arvore->direita = RotacaoDireitaAVL(arvore->direita);
            return RotacaoEsquerdaAVL(arvore); 
        }
    }

    return arvore; //se chegou aqui nao precisou rebalanceamento
}

tNoAVL * RemoveArvoreAVL(tNoAVL * arvore, int chave){
    if (!arvore){
        return NULL; //arvore vazia, nao hao que remover
    }

    tNoAVL * pNo = NULL; //no a ser liberado com free, o procurado ou o sucessor

    if (chave < arvore->chave){
        arvore->esquerda = RemoveArvoreAVL(arvore->esquerda, chave);
    }else if (chave > arvore->chave){
        arvore->direita = RemoveArvoreAVL(arvore->direita, chave);
    }else{ //arvore esta apontando para chave
        //chave encontrada
        if( (!arvore->esquerda) || (!arvore->direita) ) {
            /* Nó só possui um filho ou nenhum, pNo apontara para o nó filho
            a ser removido devido a substituicao do conteudo do pai por ele */
            pNo = arvore->esquerda ? arvore->esquerda : arvore->direita;

            if(!pNo) { 
                //sem filhos
                pNo = arvore; //sem filhos, vai apenas dar o free
                arvore = NULL; // [sub]arvore ficou vazia, pai que chamou recursivamente apontara para NULL
            } else{ 
                //um filho
                *arvore = *pNo; //copia conteudo (chave e filhos) desse unico filho para o pai
            }
            free(pNo); /* Libera o nó */
        } else { 
            //no a remover tem dois filhos

            /* pNo apontará para o sucessor do nó a ser removido (seu substituto) 
            q precisará ser removido (o substituto) após seu conteúdo ser copiado para o no a ser removido */
            pNo = arvore->direita; 
            while (pNo->esquerda){
                pNo = pNo->esquerda;
            }

            arvore->chave = pNo->chave; //apenas conteúdo do nó substituto substitui o nó a ser removido
            arvore->arte = pNo->arte;
            /* Após a substituicao do conteudo, o nó substituo precisa ser removido da arvore,
            seguindo as mesmas regras gerais. Essa remoção ocorre na árvore direita e pode alterá-la */
            arvore->direita = RemoveArvoreAVL(arvore->direita, pNo->chave);
        }
    }
    
    //removido, agora verificar o balanceamento 

    if (!arvore){
        //[sub]arvore só tinha um nó (a raiz)
        return arvore;
    }

    //atualiza altura da [sub]arvore atual, o nó corrente
    arvore->altura = MAIOR(AlturaAVL(arvore->esquerda), AlturaAVL(arvore->direita)) + 1;
    int balanceamento = BalanceamentoAVL(arvore); //a arvore ficou desbalanceada?

    //analisar os 4 casos de desbalanceamento?

    //esquerda-esquerda
    if (balanceamento > 1 && BalanceamentoAVL(arvore->esquerda) >= 0){ // Atenção ao >= da operacao 
        return RotacaoDireitaAVL(arvore);
    }
    
    //esquerda-direita
    if (balanceamento > 1 && BalanceamentoAVL(arvore->esquerda) < 0) {
        arvore->esquerda =  RotacaoEsquerdaAVL(arvore->esquerda);
        return RotacaoDireitaAVL(arvore);
    }

    //direita-direita 
    if (balanceamento < -1 && BalanceamentoAVL(arvore->direita) <= 0){
        return RotacaoEsquerdaAVL(arvore);
    }

    //direita-esquerda
    if (balanceamento < -1 && BalanceamentoAVL(arvore->direita) > 0) {
        arvore->direita = RotacaoDireitaAVL(arvore->direita);
        return RotacaoEsquerdaAVL(arvore);
    }

    return arvore; /* Nao ocorreu desbalanceamento */
}

void ImprimeInfixa(tNoAVL *arvore){
    if (!arvore)
        return;
    
    ImprimeInfixa(arvore->esquerda);
    printf("%d(%d) ", arvore->chave, BalanceamentoAVL(arvore));
    ImprimeInfixa(arvore->direita);
}

void ImprimePrefixa(tNoAVL *arvore){
    if (!arvore)
        return;
    
    printf("%d(%d) ", arvore->chave, BalanceamentoAVL(arvore));
    ImprimePrefixa(arvore->esquerda);
    ImprimePrefixa(arvore->direita);
}

int QuantOperacoesAVL(){
    return qOperacoes;
}

const tNoAVL ** ConsultaPercursoAvl(){
    return percurso;
}