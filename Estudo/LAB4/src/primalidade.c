/*
ARQUIVO COM A SOLUÇÃO DA TAREFA DE PRIMALIDADE DE FORMA CONCORRENTE
*/

/*BIBLIOTECAS*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "../../../Headers/timer.h"

/*VARIÁVEIS GLOBAIS*/
long long int max_elemento;
long long int elemento_atual = 0;
int quantidade_primos = 0;
pthread_mutex_t mutex;

double start, finish, elapsed;

/*FUNÇÕES AUXILIARES*/
//Checa a primaridade do número
int checaPrimalidade(long long int n) {
    if (n <= 1) return 0;
    if (n == 2) return 1;
    if (n%2 == 0) return 0;
    for (int i = 3; i < sqrt(n) + 1; i+=2) {
        if (n%i == 0) return 0;
    }
    return 1;
}

//Atualiza o próximo número a ser selecionado
//Garante distribuição dinâmica entre as threads
int atualizaElemento() {
    pthread_mutex_lock(&mutex);    
    long long int elemento = ++elemento_atual;
    pthread_mutex_unlock(&mutex);
    return elemento;
}

//Atualiza quantidade de primos
void atualizaPrimos() {
    pthread_mutex_lock(&mutex);
    quantidade_primos++;
    pthread_mutex_unlock(&mutex);
}

/*FUNÇÃO EXECUTADA PELAS THREADS*/
void* primo(void* arg) {
    while(elemento_atual <= max_elemento) {
        long long int elemento = atualizaElemento();
        if(checaPrimalidade(elemento) == 1) {
            atualizaPrimos();
        }
    }
    pthread_exit(NULL);
}

/*MAIN*/
int main(int argc, char* argv[]) {
    //Variáveis
    int nthreads;
    pthread_t *tid_sistema;

    //Coletando dados da entrada em linha de comando
    if (argc < 3) {
        printf("Digite: %s <elementos da sequencia> <numero de threads> \n", argv[0]);
        return -1;
    }
    max_elemento = atoll(argv[1]);
    nthreads = atoi(argv[2]);

    //Criando IDs das threads
    tid_sistema = (pthread_t*) malloc(sizeof(pthread_t) * nthreads);
    if (tid_sistema == NULL) {
        fprintf(stderr, "Erro na criação dos IDs das threads\n");
        return -2;
    }

    //Inicializando o lock
    pthread_mutex_init(&mutex, NULL);

    GET_TIME(start)
    //Criando threads
    for (long int i = 0; i < nthreads; i++) {
        if (pthread_create(&tid_sistema[i], NULL, primo, NULL)) {
            fprintf(stderr, "Erro na função pthread_create()\n");
            return -3; 
        }
    }

    //Finalizando as threads
    for (int i = 0; i < nthreads; i++) {
        if (pthread_join(tid_sistema[i], NULL)) {
            fprintf(stderr, "Erro na função pthread_join()\n");
            return -4;
        }
    }
    GET_TIME(finish)

    //Retorno
    pthread_mutex_destroy(&mutex);
    printf("Quantidade de primos = %d\n", quantidade_primos);

    elapsed = finish - start;
    printf("Time: %f'\n", elapsed);

    return 0;
}

