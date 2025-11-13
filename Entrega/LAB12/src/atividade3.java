/*
ARQUIVO COM A SOLUÇÃO DA ATIVIDADE 3 DO LABORATÓRIO 12
*/

/* ====== BIBLIOTECAS =============== */
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.ArrayList;
import java.util.List;

/* ====== TAREFA THREADS ================ */
// Herança mda classe callable
class MyCallable implements Callable<Long> {
  private int numero_inicial;
  private int numero_final;

  //construtor da classe callable
  MyCallable(int n_inicial, int n_final) {
    this.numero_inicial = n_inicial;
    this.numero_final = n_final;
  }

  /* Método usado por cada thread: somatório */
  // public Long call() throws Exception {
  //   long s = 0;
  //   for (long i=1; i<=100; i++) {
  //     s++;
  //   }
  //   return s;
  // }

  // Usado para debugar
  private void LOG(int numero, int eh_primo) {
    switch(eh_primo) {
      case 0:
        System.err.println("O número [" + numero + "] não é primo");
        break;
      case 1:
        System.err.println("O número [" + numero + "] é primo");
        break;
      default:
        System.err.println("OCORREU ALGUM ERRO!\n");
        break;
    }
  }

  // Retorna se um determinado número x é primo (1) ou não (0)
  private int checaPrimo(int x) {
    if (x <= 1) return 0;
    else if (x == 2) return 1;
    else if ((x % 2) == 0) return 0;
    else {
      for (int i = 3; i < Math.sqrt(x) + 1; i++) {
        if ((x % i) == 0) return 0;
      }
    }
    return 1;
  }

  /* Método usado por cada thread: detecção de primos */
  // Checa a primalidade de todos os números em um intervalo 
  // [numero_inicial, numero_final], e retorna a quantidade de primos
  // no intervalo
  public Long call() throws Exception {
    long quantidade_primos = 0;
    for (int numero = this.numero_inicial; numero <= this.numero_final; numero++) {
      int eh_primo = checaPrimo(numero);
      quantidade_primos += eh_primo;
      // LOG(numero, eh_primo);
    }
    return quantidade_primos;
  }
}
/* ====================================== */

/* ====== PRIMOS SEQUENCIAL ============= */
class PrimoSequencial {
  private int numero_final;
  private int quantidade_primos;

  PrimoSequencial(int numero_final) {
    this.numero_final = numero_final;
    this.quantidade_primos = 0;
  }

  private int checaPrimo(int numero) {
    if (numero <= 1) return 0;
    else if (numero == 2) return 1;
    else if ((numero % 2) == 0) return 0;
    else {
      for (int i = 3; i < Math.sqrt(numero) + 1; i += 2) {
        if ((numero % i) == 0) return 0;
      }
    }
    return 1;
  }

  // Usado para debugar
  private void LOG(int numero, int eh_primo) {
    switch(eh_primo) {
      case 0:
        System.err.println("O número [" + numero + "] não é primo");
        break;
      case 1:
        System.err.println("O número [" + numero + "] é primo");
        break;
      default:
        System.err.println("OCORREU ALGUM ERRO!\n");
        break;
    }
  }

  public int checaPrimosSequencial() {
    for (int i = 0; i <= numero_final; i++) {
      int eh_primo = checaPrimo(i);
      this.quantidade_primos += eh_primo;
      // LOG(i, eh_primo); /* USADO APENAS PARA DEBUGAR */
    }
    return quantidade_primos;
  }
}
/* ====================================== */

/* ====== MAIN ========================== */
public class atividade3  {
  public static void main(String[] args) {

    if (args.length < 2) {
      System.err.println("Para rodar o programa, digite: <nome_programa> <quantidade_threads> <numero_final>");
    }
    else {
      // Dados
      int NTHREADS = Integer.parseInt(args[0]); // quantidade de threads
      int N = Integer.parseInt(args[1]); // valor do último número a ser checado 
      
      // Criando objeto PrimoSequencial para checar corretude
      PrimoSequencial primoSequencial = new PrimoSequencial(N);

      // Criação da pool de threads e de uma lista para chamadas assíncronas
      ExecutorService executor = Executors.newFixedThreadPool(NTHREADS);
      List<Future<Long>> list = new ArrayList<Future<Long>>();
  
      // Divisão de carga entre as threads
      int tamanho_fatia = (N + 1) / NTHREADS;
      int inicio, fim;
  
      // Inicia a execução das threads de forma assíncrona
      for (int i = 0; i <= NTHREADS; i++) {
        // Define o intervalo para cada thread, [inicio, fim]
        inicio = tamanho_fatia * i;
        fim = inicio + tamanho_fatia - 1;
        if (i == NTHREADS) fim = N;
  
        // Cria as threads de forma assíncrona
        Callable<Long> worker = new MyCallable(inicio, fim); // classe runnable que executa a tarefa
        Future<Long> submit = executor.submit(worker);
        list.add(submit);
      }
  
      // Recupera os resultados e faz o somatório final da quantidade de primos
      long qtdPrimos = 0;
      for (Future<Long> future : list) {
        try {
          qtdPrimos += future.get(); // bloqueia se a computação nao tiver terminado
        } catch (InterruptedException e) {
          e.printStackTrace();
        } catch (ExecutionException e) {
          e.printStackTrace();
        }
      }
      
      executor.shutdown();
      System.out.println("Sequencial: Entre 1 e " + N + " há " + primoSequencial.checaPrimosSequencial() + " números primos!");
      System.out.println("Concorrente: Entre 1 e " + N + " há " + qtdPrimos + " números primos!");
    
    }
  }
}
/* ====================================== */
