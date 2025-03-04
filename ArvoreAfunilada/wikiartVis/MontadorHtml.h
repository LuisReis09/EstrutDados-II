#ifndef _MontadorHtml_H_
#define _MontadorHtml_H_

#include <stdio.h>
#include "Arte.h"

extern const char *diretorioBase;

extern void IniciaMontadorHtml();
extern void ConfiguraQuantidade(int q);
extern int RecebeRequisicao(int socket, char *requisicao, int tamRequisicao, char **host);
extern void ExtraiPedido(const char *requisicao, char partesPedido[][51], int *numeroDePartes);
//extern void ExtraiPedido(const char *requisicao, char *chave, char *tipo);
extern long TamanhoArquivo(const char *arquivo);
extern long TamanhoArquivoAberto(FILE *fp);
extern void CodificaImagem(const char *arquivo, char *resultado);
extern void EnviaResposta(int socket, const tArte *arte, const char* host, const tArte *prole[], const char *logHtml, const char*tipo);

#endif