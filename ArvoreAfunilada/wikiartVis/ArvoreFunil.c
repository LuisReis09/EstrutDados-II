
#include <stdio.h>    
#include <stdlib.h>   
#include "ArvoreFunil.h" 

static int qOperacoes = 0;
static const tNoFunil *percurso[50];


tNoFunil * ConstroiNo(int chave){
   tNoFunil * no;

   no = malloc(sizeof(tNoFunil));
   if (!no){
      puts("Erro ao alocar novo no da arvore afunilada.");
      exit(1);
   }

   no->chave = chave;
   no->esquerda = NULL; 
   no->direita = NULL;

   return no;
}

static tNoFunil * RotacaoDireita(tNoFunil *x){
   tNoFunil *y = x->esquerda; 
   x->esquerda = y->direita;

   y->direita = x;

   return y; /* Retorna a nova raiz */
}

static tNoFunil * RotacaoEsquerda(tNoFunil *x){
   tNoFunil *y = x->direita;
   
   x->direita = y->esquerda;
   y->esquerda = x;

   return y; /* Retorna a nova raiz */
}

static tNoFunil *MenorNo(tNoFunil *arvore){
   if (!arvore){
      return NULL;
   }

   while (arvore->esquerda)
      arvore = arvore->esquerda;

   return arvore;
}

static tNoFunil * Afunila(tNoFunil *arvore, int chave){

      /* Se a arvore estiver vazia nao ha o que afunilar */
   if (!arvore)
      return NULL;

   if (qOperacoes < 50){
      percurso[qOperacoes] = arvore;
   }
   qOperacoes++;

      /* Se a chave ja estiver na raiz nao ha o que afunilar */
   if (arvore->chave == chave)
      return arvore;

      /* Verifica se a chave se encontra na subarvore esquerda */
   if (chave < arvore->chave) {

      if (!arvore->esquerda) 
         return arvore;


      if (chave < arvore->esquerda->chave) {
         arvore->esquerda->esquerda = Afunila(arvore->esquerda->esquerda, chave);
         arvore = RotacaoDireita(arvore);

      } else if (chave > arvore->esquerda->chave) { 
         arvore->esquerda->direita = Afunila(arvore->esquerda->direita, chave);
         if (arvore->esquerda->direita != NULL)
            arvore->esquerda=RotacaoEsquerda(arvore->esquerda);
      }

      return !arvore->esquerda ? arvore : RotacaoDireita(arvore);
   } else {
      if (!arvore->direita)
         return arvore;

         /* Compara a chave do no a ser afunilado com */
         /* a chave da raiz da subarvore direita      */
      if (chave < arvore->direita->chave) { /* Caso 3B: Efetua-se zig-zag */

            /* Primeiro, recursivamente, afunila-se o no */
            /* que contem a chave na subarvore esquerda */
            /* da subarvore direita                     */
         arvore->direita->esquerda = Afunila(arvore->direita->esquerda, chave);

            /* Efetua a rotacao zig. A segunda rotacao */
            /* ocorrerah apos esta instrucao if.       */
         if (arvore->direita->esquerda != NULL)
            arvore->direita = RotacaoDireita(arvore->direita);
      } else if (chave > arvore->direita->chave) { /* Caso 2B: Zag-Zag */

            /* Primeiro, recursivamente, afunila-se o no */
            /* que contem a chave na subarvore direita   */
            /* da subarvore direita                      */
         arvore->direita->direita = Afunila(arvore->direita->direita, chave);

            /* Efetua a primeira rotacao zag. A segunda */
            /* rotacao ocorrera apos esta instrucao if. */
         arvore = RotacaoEsquerda(arvore);
      }

         /* Efetua uma rotacao esquerda (zag) sobre a raiz */
         /* da arvore, a nao ser que arvore tenha ficado   */
         /* sem filho direito apos o afunilamento          */
      return !arvore->direita ? arvore : RotacaoEsquerda(arvore);
   }
}

void IniciaArvoreFunil(tNoFunil **arvore){
   *arvore = NULL;
}

int NumeroNosArvoreFunil(tNoFunil *arvore){
   if (!arvore) {
      return 0;
   }

   return 1 + NumeroNosArvoreFunil(arvore->esquerda) +
              NumeroNosArvoreFunil(arvore->direita);
}

int ProfundidadeArvoreFunil(tNoFunil *arvore){
   int profEsq, profDir;

   if (!arvore) {
      return 0; 
   } else {
        /* Calcula a profundidade da subarvore esquerda */
      profEsq = 1 + ProfundidadeArvoreFunil(arvore->esquerda);

        /* Calcula a profundidade da subarvore direita */
      profDir = 1 + ProfundidadeArvoreFunil(arvore->direita);

      return profEsq > profDir ? profEsq : profDir;
   }
}

int BuscaArvoreFunil(tNoFunil **arvore, int chave){ //retornar o mesmo valor da chave ou -1
   if (!*arvore) {
      return -1;
   }

   qOperacoes = 0;

   *arvore = Afunila(*arvore, chave);
   if ((*arvore)->chave == chave){
      return chave;
   }else{
      return -1;
   }
}

tNoFunil * BuscaNoArvoreFunil(tNoFunil **arvore, int chave){ 
   if (!*arvore) {
      return NULL;
   }
   
   qOperacoes = 0;
   
   *arvore = Afunila(*arvore, chave);
   if ((*arvore)->chave == chave){
      return *arvore;
   }else{
      return NULL;
   }
}


int InsereArvoreFunil(tNoFunil **arvore, int chave){ //retorna 1 quando ja existe ou 0 ao inserir
   tNoFunil *pNovoNo; //novo no 

   if (!*arvore) {
      *arvore = ConstroiNo(chave); //se vazia, torna-se a raiz
      return 0;
   }

   qOperacoes = 0;

   *arvore = Afunila(*arvore, chave); //afunila o no com a chave

   if (chave == (*arvore)->chave) {
      return 1; //chave ja existe 
   } else {
      pNovoNo = ConstroiNo(chave);

         /* Faz com que o novo no se torne a raiz da arvore */
      if (chave < (*arvore)->chave) { //chave menor que a raiz atual, q ficarah, entao, a direita
            /* Faz o filho direito do novo */
            /* no apontar para a raiz      */
         pNovoNo->direita = *arvore;

            /* Faz o filho esquerdo do novo no apontar */
            /* para o filho esquerdo da raiz           */
         pNovoNo->esquerda = (*arvore)->esquerda;

            /* Torna nulo o filho esquerdo da raiz */
         (*arvore)->esquerda = NULL;
      } else { //chave maior que raiz atual, q sera seu filho esquerdo
         pNovoNo->esquerda = *arvore;

            /* Faz o filho direito do novo no apontar */
            /* para o filho direito da raiz           */
         pNovoNo->direita = (*arvore)->direita;
         (*arvore)->direita = NULL;
      }

         /* O novo no passa a ser a nova raiz */
      *arvore = pNovoNo;
   }

   return 0; //inseriu
}

int InsereArteArvoreFunil( tNoFunil **arvore, const tArte *arte){
   tNoFunil *pNovoNo; //novo no 

   if (!*arvore) {
      *arvore = ConstroiNo(arte->id); //se vazia, torna-se a raiz
      (*arvore)->arte = arte;
      return 0;
   }

   int chave = arte->id;
   qOperacoes = 0;

   *arvore = Afunila(*arvore, chave); //afunila o no com a chave

   if (chave == (*arvore)->chave) {
      return 1; //chave ja existe 
   } else {
      pNovoNo = ConstroiNo(arte->id);
      pNovoNo->arte = arte;

         /* Faz com que o novo no se torne a raiz da arvore */
      if (chave < (*arvore)->chave) { //chave menor que a raiz atual, q ficarah, entao, a direita
            /* Faz o filho direito do novo */
            /* no apontar para a raiz      */
         pNovoNo->direita = *arvore;

            /* Faz o filho esquerdo do novo no apontar */
            /* para o filho esquerdo da raiz           */
         pNovoNo->esquerda = (*arvore)->esquerda;

            /* Torna nulo o filho esquerdo da raiz */
         (*arvore)->esquerda = NULL;
      } else { //chave maior que raiz atual, q sera seu filho esquerdo
         pNovoNo->esquerda = *arvore;

            /* Faz o filho direito do novo no apontar */
            /* para o filho direito da raiz           */
         pNovoNo->direita = (*arvore)->direita;
         (*arvore)->direita = NULL;
      }

         /* O novo no passa a ser a nova raiz */
      *arvore = pNovoNo;
   }

   return 0; //inseriu
}

int RemoveNoArvoreFunil(tNoFunil **arvore, int chave){ //em caso de sucesso, retorna 0
   tNoFunil    *pE, /* Apontarah para o filho esquerdo da raiz */
                *pD, /* Apontarah para o filho direito da raiz  */
                *pS, /* Apontarah para o sucessor da raiz */
                *p;  /* Um ponteiro auxiliar */

   if (!*arvore) {
      return 1; //nao ha o q remover, arvore vazia
   }

   qOperacoes = 0;

   *arvore = Afunila(*arvore, chave); //afunila o no q quer remover

   if (chave != (*arvore)->chave) {
      return 1; //se nao estiver na raiz apos afunilamento, eh pq nao estava na arvore
   }


   /* Verifica quantos filhos a raiz possui, ela q sera removida sempre que o no existir */
   if (!(*arvore)->esquerda && !(*arvore)->direita) {
      
      free(*arvore); //nao tinha filhos, ficou vazia
      *arvore = NULL;

   } else if ((*arvore)->esquerda && (*arvore)->direita) { //dois filhos

         /* Faz pE apontar para o filho esquerdo da raiz */
      pE = (*arvore)->esquerda;

         /* Faz pD apontar para o filho direito da raiz */
      pD = (*arvore)->direita;

         /* A raiz ja pode ser liberada */
      free(*arvore);

         /* Obtem o sucessor imediato da antiga raiz */
      pS = MenorNo(pD);

         /* Afunila o sucessor imediato da antiga raiz */
      pD = Afunila(pD, pS->chave);

         /* Faz com que o filho esquerdo do sucessor */
         /* seja a arvore apontada por pE            */
      pD->esquerda = pE;

         /* Faz o ponteiro que representa a */
         /* arvore apontar para a nova raiz */
      *arvore = pD;

   } else { //raiz soh tem um filho, q sera a nova raiz

         /* Guarda o endereco da raiz para remove-la depois */
      p = *arvore;

      *arvore = (*arvore)->esquerda ? (*arvore)->esquerda : (*arvore)->direita; //nova arvore sera unico filho

      free(p); /* Remove a raiz antiga */
   }

   return 0; //remocao feita
}

int EstaVaziaArvoreFunil(tNoFunil *arvore){
   return arvore == NULL;
}


void DestroiArvoreBin(tNoFunil *arvore){
   if (arvore) {
      DestroiArvoreBin(arvore->esquerda);

      DestroiArvoreBin(arvore->direita);

      free(arvore); /* Libera a raiz */
   }
}


void ImprimeInfixaFunil(tNoFunil *arvore){
    if (!arvore)
        return;
    
    ImprimeInfixaFunil(arvore->esquerda);
    printf("%d ", arvore->chave);
    ImprimeInfixaFunil(arvore->direita);
}

void ImprimePrefixaFunil(tNoFunil *arvore){
    if (!arvore)
        return;
    
    printf("%d ", arvore->chave);
    ImprimePrefixaFunil(arvore->esquerda);
    ImprimePrefixaFunil(arvore->direita);
}

int MenorChave(tNoFunil *p){
   tNoFunil *q; //ponteiro para menor chave

   if (!p) {
      return -1;
   }
   while (p) {
      q = p;
      p = p->esquerda;
   }
   return q->chave;
}

int MaiorChave(tNoFunil *p){
   tNoFunil *q; 
   if (!p) {
      return -1;
   }
   while (p) {
      q = p;
      p = p->direita;
   }
   return q->chave;
}

int QuantOperacoesFunil(){
   return qOperacoes;
}

const tNoFunil ** ConsultaPercursoFunil(){
   return percurso;
}
