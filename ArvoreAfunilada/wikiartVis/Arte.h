#ifndef _Arte_H_
#define _Arte_H_

typedef struct rotArte {
    int id;
    char *artista;
    char *descricao;
    char *genero;
    char *nomeArquivo;
    int largura;
    int altura;
} tArte;

extern tArte * CriaArte(int id, const char *artista, const char *descricao, const char *genero, const char *nomeArquivo, int largura, int altura);
extern void LiberaArte(tArte **arte);

extern void ImprimeArte(const tArte *umaArte);
extern void ConcatenaHtml(char *destino, const tArte *umaArte, int ehLink, const char *detalheLink);

#endif