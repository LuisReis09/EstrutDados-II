#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h> 

#include "MontadorHtml.h"

//para codificar as imagens em base64
static char *BASE64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static void base64_encode(const char *in, const unsigned long in_len, char *out) {
  int in_index = 0;
  int out_index = 0;

  while (in_index < in_len) {
    // process group of 24 bit

    // first 6-bit
    out[out_index++] = BASE64[ (in[in_index] & 0xFC) >> 2 ];

    if ((in_index + 1) == in_len) {
      // padding case n.1
      //
      // Remaining bits to process are the right-most 2 bit of on the
      // last byte of input. we also need to add two bytes of padding
      out[out_index++] = BASE64[ ((in[in_index] & 0x3) << 4) ];
      out[out_index++] = '=';
      out[out_index++] = '=';
      break;
    }

    // second 6-bit
    out[out_index++] = BASE64[ ((in[in_index] & 0x3) << 4) | ((in[in_index+1] & 0xF0) >> 4) ];

    if ((in_index + 2) == in_len) {
      // padding case n.2
      //
      // Remaining bits to process are the right most 4 bit on the
      // last byte of input. We also need to add a single byte of
      // padding.
      out[out_index++] = BASE64[ ((in[in_index + 1] & 0xF) << 2) ];
      out[out_index++] = '=';
      break;
    }

    // third 6-bit
    out[out_index++] = BASE64[ ((in[in_index + 1] & 0xF) << 2) | ((in[in_index + 2] & 0xC0) >> 6) ];

    // fourth 6-bit
    out[out_index++] = BASE64[ in[in_index + 2] & 0x3F ];

    in_index += 3;
  }

  out[out_index] = '\0';
  return;
}

static int is_socket_connected(int sockfd) { 
    char buffer; 
    int result = recv(sockfd, &buffer, sizeof(buffer), MSG_DONTWAIT); 
    if (result == 0) { 
        return 0; // Socket is closed 
    } else if (result < 0) { 
        int error = 0;
        socklen_t len = sizeof (error);
        int retval = getsockopt (sockfd, SOL_SOCKET, SO_ERROR, &error, &len);
        //printf("error=%d, retval=%d\n", error, retval);

        return  (error != 0 || retval != 0) ? 0 : 1;//(errno == E || errno == EWOULDBLOCK) ? 1 : 0; // Check for non-blocking error 
    } 
    return 1; // Socket is connected 
} 

static int iniciado = 0;

static char *conteudoPagina = NULL;
static char *conteudoArtePrincipal = NULL;
static char *conteudoArteRelacionada = NULL;

static int qArtes = 0;

const char *diretorioBase = "../../../../turma 2024.2/compartilhamentos/wikiart";;

void IniciaMontadorHtml(){
    if (iniciado)
        return;
    
    //pagina principal
    FILE *fp = fopen("modeloPagina.html", "r");
    if (!fp){
        printf("Erro ao tentar abrir modelo da pagina.\n");
        exit(1);
    }
    long t = TamanhoArquivoAberto(fp);
    if ( (conteudoPagina = malloc(t)) == NULL ){
        printf("Erro ao tentar alocar conteudo da pagina.\n");
        exit(1);
    }
    long lido = 0, r;
    while(lido < t){
        r = fread(conteudoPagina + lido, 1, t - lido, fp);
        lido += r;
    }
    fclose(fp);

    //arte principal
    fp = fopen("modeloArtePrincipal.html", "r");
    if (!fp){
        printf("Erro ao tentar abrir modelo da arte principal.\n");
        exit(1);
    }
    t = TamanhoArquivoAberto(fp);
    if ( (conteudoArtePrincipal = malloc(t)) == NULL ){
        printf("Erro ao tentar alocar conteudo da arte principal.\n");
        free(conteudoPagina);
        exit(1);
    }
    lido = 0;
    while(lido < t){
        r = fread(conteudoArtePrincipal + lido, 1, t - lido, fp);
        lido += r;
    }
    fclose(fp);

    //arte relacionada
    fp = fopen("modeloArteRelacionada.html", "r");
    if (!fp){
        printf("Erro ao tentar abrir modelo da arte relacionada.\n");
        exit(1);
    }
    t = TamanhoArquivoAberto(fp);
    if ( (conteudoArteRelacionada = malloc(t)) == NULL ){
        printf("Erro ao tentar alocar conteudo da arte relacionada.\n");
        free(conteudoArtePrincipal);
        free(conteudoPagina);
        exit(1);
    }
    lido = 0;
    while(lido < t){
        r = fread(conteudoArteRelacionada + lido, 1, t - lido, fp);
        lido += r;
    }
    fclose(fp);

    iniciado = 1;
}

void ConfiguraQuantidade(int q){
    qArtes = q;
}

//le a requisicao http ate o fim
int RecebeRequisicao(int socket, char *requisicao, int tamRequisicao, char **host){
    int lido = 0, r;

    while(lido <= tamRequisicao){
        r = read(socket, requisicao + lido, 1024);
        if (r <= 0){
            printf("Erro lendo requisicao!\n");
            return r;
        }
        lido += r;

        if (lido >= 4){
            requisicao[lido] = '\0';
            if (strcmp(&requisicao[lido-4], "\r\n\r\n") == 0){
                //separar o host?
                if (host != NULL){
                    char *posHost = strstr(requisicao, "Host:");
                    if (posHost){
                        posHost += 6;
                        char *atual = posHost;
                        while( (*atual >= '0' && *atual <= '9') || *atual == '.' ){
                            atual++;
                        }
                        char *fim = strstr(posHost, "\r\n");
                        *fim = '\0';
                        *host = posHost;
                    }
                }
                return lido;
            }
        }
    }

    return -1;
}

int ContaOcorrenciasChar(const char *str, char c){
    int contador = 0;

    for(int i = 0; str[i] != '\0'; i++){
        if (str[i] == c){
            contador++;
        }
    }

    return contador;
}

//preenche a chave com o que foi pedido na requisicao http
void ExtraiPedido(const char *requisicao, char partesPedido[][51], int *numeroDePartes){ 
    char linhaReq[1024], *linhaTratada = NULL;
    sscanf(requisicao, "%*s%s%*s", linhaReq);
    
    //tirar barra da frente e de tras 
    char *c = linhaReq;
    while (*c == '/'){
        c++;
    }
    linhaTratada = c;
    c = linhaTratada + strlen(linhaTratada) - 1;
    while(*c == '/'){
        c--;
    }
    *(++c) = '\0';

    int nPartes = ContaOcorrenciasChar(linhaTratada, '/') + 1;
    *numeroDePartes = nPartes;
    //printf("[debug] req original: %s\n", linhaReq);
    //printf("[debug] linha usada (%d partes): %s\n", nPartes, linhaTratada);

    const char *separador = "/";
    char *tok = strtok(linhaReq, separador);
    int iParte = 0;
    while( tok != NULL ) {
        strcpy(partesPedido[iParte++], tok);
        tok = strtok(NULL, separador);
    }
        
    /*
    int iPedido = 0;
    int iRequisicao = 5;

    while(1){
        chave[iPedido] = requisicao[iRequisicao];
        if (chave[iPedido] == ' ' | chave[iPedido] == '/'){
            break;
        }
        iPedido++;
        iRequisicao++;
    }
    chave[iPedido] = '\0';

    iPedido = 0;
    if (requisicao[iRequisicao] == '/'){
        //tipo
        iRequisicao++;
        while(1){
            tipo[iPedido] = requisicao[iRequisicao];
            if (tipo[iPedido] == ' ' | tipo[iPedido] == '/'){
                break;
            }
            iPedido++;
            iRequisicao++;
        }
    }
    tipo[iPedido] = '\0';
    */
}

/*
void ExtraiPedido(const char *requisicao, char *chave, char *tipo){ 
    int iPedido = 0;
    int iRequisicao = 5;

    while(1){
        chave[iPedido] = requisicao[iRequisicao];
        if (chave[iPedido] == ' ' | chave[iPedido] == '/'){
            break;
        }
        iPedido++;
        iRequisicao++;
    }
    chave[iPedido] = '\0';

    iPedido = 0;
    if (requisicao[iRequisicao] == '/'){
        //tipo
        iRequisicao++;
        while(1){
            tipo[iPedido] = requisicao[iRequisicao];
            if (tipo[iPedido] == ' ' | tipo[iPedido] == '/'){
                break;
            }
            iPedido++;
            iRequisicao++;
        }
    }
    tipo[iPedido] = '\0';
}
*/

//informa o tamanho de um arquivo no disco
long TamanhoArquivo(const char *arquivo){
    FILE *fp = fopen(arquivo, "rb");
    if (!fp){
        printf("Erro ao abrir arquivo para calcular tamanho: \'%s\'.", arquivo);
        return -1;
    }

    fseek(fp, 0, SEEK_END);
    long tamanhoArquivo = ftell(fp);
    
    fclose(fp);

    return tamanhoArquivo;
}

//informa o tamanho de um arquivo no disco
long TamanhoArquivoAberto(FILE *fp){
    if (!fp){
        printf("Erro ao usar arquivo nulo para calcular tamanho.");
        return -1;
    }

    long posOriginal = ftell(fp);
    fseek(fp, 0, SEEK_END);
    long tamanhoArquivo = ftell(fp);
    fseek(fp, posOriginal, SEEK_SET);
    
    return tamanhoArquivo;
}

//le um arquivo do disco e coloca a codificacao base64 dele em resultado
void CodificaImagem(const char *arquivo, char *resultado){
    FILE *fp = fopen(arquivo, "rb");
    if (!fp){
        printf("Erro ao abrir arquivo para codificacao: \'%s\'.", arquivo);
        exit(1);
    }

    long tamanhoArquivo = TamanhoArquivoAberto(fp);
    //printf("[debug] tam arquivo: %ld\n", tamanhoArquivo);
    char * conteudoArquivo = malloc(tamanhoArquivo);
    if (!conteudoArquivo){
        puts("Erro ao alocar buffer para o conteudo do arquivo.");
        exit(1);
    }
    
    long lido = 0, r;
    while(lido < tamanhoArquivo){
        r = fread(conteudoArquivo + lido, 1, tamanhoArquivo-lido, fp);
        lido += r;
    }

    base64_encode(conteudoArquivo, tamanhoArquivo, resultado);
    //printf("[debug] imagem codificada em %ld bytes.\n",  strlen(resultado));
    
    free(conteudoArquivo);
    fclose(fp);
}

//responde ao cliente o que foi pedido
void EnviaResposta(int socket, const tArte *arte, const char* host, const tArte *prole[], const char *logHtml, const char*tipo){
    //verificar pedido
    int chave = arte ? arte->id : 0;
    int resposta404 = 0;

    //montar html da resposta (parte interna dinamica)
    char *resposta = NULL;
    char *artePrincipal = NULL;
    char *arteRelacionada[6] = {NULL};
    char log[1024*20] = "";

    if (chave == 0 && arte == NULL){
        resposta = malloc(1024); 
        if (!resposta){
            puts("Erro ao alocar buffer para resposta raiz.");
            exit(1);
        }
        
        int umaChave = rand() % qArtes + 1;
        sprintf(resposta, "<html><head><title>EDAII - WikiArt</title></head><body><p>Informe uma chave, por exemplo, <b><a href=\"http://%s/%d\">%d</a></b></p></body></html>\n", host, umaChave, umaChave);    
        //printf("[debug] montada resposta raiz com tamanho %ld.\n", strlen(resposta));
    }else{
        if (!arte){
            resposta404 = 1;
            resposta = malloc(1024);
            if (!resposta){
                puts("Erro ao alocar buffer para resposta de erro.");
                exit(1);
            }
            //printf("[debug] resposta principal alocada (%p).\n", respostaPrincipal);

            sprintf(resposta, "Chave <b>%d</b> n&atilde;o encontrada.<br>\n", chave);
            //printf("[debug] montada resposta de arte nao encontrada com tamanho %ld.\n", strlen(resposta));
        }else{
            printf("Arte pedida: %s, %s\n", arte->descricao, arte->artista);
            char nomeArquivo[strlen(diretorioBase) + strlen(arte->nomeArquivo) + 2];
            sprintf(nomeArquivo, "%s/%s", diretorioBase, arte->nomeArquivo);
            //printf("arq: %s\n", nomeArquivo);

            //montar div da arte principal
            artePrincipal = malloc(TamanhoArquivo(nomeArquivo) * 2);
            if (!artePrincipal){
                printf("Erro ao alocar para html da arte principal.");
                exit(1);
            }
            strcpy(artePrincipal, conteudoArtePrincipal);
            //printf("[debug] arte principal alocada (%p/%ldb).\n", artePrincipal, TamanhoArquivo(nomeArquivo) * 2);
            
            char *posBase64 = strstr(artePrincipal, "[imgdata]");
            *posBase64 = '\0';
            const char *aposBase64Original = strstr(conteudoArtePrincipal, "[imgdata]") + strlen("[imgdata]");
            CodificaImagem(nomeArquivo, posBase64);
            //printf("[debug] codificacao: %s (%s)\n", posBase64, nomeArquivo);
            strcpy(artePrincipal + strlen(artePrincipal), aposBase64Original);

            char *posBanner = strstr(artePrincipal, "[banner]");
            *posBanner = '\0';
            const char *aposBannerOriginal = strstr(conteudoArtePrincipal, "[banner]") + strlen("[banner]");
            ConcatenaHtml(posBanner, arte, 0, tipo);
            strcpy(artePrincipal + strlen(artePrincipal), aposBannerOriginal);
            
            //montar divs das artes relacionadas
            for(int i = 0; i < 6; i++){
                char nomeArquivo[strlen(diretorioBase) + (prole[i] ? strlen(prole[i]->nomeArquivo) : 2) + 2];
                sprintf(nomeArquivo, "%s/%s", diretorioBase, prole[i] ? prole[i]->nomeArquivo : "");
                
                arteRelacionada[i] = malloc(prole[i] ? TamanhoArquivo(nomeArquivo) * 2: 1024);
                if (!arteRelacionada[i]){
                    printf("Erro ao alocar para html da arte filha.");
                    exit(1);
                }
                strcpy(arteRelacionada[i], conteudoArteRelacionada);

                char *posBase64 = strstr(arteRelacionada[i], "[imgdata]");
                *posBase64 = '\0';
                const char *aposBase64Original = strstr(conteudoArteRelacionada, "[imgdata]") + strlen("[imgdata]");
                if (prole[i])
                    CodificaImagem(nomeArquivo, posBase64);
                strcpy(arteRelacionada[i] + strlen(arteRelacionada[i]), aposBase64Original);
    
                char *posBanner = strstr(arteRelacionada[i], "[banner]");
                *posBanner = '\0';
                const char *aposBannerOriginal = strstr(conteudoArteRelacionada, "[banner]") + strlen("[banner]");
                if (prole[i])
                    ConcatenaHtml(posBanner, prole[i], 1, tipo);
                strcpy(arteRelacionada[i] + strlen(arteRelacionada[i]), aposBannerOriginal);
                //printf("[debug] ar%d: %s\n", i, arteRelacionada[i]);
                //printf("[debug] arte relacionada #%d alocada (%p/%ldb).\n", i, artePrincipal, TamanhoArquivo(nomeArquivo) * 2);
            }

            sprintf(log, "\n<b>%s</b> | %s<br>\n"
                         "<i>%s</i><br><br><hr>\n", arte->descricao, arte->artista, arte->genero);
        }
    }

    //montar pagina principal / resposta
    char *html = NULL;
    int tamanhoHTML = strlen(conteudoPagina) + 1024*10;
    if (resposta != NULL){
        //ja tem, talvez um erro
        tamanhoHTML += strlen(resposta) + 1;
        html = resposta;
    }else{
        tamanhoHTML += strlen(artePrincipal) + 1;
        for(int i = 0; i < 6; i++){
            tamanhoHTML += strlen(arteRelacionada[i]) + 1;
        }
        tamanhoHTML += strlen(log) + 1;
        tamanhoHTML += strlen(logHtml) + 1;
    }
    printf("Montar HTML em %d bytes.\n", tamanhoHTML);

    if (html == NULL){
        html = malloc(tamanhoHTML);
        if (!html){
            puts("Erro ao alocar html da resposta.");
            exit(1);
        }
        
        //montar pagina principal
        strcpy(html, conteudoPagina);

        //TODO lincoln: issoprecisa ser revisado, as concatenacoes quando tem mtos campos
        
        char *posArtePrincipal = strstr(html, "[arte-principal]");
        const char *aposArtePrincipalOriginal = strstr(conteudoPagina, "[arte-principal]") + strlen("[arte-principal]");
        strcpy(posArtePrincipal, artePrincipal);
        strcpy(html + strlen(html), aposArtePrincipalOriginal);
        free(artePrincipal);

        int iArteRelacionada = 0;
        for(int p = 1; p <= 2; p++){
            char tag[51];
            sprintf(tag, "[arte-relacionada-%d]", p);
            char *posArteRelacionada = strstr(html, tag);
            const char *aposArteRelacionadaOriginal = strstr(conteudoPagina, tag) + strlen(tag);
            //printf("[debug] apos-tag-%d: %s\n", p, aposArteRelacionadaOriginal);
            for(int i = 0; i < 2*p; i++){
                *posArteRelacionada = '\0';
                //printf("[debug] ar%d: %s\n", iArteRelacionada, arteRelacionada[iArteRelacionada]);
                strcat(posArteRelacionada, arteRelacionada[iArteRelacionada]);
                posArteRelacionada += strlen(posArteRelacionada);

                free(arteRelacionada[iArteRelacionada]);
                iArteRelacionada++;
            }
            strcpy(html + strlen(html), aposArteRelacionadaOriginal);
        }
        //printf("[debug] aposfinal2: %s\n", html);

        char *posLog = strstr(html, "[texto-log]");
        *posLog = '\0';
        const char *aposLogOriginal = strstr(conteudoPagina, "[texto-log]") + strlen("[texto-log]");
        strcat(log, logHtml);
        strcat(html, log);
        strcpy(html + strlen(html), aposLogOriginal);
    }

    //monta protocolo
    char cabecalho[1024], linhaTamanho[51];
    strcpy(cabecalho, "HTTP/1.1 200 OK\r\n");
    if (resposta404 > 0){
        strcpy(cabecalho, "HTTP/1.1 404 Not Found\r\n");
        //sprintf(linhaTamanho, "Content-Length: %d\r\n", tamFavicon);
        //strcat(cabecalho, "Content-Type: image/png\r\n");
    }else{
        strcpy(cabecalho, "HTTP/1.1 200 OK\r\n");
        sprintf(linhaTamanho, "Content-Length: %lu\r\n", strlen(html));
        strcat(cabecalho, linhaTamanho); 
        strcat(cabecalho, "Cache-Control: no-cache, no-store, must-revalidate\r\n");
        strcat(cabecalho, "Expires: 0\r\n");
        strcat(cabecalho, "Pragma: no-cache\r\n");
        strcat(cabecalho, "Content-Type: text/html; charset=UTF-8\r\n");
    }
    strcat(cabecalho, "Connection: close\r\n\r\n");

    //envia cabecalho e conteudo
    int enviado = 0, enviar = strlen(cabecalho), r;
    //printf("[debug] cab: %s\n", cabecalho);
    printf("Enviar %d bytes do cabecalho...\n", enviar);
    while(enviado < enviar && is_socket_connected(socket)){
        //r = send(socketCliente, cabecalho + enviado, enviar - enviado, 0);
        r = write(socket, cabecalho + enviado, enviar - enviado);
        //printf("%d enviados...\n", r);
        if (r <= 0){
            puts("Erro ao enviar.");
            exit(1);
        }
        enviado += r;
    }

    enviado = 0;
    char *bufferEnvio = html;
    enviar = strlen(html);
    //printf("[debug] html: %s\n", html);
    printf("Enviar %d bytes do corpo...\n", enviar);
    while(enviado < enviar && is_socket_connected(socket)){
        //printf("enviar %p %d %d\n", bufferEnvio+enviado, enviar-enviado, socketCliente);    
        //r = send(socketCliente, bufferEnvio + enviado, enviar-enviado, MSG_DONTWAIT);
        r = write(socket, bufferEnvio + enviado, enviar - enviado);

        printf("Enviados %d bytes do corpo... (%d/%d)\n", r, enviado + r, enviar);
        if (r <= 0){
            puts("Erro ao enviar.");
            //exit(1);
            break;
        }
        enviado += r;
    }   


    free(html);
    //printf("[debug] resposta HTML liberada.\n");
}
