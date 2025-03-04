#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h> 

#include "ArvoveAVL.h"
#include "Arte.h"
#include "MontadorHtml.h"
#include "tabelaEncadeada.h"
#include "ArvoreFunil.h"

#define PORTA 8080
#define MAX_CONEXOES 10
#define MAX_LINHA 10240

static int qArtes = 0;

//trim
void RemoveQuebraLinha(char *str){
    if (str[strlen(str) - 1] == '\n')
        str[strlen(str) - 1] = '\0';
}

//primeira letra maiuscula 
void NormalizaString(char *str){
    if ( (str[0] = isalpha(str[0]) ? toupper(str[0]) : str[0]) == '\0' ){
        return;
    }

    for(int i = 1; str[i] != '\0'; i++){
        if (str[i-1] == ' ' && isalpha(str[i])){
            str[i] = toupper(str[i]);
        }
    }
}

//altera char
void TrocaChars(char *str, char procurado, char substituto){
    for(int i = 0; str[i] != '\0'; i++){
        if (str[i] == procurado){
            str[i] = substituto;
        }
    }
}

//carrega a arvore avl
int CarregaArquivo(const char *arq, tNoAVL **arvoreAvl, tTabelaEnc **listaEncadeada, tNoFunil **arvoreFunil){
    FILE *fp;
    char linha[MAX_LINHA+1], *arquivo, *artista, *genero, *descricao, *largura, *altura, *virg, *aspas;
    int cont;
    tNoAVL *umNo;

    fp = fopen(arq, "r");
    if (!fp){
        printf("Erro ao abrir arquivo \"%s\".", arq);
        exit(1);
    }

    cont = 0;
    fgets(linha, MAX_LINHA+1, fp); //ignorar primeira linha
    while(1){
        fgets(linha, MAX_LINHA+1, fp);
        RemoveQuebraLinha(linha);
        if (feof(fp)){
            break;
        }
        virg = strchr(linha, ',');
        if (virg){
            umNo = malloc(sizeof(tNoAVL));
            if (!umNo){
                puts("Erro ao alocar nova arte para arvore.");
                exit(1);
            }

            *virg = '\0';
            arquivo = linha;
            
            artista = virg + 1;
            virg = strchr(artista, ',');
            *virg = '\0';
            NormalizaString(artista);
            
            genero = virg + 1;
            if (*genero == '\"'){
                virg = strchr(genero+1, '\"') - 2;
                *virg = '\0';    
                virg += 2;
            }else{
                virg = strchr(genero, ',') - 1;
                *virg = '\0';    
                virg += 1;
            }
            genero += 1;

            descricao = virg + 1;
            virg = strchr(descricao, ',');
            *virg = '\0';
            //printf("%s/", descricao);
            TrocaChars(descricao, '-', ' ');
            //printf("%s/", descricao);
            NormalizaString(descricao);
            //printf("%s\n", descricao);

            virg = virg + 1;
            virg = strchr(virg, ',');
            *virg = '\0';

            largura = virg + 1;
            virg = strchr(largura, ',');
            *virg = '\0';
            
            altura = virg + 1;
            virg = strchr(altura, ',');
            *virg = '\0';

            //printf("arquivo: \"%s\"\nartista: \"%s\"\ngenero: \"%s\"\ndesc: \"%s\"\nlargura: \"%s\"\naltura: \"%s\"\n---------\n", arquivo, artista, genero, descricao, largura, altura);
            cont++;
            tArte *umaArte = CriaArte(cont, artista, descricao, genero, arquivo, atoi(largura), atoi(altura));
            umNo->arte = umaArte;
            umNo->chave = cont;
            umNo->esquerda = umNo->direita = NULL;

            //testa arquivo
            char nomeArquivo[strlen(diretorioBase) + strlen(umNo->arte->nomeArquivo) + 2];
            sprintf(nomeArquivo, "%s/%s", diretorioBase, umNo->arte->nomeArquivo);
            FILE *fpTeste = fopen(nomeArquivo, "rb");
            if (!fpTeste){
                free(umNo);
                cont--;
                continue;
            }
            fclose(fpTeste);

            
            *arvoreAvl = InsereNoArvoreAVL(*arvoreAvl, umNo);
            AcrescentaElementoEnc(listaEncadeada, umaArte);
            InsereArteArvoreFunil(arvoreFunil, umaArte);
            //free(umaArte);
        }
    }

    fclose(fp);
    return cont;
}

void MontaHtmlDados(char *htmlDestino, const char partesPedido[][51], int nPartes){
    char tempHtml[1024 * 10];
    const char *tipo = partesPedido[0];
    const char *chave = partesPedido[nPartes-1];
    char tipoExtenso[51];
    int operacoes = -1;
    
    //printf("[debug] tipo=%s, chave=%s\n", tipo, chave);

    if (!strcmp(tipo, "avl") || (atoi(tipo) != 0 && strlen(tipo) > 0)){
        strcpy(tipoExtenso, "&Aacute;rvore AVL");
        operacoes = QuantOperacoesAVL();
    }else if (!strcmp(tipo, "lista")){
        strcpy(tipoExtenso, "Lista Encadeada");
        operacoes = QuantOperacoesEnc();
    }else if (!strcmp(tipo, "funil")){
        strcpy(tipoExtenso, "&Aacute;rvore Afunilada");
        operacoes = QuantOperacoesFunil();
    }

    sprintf(tempHtml, "<b><i>Percurso (%s)</i></b><br>", tipoExtenso);
    strcat(htmlDestino, tempHtml);

    if (!strcmp(tipo, "avl") || (atoi(tipo) != 0 && strlen(tipo) > 0)){
        const tNoAVL **percurso = ConsultaPercursoAvl();
        strcat(htmlDestino, "raiz: ");
        for(int i = 0; i < operacoes; i++){
            if (!percurso[i]){
                break;
            }
            sprintf(tempHtml, "<a href=\"/avl/%d\">%d</a>\n", percurso[i]->arte->id, percurso[i]->arte->id);
            strcat(htmlDestino, tempHtml);
            if (i < operacoes-1){
                strcat(htmlDestino, " -&gt; ");            
            }
        }
        strcat(htmlDestino, "<br>\n");
    }else if (!strcmp(tipo, "funil")){
        const tNoFunil **percurso = ConsultaPercursoFunil();
        strcat(htmlDestino, "raiz: ");
        for(int i = 0; i < operacoes; i++){
            if (!percurso[i]){
                break;
            }
            sprintf(tempHtml, "<a href=\"/funil/%d\">%d</a>\n", percurso[i]->arte->id, percurso[i]->arte->id);
            strcat(htmlDestino, tempHtml);
            if (i < operacoes-1){
                strcat(htmlDestino, " -&gt; ");            
            }
        }
        strcat(htmlDestino, "<br>\n");
    }
    
    sprintf(tempHtml, "N&uacute;mero de n&oacute;s visitados: %d<br><br>\n", operacoes);
    strcat(htmlDestino, tempHtml);
    
    strcat(htmlDestino, "<b><i>Sugest&otilde;es aleat&oacute;rias</i></b><br>");
    for(int i = 0; i < 15; i++){
        int umaChave = rand() % qArtes + 1;
        char link[101] = "/";

        //montar link
        for(int i = 0; i < nPartes-1; i++){
            strcat(link, partesPedido[i]);
            strcat(link, "/");
        }
        
        sprintf(tempHtml, "<a href=\"%s%d\">%d</a>\n", link, umaChave, umaChave);
        strcat(htmlDestino, tempHtml);
    }
    //strcat(htmlDestino, "<br>\n");
}

const tArte * ProcessaPedido(char *htmlDestino, const char partesPedido[][51], int nPartes, const tArte * artesSugeridas[], const tNoAVL *arvoreAvl, tTabelaEnc **lista, tNoFunil **arvoreFunil){
    for(int i = 0; i < 6; i++){
        artesSugeridas[i] = NULL;
    }

    const tArte * umaArte = NULL;

    const char * chave = partesPedido[nPartes-1];
    if (!strcmp(partesPedido[0], "avl") || (atoi(partesPedido[0]) != 0 && strlen(partesPedido[0]) > 0)){
        const tNoAVL *umNo = atoi(chave) != 0 ? BuscaArvoreBB(arvoreAvl, atoi(chave)) : NULL;

        //vizinhos (nós filhos e netos)
        if (umNo){
            artesSugeridas[0] = umNo->esquerda ? umNo->esquerda->arte : NULL;  
            artesSugeridas[1] = umNo->direita ? umNo->direita->arte : NULL; 
            umaArte = umNo->arte; 
        }
        if (artesSugeridas[0]){ //filho esquerdo
            artesSugeridas[2] = umNo->esquerda->esquerda ? umNo->esquerda->esquerda->arte : NULL;
            artesSugeridas[3] = umNo->esquerda->direita ? umNo->esquerda->direita->arte : NULL;
        }
        if (artesSugeridas[1]){ //filho direito
            artesSugeridas[4] = umNo->direita->esquerda ? umNo->direita->esquerda->arte : NULL;
            artesSugeridas[5] = umNo->direita->direita ? umNo->direita->direita->arte : NULL;
        }
    }else if (!strcmp(partesPedido[0], "funil")){
        const tNoFunil *umNo = atoi(chave) != 0 ? BuscaNoArvoreFunil(arvoreFunil, atoi(chave)) : NULL;

        //vizinhos (nós filhos e netos)
        if (umNo){
            artesSugeridas[0] = umNo->esquerda ? umNo->esquerda->arte : NULL;  
            artesSugeridas[1] = umNo->direita ? umNo->direita->arte : NULL; 
            umaArte = umNo->arte; 
        }
        if (artesSugeridas[0]){ //filho esquerdo
            artesSugeridas[2] = umNo->esquerda->esquerda ? umNo->esquerda->esquerda->arte : NULL;
            artesSugeridas[3] = umNo->esquerda->direita ? umNo->esquerda->direita->arte : NULL;
        }
        if (artesSugeridas[1]){ //filho direito
            artesSugeridas[4] = umNo->direita->esquerda ? umNo->direita->esquerda->arte : NULL;
            artesSugeridas[5] = umNo->direita->direita ? umNo->direita->direita->arte : NULL;
        }

    }else if(!strcmp(partesPedido[0], "lista")){
        const tTabelaEnc *umNo = atoi(chave) != 0 ? BuscaMovimentacaoInicioEncNo(lista, atoi(chave)) : NULL;
        //vizinhos (nós filhos e netos)
        if (umNo){
            umaArte = umNo->dado;
            for(int i = 0; i < 6; i++){
                if ( (umNo = umNo->prox) == NULL){
                    break;
                }
                artesSugeridas[i] = umNo->dado;
            }
        }
    }
    
    MontaHtmlDados(htmlDestino, partesPedido, nPartes);
    return umaArte;
}

int main(void){
    int socketServidor, socketCliente, resultado, opt = 1, contador = 0, nPartes;
    struct sockaddr_in endereco;
    char requisicao[1024*20], chave[1024], *host, estrutura[1024];
    char dadosHtml[1024*10], partesRequisicao[20][51];
    tNoAVL *arvoreAvl;
    tTabelaEnc *lista;
    tNoFunil *arvoreFunil;
    const tArte * artesSugeridas[6];

    puts("Iniciando servidor...");

    srand(time(NULL));
    IniciaArvoreAVL(&arvoreAvl);
    CriaTabelaEnc(&lista);
    IniciaArvoreFunil(&arvoreFunil);
    
    char nomeArquivo[1024];
    sprintf(nomeArquivo, "%s/classes.csv", diretorioBase);
    qArtes = CarregaArquivo(nomeArquivo, &arvoreAvl, &lista, &arvoreFunil);
    printf("Foram carregadas %d artes.\n", qArtes);
    printf("Chave raiz: %d\n", arvoreAvl->chave);

    IniciaMontadorHtml();
    ConfiguraQuantidade(qArtes);

    socketServidor = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socketServidor < 0){
        puts("Erro: Nao foi possivel criar o socket para comunicacao.");
        return 1;
    }

    endereco.sin_family = AF_INET;
    endereco.sin_port = htons(PORTA);
    endereco.sin_addr.s_addr = INADDR_ANY;
    
    resultado = bind(socketServidor, (struct sockaddr*) &endereco, sizeof(endereco));
    if (resultado < 0){
        puts("Erro: Nao foi possivel vincular o socket do servidor.");
        return 2;
    }

    resultado = listen(socketServidor, 10);
    if (resultado < 0){
        puts("Erro: Nao foi possivel preparar o socket para receber conexoes.");
        return 3;
    }

    while(1){
        puts("\nAguardando cliente...");
        socketCliente = accept(socketServidor, NULL, NULL);
        printf("Aceitando cliente #%d (socket = %d)...\n", ++contador, socketCliente);
        if (socketCliente < 0){
            puts("Erro: Nao foi possivel realizar a conexao com um cliente.");
            continue;
        }
        
        if (RecebeRequisicao(socketCliente, requisicao, sizeof(requisicao), &host) == -1){
            close(socketCliente);
            socketCliente = -1;
            puts("A requisicao nao pode ser lida corretamente, voltando...");
            continue;
        }
        
        ExtraiPedido(requisicao, partesRequisicao, &nPartes);
        printf("Cliente pediu: ");
        for(int i = 0; i < nPartes; i++){
            printf("\"%s\"%s", partesRequisicao[i], (i != nPartes-1) ? " | " : "\n");
        }
        strcpy(chave, partesRequisicao[nPartes-1]);
        printf("Chave pedida: %s\n", chave);
                
        //processar pedido
        strcpy(dadosHtml, "");
        const tArte * umaArte = ProcessaPedido(dadosHtml, partesRequisicao, nPartes, artesSugeridas, arvoreAvl, &lista, &arvoreFunil);

        //responder e fechar
        strcpy(estrutura, atoi(partesRequisicao[0]) != 0 ? "avl" : partesRequisicao[0]);
        EnviaResposta(socketCliente, umaArte, host, artesSugeridas, dadosHtml, estrutura);
        printf("Fechando conexao do cliente #%d...\n", contador);
        int r;
        r = shutdown(socketCliente, SHUT_RDWR);
        if (r){
            puts("Erro no shutdown().");
        }
        
        r = close(socketCliente);
        if (r){
            puts("Erro no close() 1.");
        }
        socketCliente = -1;
    }

    shutdown(socketServidor, SHUT_RDWR);
    close(socketServidor);

    return 0;
} 
