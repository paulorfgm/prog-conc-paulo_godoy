/*
ARQUIVO COM A SOLUÇÃO DO PRODUTO INTERNO DE FORMA CONCORRENTE
*/

// BIBLIOTECAS ===============================
#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h> 
#include "../../Headers/timer.h"

// VARIÁVEIS GLOBAIS =========================
float *vetor_A; 
float *vetor_B;

double start, finish, elapsed_concorrente;

// ESTRUTURAS DE DADOS =======================
//Estrutura com os argumentos para cada thread
typedef struct {
    long int tamanho_vetor;
    short int nthreads;
    short int id;
} t_Args;

// FUNÇÕES AUXILIARES =========================
// OBS: preferi criar funções auxiliares para ficar mais fácil

//função que recebe o tamanho de um vetor e o inicializa. Retorna NULL em caso de erro de memória.
float* inicializaVetor(int tamanho) {
    float* vetor = (float*) malloc(sizeof(float) * tamanho);
    if(!vetor) {
        fprintf(stderr, "Erro de alocao da memoria do vetor\n");
        return NULL;
    }
    return vetor;
}

//função que calcula a variação relativa (fórmula passada no roteiro do laboratório)
float variacaoRelativa(float a, float b) {
    float var = ((a - b)/a);
    return (var >= 0 ? var : -var); 
}

// FUNÇÃO EXECUTADA PELAS THREADS ============
void *produtoInternoThread (void *args) {
    //Variáveis
    t_Args *arg = (t_Args*) args;
    int inicio, fim, fatia;
    float soma_local = 0;
    float *ret; 

    //Calculando fatias para cada thread
    fatia = arg->tamanho_vetor / arg->nthreads;
    inicio = arg->id * fatia;
    fim = arg->id == (arg->nthreads-1) ? arg->tamanho_vetor : inicio + fatia;
    
    //Operação de multiplicação elemento a elemento, e soma desses produtos
    for(int i = inicio; i < fim; i++) {
        soma_local += vetor_A[i] * vetor_B[i];
    }

    //Retorno (somatório de todos os produtos feitos na thread)
    ret = (float*) malloc(sizeof(float));
    if(ret != NULL) *ret = soma_local;
    else fprintf(stderr, "Erro no retorno da thread\n");
    pthread_exit((void*) ret);
}

// MAIN ======================================
int main(int argc, char* argv[]) {
    /*Variáveis*/
    long int tamanho_vetor;
    short int nthreads;
    float *retorno_thread;
    float valor_concorrente;
    float valor_sequencial;
    double elapsed_sequencial;
    FILE* arquivoEntrada;
    size_t ret;
    pthread_t *tid_sistema;

    /*Checando Argumentos de Entrada*/
    if(argc < 3) {
        fprintf(stderr, "Digite: %s <quantidade de threads> <nome arquivo entrada>\n", argv[0]);
        return 1;
    }
    nthreads = atoi(argv[1]);

    /*Abertura e Leitura do Arquivo de Entrada*/
    arquivoEntrada = fopen(argv[2], "rb");
    if(arquivoEntrada == NULL) {
        printf("Erro de abertura do arquivo\n");
        return 2;
    }

    ret = fread(&tamanho_vetor, sizeof(long int), 1, arquivoEntrada);
    if(!ret) {
        fprintf(stderr, "Erro de leitura das dimensões do vetor no arquivo\n");
        return 3;
    }

    /*Aloca espaços de memória e carrega os vetores de entrada*/
    vetor_A = inicializaVetor(tamanho_vetor);
    if(vetor_A == NULL) return 4;
    ret = fread(vetor_A, sizeof(float), tamanho_vetor, arquivoEntrada);
    if(ret < tamanho_vetor) {
        fprintf(stderr, "Erro de leitura dos elementos do vetor A\n");
        return 3;
    }

    vetor_B = inicializaVetor(tamanho_vetor);
    if(vetor_B == NULL) return 4;
    ret = fread(vetor_B, sizeof(float), tamanho_vetor, arquivoEntrada);
    if(ret < tamanho_vetor) {
        fprintf(stderr, "Erro de leitura dos elementos do vetor B\n");
        return 3;
    }

    /*Recupera produto interno calculado de forma sequencial*/
    ret = fread(&valor_sequencial, sizeof(float), 1, arquivoEntrada);
    if(ret < 1) {
        fprintf(stderr, "Erro na leitura do produto interno sequencial\n");
        return 3;
    }

    /*Recupera tempo de processamento do programa sequencial*/
    ret = fread(&elapsed_sequencial, sizeof(double), 1, arquivoEntrada);
    if(ret < 1) {
        fprintf(stderr, "Erro na leitura do tempo de processamento sequencial\n");
        return 3;
    }

    /*Limitando número de threads ao tamanho do vetor*/
    if(nthreads > tamanho_vetor) nthreads = tamanho_vetor;

    /*Criação das threads*/
    tid_sistema = (pthread_t*) malloc(sizeof(pthread_t) * nthreads);
    if(tid_sistema == NULL) {
        fprintf(stderr, "Erro na criação dos IDs das threads\n");
        return 5;
    }

    GET_TIME(start);
    for(long int i = 0; i < nthreads; i++) {
        t_Args *args = (t_Args*) malloc(sizeof(t_Args));
        if(args == NULL) {
            fprintf(stderr, "Erro de alocao da memoria dos argumentos para thread\n");
            return 4;
        }

        args->id = i;
        args->tamanho_vetor = tamanho_vetor;
        args->nthreads = nthreads;

        if(pthread_create(&tid_sistema[i], NULL, produtoInternoThread, (void*) args)) {
            fprintf(stderr, "Erro na função pthread_create()\n");
            return 6;
        }
    }

    /*Finalização das threads*/
    valor_concorrente = 0;
    for(int i = 0; i < nthreads; i++) {
        if (pthread_join(tid_sistema[i], (void *) &retorno_thread)) {
            fprintf(stderr, "Erro na função pthread_join\n");
            return 6;
        }
        valor_concorrente += *retorno_thread;
        free(retorno_thread);
    }
    GET_TIME(finish);
    elapsed_concorrente = finish - start;


    /*Apresentando resultados*/
    printf("\nSoma_concorrente = %.26f\n", valor_concorrente);
    printf("\nSoma-sequencial = %.26f\n", valor_sequencial);
    printf("\nVariação-relativa = %.26f\n", variacaoRelativa(valor_sequencial, valor_concorrente));
    printf("\nTempo decorrido no sequencial = %lf\n", elapsed_sequencial);
    printf("\nTempo decorrido no concorrente = %lf\n", elapsed_concorrente);


    /*Finalização do programa*/
    free(vetor_A);
    free(vetor_B);
    free(tid_sistema);
    fclose(arquivoEntrada);

    return 0;
}
