#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int *vetor;

//Estrutura de Dados para passar os argumentos: i, N e vetor
typedef struct {
    int id, inicio, fim;
} t_Args;

//Função auxiliar: criar vetor
int* inicializaVetor(int tamanho) {
    int* vetor = (int *) malloc(sizeof(int)*tamanho*4);
    if(vetor != NULL) {
        for (int i = 0; i < tamanho; i++) vetor[i] = i; //Podemos mudar a regra de geração aqui
    }
    return vetor;
}

//Função auxiliar: checar resultado
int checaResultado(int tamanho, int *vetor_original, int *vetor_novo) {
    for(int i = 0; i < tamanho; i++) {
        if(vetor_original[i] + 1 != vetor_novo[i]) return 1;
    }
    return 0;
}

//Função auxiliar: task (somar +1 em cada elemento do vetor)
void* incrementarVetor(void* arg) {
    t_Args args = *(t_Args*) arg;

    for(int i = args.inicio; i < args.fim; i++) {
        vetor[i]++;
    }

    free(arg);
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    //Checar quantidade de argumentos e pegar tamanho do vetor (N)
    int nthreads = 4;
    int tamanho_vetor;

    if(argc < 2) {
        printf("--ERRO: Informe o tamanho do vetor <%s> <tamanho_vetor>\n", argv[0]);
        return 1;
    }
    tamanho_vetor = atoi(argv[1]);

    //Criar vetor
    vetor = inicializaVetor(tamanho_vetor);
    if(vetor == NULL) {
        printf("--ERRO: não há memória para inicializar o vetor\n");
        return 1;
    }

    int *vetor_copia = inicializaVetor(tamanho_vetor);
    if(vetor_copia == NULL) {
        printf("--ERRO: não há memória para inicializar o vetor\n");
        return 1;
    }

    //Criar threads: a percepção dos valores que deveriam ser passados foi um pouco com a ajuda do GPT. A minha ideia inicial era fazer os programas acessarem o vetor todo, e calcularem eles mesmos o início/fim, mas não estava dando certo.
    pthread_t tid_sistema[nthreads];
    int bloco = tamanho_vetor / nthreads;
    // int resto = tamanho_vetor % nthreads; //sugestão do GPT, mas não é necessário pois n_threads = 4 e tamanho_vetor é múltiplo de 4. Então, nunca temos resto.
    int inicio = 0;

    for(int i = 0; i < nthreads; i++) {
        
        //Inicializa estrutura dos argumentos
        t_Args *args = malloc(sizeof(t_Args)); //?Imagino que precisemos criar para cada thread, pois cada thread fechará o próprio args internamente?
        if(args == NULL) {
            printf("--ERRO: malloc()\n");
            return 1;
        }

        //Passa a struct com os argumentos
        args->id = i;
        args->inicio = inicio;
        args->fim = inicio + bloco /*+ (i < resto ? 1 : 0)*/;
        inicio = args->fim; 

        //Cria as threads
        if(pthread_create(&tid_sistema[i], NULL, incrementarVetor, (void *) args)) {
            printf("--ERRO: pthread_create() da thread %d\n", i);
            return 2;
        }
    }

    //Juntar threads
    for(int i = 0; i < nthreads; i++) {
        if(pthread_join(tid_sistema[i], NULL)) {
            printf("--ERRO:  pthread_join() da thread %d\n", i);
            return 3;
        }
    }

    //Checar corretude
    if(checaResultado(tamanho_vetor, vetor_copia, vetor)) {
        printf("--ERRO DE CORRETUDE: O vetor obtido não foi o esperado!\n");
        return 4;
    }
    else {
        printf("--CORRETUDE COMPLETA: o vetor obtido foi o esperado!\n");
    }    

    printf("--Thread principal terminou\n");

    return 0;
}