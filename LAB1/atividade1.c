/*Apresentando criação de threads*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

//todas threads estão no mesmo processo, então compartilham mesmo espaço de endereçamento: variáveis globais, etc

//função que as threads executam
void* task(void* arg) {
    //void ocupa 8 bytes, então geralmente preferimos long int que também tem 8 bytes.
    long int id = (long int) arg;
    printf("Oi turma, tudo bem? Sou a thread %ld!\n", id);
    pthread_exit(NULL /*Retorno que será coletado em "pthread_join"*/);
}

//função principal
int main(int argc, char* argv[]) {
    //declaração de variáveis
    short int nthreads;
    
    //receber os argumentos de entrada e valida-los
    if(argc<2) {
        printf("ERRO: digite %s <numero threads>\n", argv[0]);
        return 1;
    }
    nthreads = atoi(argv[1]);

    //criar vetor de identificadores de threads
    pthread_t tids[nthreads];

    //criar as threads
    for (long int i = 0; i < nthreads; i++) {
        if(pthread_create(&tids[i], NULL, task, (void*) i)) { //Retorna "Verdadeiro" (1) caso tenha erro.
            printf("ERRO: pthread_create da thread %ld\n", i);
            return 2;
        };
    }

    //esperar as threads terminarem
    //neste caso não faz diferença usar "long" ou "short" int
    //função bloqueante: funções que levam o processo para o estado bloqueado, como pthread_join e scanf. impõe sincronização (forte).
    //vale a pena usar o número total de threads igual ao número de threads da sua cpu, pois se por exemplo tivermos 8 threads e 4 núcleos da cpu, vão ter sempre 4 threads esperando e 4 threads executando - gerando mais troca de contexto
    for (short int i = 0; i < nthreads; i++) {
        if(pthread_join(tids[i], NULL /*Podemos pegar retorno das threads aqui*/)) {
            printf("ERRO: pthread_join da thread %hd\n", i);
            return 3;
        }
        
    }

    //finalizar a execução
    puts("Main terminou.");

    return 0;
}