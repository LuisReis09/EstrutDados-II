

// import static java.lang.Math.round;


import java.util.function.Consumer;
import java.util.function.Predicate;


/*
    Implementacao de uma lista encadeada com saltos, que permite a insercao de elementos
    de forma aleatoria (metodo 1) ou pre-definida (metodo 2).

    CONSTRUTOR:
    ListaSaltos(int quant_maxima, char metodo)
        - recebe a quantidade maxima de elementos que a lista pode conter e o metodo de saltos a ser utilizado


    ===== METODOS DISPONIVEIS =====
    
    void inserir(T elemento)               
        - insere um elemento na lista, de acordo com o metodo escolhido
    
    void exibir()                         
        - exibe a lista
    
    void exibir(Consumer<T> funcao)
        - exibe a lista, aplicando uma funcao em cada elemento

    T elemento(int nivel, int posicao)
        - retorna o elemento na posicao especificada

    int[] posicao(Predicate<T> condicao)
        - retorna a posicao do elemento que satisfaz a condicao
    
    T elemento(Predicate<T> aceitacao, Predicate<T> continuacao)
        - retorna o primeiro elemento que satisfaz a aceitacao, seguindo a continuacao
    
    int tamanho()
        - retorna o tamanho da lista

    T[] nivel(int nivel)
        - retorna um vetor com os elementos do nivel especificado
    
    int qOperacoes()
        - retorna a quantidade de operacoes realizadas pela ultima funcao chamada
    
    void limpar()
        - limpa a lista
    
    NoSaltos<T>[] getCabeca()
        - retorna o vetor de cabecas da lista

    int getNivelMaximo()
        - retorna o nivel maximo da lista
    
    char getMetodo()
        - retorna o metodo de saltos utilizado

*/

public class ListaSaltos<T>{
    private NoSaltos<T>[] cabeca;
    private int tamanho;
    private int tamanho_maximo;
    private int nivel_maximo;

    private char[] metodo2_niveis;
    private int ind_atual = 0;

    private int qOperacoes = 0;
    private char metodo;

    @SuppressWarnings("unchecked")
    public ListaSaltos(int quant_maxima, char metodo){
        tamanho_maximo = quant_maxima;
        this.metodo = metodo;
        nivel_maximo = (int) Math.floor(Math.log(quant_maxima) / Math.log(2));

        if(metodo == 2){
            metodo2_niveis = new char[quant_maxima];
            metodo2_niveis(nivel_maximo);
        }
        
        cabeca       = new NoSaltos[nivel_maximo + 1];

        for(int i = 0; i <= nivel_maximo; i++){
            cabeca[i] = null;
        }

        tamanho = 0;
    }

    public void inserir(T elemento){
        if(metodo == 1){
            inserirMetodo1(elemento);
        }else{
            inserirMetodo2(elemento);
        }
    }

    private void inserirMetodo1(T elemento){
        if(tamanho == tamanho_maximo){
            System.out.println("Tamanho maximo atingido");
            return;
        }

        int nivel = nivel();
        NoSaltos<T> novo = new NoSaltos<T>(elemento, nivel);

        // A insercao sera sempre no inicio, para facilitar a implementacao

        for(int i = 0; i <= nivel; i++){
            if(cabeca[i] != null){
                novo.proximo[i] = cabeca[i];
            }
            cabeca[i] = novo;
        }

        tamanho++;
    }

    private void metodo2_niveis(int nivel_max){
        
        try{
            if(nivel_max == 1){
                metodo2_niveis[ind_atual++] = 0;
                metodo2_niveis[ind_atual++] = 1;
                metodo2_niveis[ind_atual++] = 0;
            }   
            else{
                metodo2_niveis(nivel_max - 1);
                metodo2_niveis[ind_atual++] = (char) nivel_max;
                metodo2_niveis(nivel_max - 1);
            }
        }catch(Exception e){
            ;
        }
    }

    public void inserirMetodo2(T elemento){
        if(tamanho == tamanho_maximo){
            System.out.println("Tamanho maximo atingido");
            return;
        }


        int nivel = metodo2_niveis[tamanho];
    
        NoSaltos<T> novo = new NoSaltos<T>(elemento, nivel);

        for(int i = 0; i <= nivel; i++){
            if(cabeca[i] != null){
                novo.proximo[i] = cabeca[i];
            }
            cabeca[i] = novo;
        }

        tamanho++;
    }

    private int nivel(){
        int nivel = 0;
        
        // Enquanto o numero aleatorio for 1, aumenta o nivel
        while((int) (Math.random() * 2) == 1 && nivel < nivel_maximo){
            nivel++;
        }

        return nivel;
    }

    public void exibir(){
        qOperacoes = 0;
        System.out.print("\n-----------------------------\n");
        for(int i = nivel_maximo - 1; i >= 0; i--){
            System.out.print("Nivel " + i + ": ");
            if(cabeca[i] == null){
                System.out.println("null");
                continue;
            }
            
            NoSaltos<T> atual = cabeca[i];
            while(atual != null){
                System.out.print(atual.elemento + " ");
                atual = atual.proximo[i];
                qOperacoes++;
            }
            System.out.println();
        }
        System.out.print("-----------------------------\n");
    }

    public void exibir(Consumer<T> funcao){
        qOperacoes = 0;
        for(int i = nivel_maximo - 1; i >= 0; i--){
            System.out.print("\n-----------------------------\n");
            System.out.println("Nivel " + i + ": ");
            if(cabeca[i] == null){
                System.out.println("null");
                continue;
            }
            
            NoSaltos<T> atual = cabeca[i];
            while(atual != null){
                funcao.accept(atual.elemento);
                atual = atual.proximo[i];
                qOperacoes++;
            }
        }
        System.out.print("\n-----------------------------\n");
    }

    public T elemento(int nivel, int posicao){
        qOperacoes = 0;
        NoSaltos<T> atual = cabeca[nivel];
        int i = 0;

        while(atual != null){
            qOperacoes++;
            if(i == posicao){
                return atual.elemento;
            }

            atual = atual.proximo[nivel];
            i++;
        }

        return null;
    }

    public int[] posicao(Predicate<T> condicao){
        // Retorna um vetor com o nivel e a posicao do elemento, com base na condicao proposta
        qOperacoes = 0;

        int[] ans = new int[2];
        ans[0] = -1;
        ans[1] = -1;

        for(int i = nivel_maximo - 1; i >= 0; i--){
            NoSaltos<T> atual = cabeca[i];
            int j = 0;

            while(atual != null){
                qOperacoes++;

                if(condicao.test(atual.elemento)){
                    ans[0] = i;
                    ans[1] = j;
                    return ans;
                }

                atual = atual.proximo[i];
                j++;
            }
        }

        return ans;
    }

    public T elemento(Aceitacao<T> aceitacao, Predicate<T> continuacao){
        
        /*
            Continuacao deve ser uma funcao que retorna true se a procura deve seguir
            para a proxima chave (no mesmo nivel), e false caso contrario (ou seja, tenha que descer um nivel)
            
            Aceitacao deve ser uma funcao que retorna true se o elemento for aceito, e false caso contrario
        */
        qOperacoes = 0;

        NoSaltos<T> atual = cabeca[nivel_maximo - 1];
        NoSaltos<T> ultimo = atual;

        for(int i = nivel_maximo - 1; i >= 0; i--){
            
            // Enquanto tiver elemento no nivel
            while(atual != null){
                qOperacoes++;

                System.out.println("Procurando " + atual.elemento + " no nivel " + i);

                // Se nao puder continuar:
                if(!continuacao.test(atual.elemento)){

                    // Verifica se o elemento atual eh aceito
                    if(aceitacao.aceitar(atual.elemento)){
                        return atual.elemento;
                    }

                    // Se nao for aceito, desce um nivel, usando o ultimo elemento pra continuar como cabeca
                    if(i > 0)
                        atual = ultimo.proximo[i-1];
                    else
                        atual = null;
                    break;
                }

                ultimo = atual;
                atual = atual.proximo[i];

                if(atual == null && i>0){
                    atual = ultimo.proximo[i-1];
                }
            }
        }

        System.out.println("Elemento nao encontrado");
        return null;
    }

    public int tamanho(){
        return tamanho;
    }


    public T[] nivel(int nivel){
        // Retorna um vetor com os elementos do nivel especificado
        if(nivel < 0 || nivel > nivel_maximo){
            return null;
        }
        qOperacoes = 0;
        ListaIndexada<T> lista = new ListaIndexada<T>();
        NoSaltos<T> atual = cabeca[nivel];

        lista.redimensionar(tamanho);

        while(atual != null){
            qOperacoes++;
            lista.inserir(atual.elemento);
            atual = atual.proximo[nivel];
        }

        return lista.toArray();
    }

    public int qOperacoes(){
        return qOperacoes;
    }

    public void limpar(){
        qOperacoes = 0;
        NoSaltos<T> atual = cabeca[0];
        NoSaltos<T> proximo = null;

        while(atual != null){
            proximo = atual.proximo[0];
            atual = null;
            atual = proximo;
            qOperacoes++;
        }

        for(int i = 0; i <= nivel_maximo; i++){
            cabeca[i] = null;
        }

        tamanho = 0;
        System.gc();
    }

    public NoSaltos<T>[] getCabeca(){
        return cabeca;
    }

    public int getNivelMaximo(){
        return nivel_maximo;
    }

    public char getMetodo(){
        return metodo;
    }
}
