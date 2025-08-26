/*Apresentando como passar mais de um argumento para uma thread*/

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

typedef struct {
    int idThread, nThreads;
} t_Args;

//funcao executada pelas threads
void *PrintHello (void *arg) {
    t_Args args = *(t_Args*) arg;
    //log da thread
    
    printf("Sou a thread %d de %d threads\n", args.idThread, args.nThreads);

    free(arg); //alocado na main
    
    pthread_exit(NULL);
}

//--funcao principal do programa
int main(int argc, char* argv[]) {
  t_Args *args; //receberá os argumentos para a thread
   int nthreads; //qtde de threads que serao criadas (recebida na linha de comando)
   
   //verifica se o argumento 'qtde de threads' foi passado e armazena seu valor
   if(argc<2) { 
      printf("--ERRO: informe a qtde de threads <%s> <nthreads>\n", argv[0]); 
      return 1;
   }
   nthreads = atoi(argv[1]);

   //identificadores das threads no sistema
   pthread_t tid_sistema[nthreads]; 
  
   //cria as threads e atribui a tarefa de cada thread
   for(int i=0; i<nthreads; i++) {
        printf("--Aloca e preenche argumentos para thread %d\n", i);
        args = malloc(sizeof(t_Args));
    if (args == NULL) {
      printf("--ERRO: malloc()\n"); 
      return 2;
    }
    args->idThread = i; 
    args->nThreads = nthreads; 

      if (pthread_create(&tid_sistema[i], NULL, PrintHello, (void*) args)) {
      printf("--ERRO: pthread_create() da thread %d\n", i); 
         return 2;
      }
   }

   //log da funcao main
   printf("--Thread principal terminou\n");

   pthread_exit(NULL); //necessario para que o processo não seja encerrado antes das threads terminarem
}
