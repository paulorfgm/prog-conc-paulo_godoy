/*
ARQUIVO COM A SOLUÇÃO DA TAREFA DE PRIMALIDADE DE FORMA CONCORRENTE USANDO PRODUTOR/CONSUMIDOR
E SEMÁFOROS
*/

/*BIBLIOTECAS*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <semaphore.h>


/*VARIÁVEIS GLOBAIS*/
#define NUM_THREADS 2

sem_t slotCheio, slotVazio;
sem_t mutex;

int N, M, primos = 0;
int *buffer;

//! Como é só um produtor e um consumidor, posso ser um pouco mais direto na implementação

/*FUNÇÕES AUXILIARES*/
//Checa a primaridade do número
int checaPrimalidade(int n) {
    if (n <= 1) return 0;
    if (n == 2) return 1;
    if (n%2 == 0) return 0;
    for (int i = 3; i < sqrt(n) + 1; i+=2) {
        if (n%i == 0) return 0;
    }
    return 1;
}

//imprime o buffer
void printBuffer(int buffer[]) {
   for(int i=0;i<M;i++) 
      printf("%d ", buffer[i]); 
   puts("");
}

void Insere(int item) {
    static int pos = 0;
    sem_wait(&slotVazio);
    sem_wait(&mutex);
    buffer[pos] = item;
    pos = (pos + 1) % M;
    printf("Prod inseriu %d\n", item);
    printBuffer(buffer);
    sem_post(&mutex);
    sem_post(&slotCheio);
}

int Retira() {
    int item;
    static int pos = 0;
    sem_wait(&slotCheio);
    sem_wait(&mutex);
    item = buffer[pos];
    buffer[pos] = 0;
    pos = (pos + 1) % M;
    printf("Cons retirou %d\n", item);
    printBuffer(buffer);
    sem_post(&mutex);
    sem_post(&slotVazio);
    return item;
}

/*FUNÇÃO EXECUTADA PELAS THREADS*/
void* geraNumeros(void* args) {
    static int atual = 0;
    while(atual < N) {
        atual++;
        Insere(atual);
    }
    pthread_exit(NULL);
}


void* consomeNumeros(void *args) {
    static int checados = 0;
    int item;
    while(checados < N) {
        checados++;
        item = Retira();
        if(checaPrimalidade(item)) primos++; 
    }
    pthread_exit(NULL);
}


/*MAIN*/
int main(int argc, char* argv[]) {
    pthread_t *tid;

    if (argc < 3) {
        printf("ERRO: %s <N> <M> \n", argv[0]);
        return -1;
    }
    N = atoi(argv[1]);
    M = atoi(argv[2]);

    buffer = (int *) malloc(sizeof(int) * M);
    tid = (pthread_t*) malloc(sizeof(pthread_t) * NUM_THREADS);
    if (tid == NULL || buffer == NULL) {
        fprintf(stderr, "Erro na alocação de memória\n");
        return -1;
    }
    

    // Inicia os semaforos
    sem_init(&mutex, 0, 1);
    sem_init(&slotCheio, 0, 0);
    sem_init(&slotVazio, 0, M);

    pthread_create(&tid[0], NULL, geraNumeros, NULL);
    pthread_create(&tid[1], NULL, consomeNumeros, NULL);

    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_join(tid[i], NULL)) {
            fprintf(stderr, "Erro na função pthread_join()\n");
            return -1;
        }
    }

    printf("\nQuantidade de primos: %d\n", primos);

    return 0;
}

