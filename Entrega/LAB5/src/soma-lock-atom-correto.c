/*
ARQUIVO COM A SOLUÇÃO DA ATIVIDADE 5 DO LABORATÓRIO 5
*/

/*BIBLIOTECAS*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SOMADOR_POR_THREAD 100000
#define LIMITE_DE_IMPRESSAO 1000

/*VARIÁVEIS GLOBAIS*/
long int soma = 0;
long long int soma_maxima;

short int TERMINOU = 0;
short int IMPRIMIR = 0;

pthread_mutex_t mutex; 
pthread_cond_t cond;

/*FUNÇÕES DAS THREADS*/

//função executada pelas threads para contas
void *ExecutaTarefa(void *arg) {
  for(int i = 0; i < SOMADOR_POR_THREAD; i++) {
    //--Seção crítica
    pthread_mutex_lock(&mutex);
    while(IMPRIMIR) {
      pthread_cond_wait(&cond, &mutex);
    }
    soma++;
    if(!(soma % LIMITE_DE_IMPRESSAO)) {
      IMPRIMIR = 1;
      pthread_cond_signal(&cond);
    }
    if(soma == soma_maxima) TERMINOU = 1;
    pthread_mutex_unlock(&mutex);
    //--Seção crítica
  }
  pthread_exit(NULL);
}

//função executada pela thread de log
void *extra(void *args) {
  while(!TERMINOU) {
    pthread_mutex_lock(&mutex);
    if(!IMPRIMIR) {
      pthread_cond_wait(&cond, &mutex);
    } 
    printf("soma = %ld \n", soma);
    IMPRIMIR = 0;
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);
  }
  pthread_exit(NULL);
}

/*MAIN*/
int main(int argc, char *argv[]) {
  //Variáveis
  pthread_t *tid;
  int nthreads;

  //Coletando dados da linha de comando
  if (argc < 2) {
    printf("Digite: %s <numero de threads>\n", argv[0]);
    return 1;
  }
  nthreads = atoi(argv[1]);

  //Criando tid das threads
  tid = (pthread_t *)malloc(sizeof(pthread_t) * (nthreads + 1));
  if (tid == NULL) {
    puts("ERRO--malloc");
    return 2;
  }

  //Criando mutex e cond
  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&cond, NULL);

  soma_maxima = SOMADOR_POR_THREAD * nthreads;

  //Criando as threads
  for (long int t = 0; t < nthreads; t++) {
    if (pthread_create(&tid[t], NULL, ExecutaTarefa, (void *)t)) {
      printf("--ERRO: pthread_create()\n");
      exit(-1);
    }
  }

  //Criando a thread extra
  if (pthread_create(&tid[nthreads], NULL, extra, NULL)) {
    printf("--ERRO: pthread_create()\n");
    exit(-1);
  }

  //Esperando finalização das threads
  for (int t = 0; t < nthreads + 1; t++) {
    if (pthread_join(tid[t], NULL)) {
      printf("--ERRO: pthread_join() \n");
      exit(-1);
    }
  }

  //Finalizando programa
  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&cond);

  printf("Valor de 'soma' = %ld\n", soma);

  return 0;
}
