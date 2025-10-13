/*
ARQUIVO COM A SOLUÇÃO DA ATIVIDADE 1 DO LABORATÓRIO 8
*/

/*=== BIBLIOTECAS ===*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>

/*=== VARIÁVEIS GLOBAIS ===*/
#define NUM_THREADS_CONSUMIDORAS 5

long long int *vetor; 
long long int N;
int M;

int qtd_global_primos = 0;

sem_t mutex, vetorCheio, vetorVazio;

/*=== FUNÇÕES EXTRAS ===*/
int ehPrimo(long long int n) {
    int i;
    if (n<=1) return 0;
    if (n==2) return 1;
    if (n%2==0) return 0;
    for (i = 3; i < sqrt(n) + 1; i += 2)
    if(n%i == 0) return 0;
    return 1;   
} 

//Função sequencial para comparação
int sequencial() {
    int qtd_primos = 0;
    for (int i = 1; i <= N; i++) {
        if (ehPrimo(i)) qtd_primos++;
    }
    return qtd_primos;
}

/*=== FUNÇÃO DAS THREADS ===*/
void* produtora() {
    static int produzidos = 1;

    while(produzidos <= N) { 
        sem_wait(&vetorVazio);
        sem_wait(&mutex);

        for (int i = 0; i < M; i++) {

            //Caso N não seja divisível por M, no último loop de produção 
            //alcançaremos N antes de fechar o vetor com M elementos.
            //Então, basta mandar uma quantidade de sinais igual a quantidade
            //de elementos que conseguimos colocar.
            if (produzidos == (N + 1) && (i < M)) {
                for (int j = 0; j < i; j++) {
                    sem_post(&vetorCheio);
                }
                sem_post(&mutex);
                pthread_exit(NULL);
            }
            
            vetor[i] = produzidos;
            produzidos++;
        }
        
        sem_post(&mutex);

        for (int i = 0; i < M; i++) {
            sem_post(&vetorCheio);
        }

    }

    pthread_exit(NULL);
}

void* consumidora() {
    static int consumidos = 0;
    static int out = 0;
    
    int* qtd_local_primos;
    int item;

    if (!(qtd_local_primos = (int *) malloc(sizeof(int)))) {
        printf("Erro alocando quantidade local de primos na thread consumidora\n");
        exit(1);
    } 
    *qtd_local_primos = 0;

    while(consumidos < N) {
        sem_wait(&vetorCheio);
        sem_wait(&mutex);

        //Ocorreu de uma outra thread pegar o sinal que era para "ela"
        if(consumidos >= N) { 
            sem_post(&mutex);
            break;
        }

        item = vetor[out];
        out = (out + 1) % M;
        consumidos++;

        //Se voltar a ocorrer de termos out como 0, então percorremos todo o vetor
        if(out == 0) {
            sem_post(&vetorVazio);
        }

        //Pode parecer estranho, mas é para garantir que, caso ocorra a situação de consumidos > N,
        //As threads poderem sair de um possível deadlock. Mando mais sinais do que o necessário,
        //Pois as threads vão entrar na condição do break assim que chegarem, e vão terminar
        if(consumidos == N) {
            for (int i = 0; i < NUM_THREADS_CONSUMIDORAS; i++) {
                sem_post(&vetorCheio);
            }
        }

        sem_post(&mutex);

        //Calculo a primalidade fora da região crítica para poder ganhar com a concorrência
        if (ehPrimo(item)) {
            *qtd_local_primos = *qtd_local_primos + 1;
        }
    }

    sem_wait(&mutex);
    qtd_global_primos += *qtd_local_primos;
    sem_post(&mutex);
    
    pthread_exit((void *) qtd_local_primos); 
}

/*=== MAIN ===*/
int main(int argc, char* argv[]) {
    //Dados
    pthread_t *tid;

    void *ret;
    
    int id_vencedor = -1;
    int qtd_primos_vencedor = 0;
    int qtd_primos_sequencial;

    //Defesa da entrada na linha de comando
    if (argc < 3) {
        printf("Erro! Entrada esperada: %s <quantidade_inteiros> <tamanho_vetor>\n", argv[0]);
        exit(1);
    }
    else {
        N = atoll(argv[1]); 
        M = atoi(argv[2]);
    }

    //Inicialização de ponteiros
    if (!(vetor = (long long int *) malloc(sizeof(long long int) * M))) {
        printf("Erro na alocação do buffer\n");
        exit(1);
    }
    if (!(tid = (pthread_t *) malloc(sizeof(pthread_t) * (NUM_THREADS_CONSUMIDORAS + 1)))) {
        printf("Erro na alocação do vetor de ids para threads\n");
        exit(1); 
    }

    //Inicialização de semáforos
    sem_init(&mutex, 0, 1); //Semáforo binário para exclusão mútua
    sem_init(&vetorVazio, 0, 1); //Semáforo binário para a única thread produtra ativar quando o vetor estiver vazio
    sem_init(&vetorCheio, 0, 0); //Semáforo contador incializado com 0, para as threads consumidoras esperarem

    //Inicializando threads

    //-- thread produtora
    if (pthread_create(&tid[0], NULL, produtora, NULL)) {
        printf("Erro na criação da thread produtora!\n");
        exit(1);
    }

    //-- threads consumidoras
    for (int i = 1; i <= NUM_THREADS_CONSUMIDORAS; i++) {
        if (pthread_create(&tid[i], NULL, consumidora, NULL)) {
            printf("Erro na criação das threads consumidoras!\n");
            exit(1);
        }
    }

    //Juntando as threads e calculando thread vencedora
    
    //-- thread produtora
    if(pthread_join(tid[0], NULL)) {
        printf("Erro no retorno da thread produtora!\n");
        exit(1);
    }

    //-- thread consumidora
    printf("Quantidade de primos encontrada por cada Thread:\n");
    for (int i = 1; i <= NUM_THREADS_CONSUMIDORAS; i++) {
        if(pthread_join(tid[i], &ret)) {
            printf("Erro no retorno das threads consumidoras!\n");
            exit(1);
        }
        else {
            int* qtd_primos = (int*) ret;
            printf("\t-Thread [%d]: %d primos\n", i, *qtd_primos);
            if (*qtd_primos > qtd_primos_vencedor) {
                qtd_primos_vencedor = *qtd_primos;
                id_vencedor = i;
            }
            free(qtd_primos);
        }
    }
    printf("\n");

    //Chamada da função sequencial (para garantir corretude)
    qtd_primos_sequencial = sequencial();

    //Mostrando resultados
    printf("Resultados:\n\t-Quantidade total de primos (sequencial): %d\n\t-Quantidade total de primos (concorrente): %d\n\t-Thread vencedora: %d\n\n", qtd_primos_sequencial, qtd_global_primos, id_vencedor);

    //Retorno
    sem_close(&mutex);
    sem_close(&vetorVazio);
    sem_close(&vetorCheio);
    free(tid);
    free(vetor);

    return 0;
}