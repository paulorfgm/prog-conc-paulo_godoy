/*
ARQUIVO COM A SOLUÇÃO DA ATIVIDADE 1 DO LABORATÓRIO 12
*/

/* ====== BIBLIOTECAS =============== */
import java.util.LinkedList;

/* ====== ELEMENTO COMPARTILHADO ======== */
// Armazena a quantidade total de primos para checar corretude
class Recurso {
  private int quantidade_primos; 

  public Recurso() {
    this.quantidade_primos = 0;
  }

  public synchronized void incrementa(int x) {
    this.quantidade_primos += x;
  }

  public synchronized int get() {
    return this.quantidade_primos;
  }
}
/* ====================================== */

/* ====== TAREFAS DA POOL =============== */
class FilaTarefas {
  // Dados
  private final int nThreads;
  private final MyPoolThreads[] threads;
  private final LinkedList<Runnable> queue;
  private boolean shutdown;

  // Construtor (inicializa todas as nThreads threads)
  public FilaTarefas(int nThreads) {
    this.shutdown = false;
    this.nThreads = nThreads;
    queue = new LinkedList<Runnable>();
    threads = new MyPoolThreads[nThreads];
    for (int i = 0; i < nThreads; i++) {
      threads[i] = new MyPoolThreads();
      threads[i].start();
    }
  }

  // Função que executa a ação de adicionar o runnable 
  // na fila de threads que estão aguardando.
  public void execute(Runnable r) {
    synchronized (queue) {
      if (this.shutdown)
        return;
      queue.addLast(r);
      queue.notify();
    }
  }

  // Função que finaliza toda a fila de tarefas
  public void shutdown() {
    synchronized (queue) {
      this.shutdown = true;
      queue.notifyAll(); // Notifica todos do conjunto de threads que houve desligamento completo. 
    }
    for (int i = 0; i < nThreads; i++) {
      try {
        threads[i].join();
      } catch (InterruptedException e) {
        return;
      }
    }
  }

  // Classe que descreve uma única thread, herdando de Thread
  private class MyPoolThreads extends Thread {
    public void run() {
      Runnable r;
      while (true) {
        synchronized (queue) {
          while (queue.isEmpty() && (!shutdown)) {
            try {
              queue.wait();
            } catch (InterruptedException ignored) {
            }
          }
          if (queue.isEmpty())
            return;
          r = (Runnable) queue.removeFirst(); // Se a fila não estiver vazia, removemos o item na frente
        }
        try {
          r.run(); // Exeecutamos este item (fora do synchronized da queue)
        } catch (RuntimeException e) {
        }
      }
    }
  }
}
/* ====================================== */

/* ====== RUNNABLE BASE ================= */
class Hello implements Runnable {
  String msg;

  public Hello(String m) {
    msg = m;
  }

  // método executado pela thread
  public void run() {
    System.out.println(msg);
  }
}
/* ====================================== */

/* ====== PRIMOS THREADS ================ */
class Primo implements Runnable {
  int numero;
  int eh_primo; //indica se o número selecionado é (1) ou não (0) um primo
  Recurso quantidade_primos_global;

  public Primo(int numero, Recurso qtdPrimos) {
    this.numero = numero;
    this.quantidade_primos_global = qtdPrimos;
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

  public void run() {
    this.quantidade_primos_global.incrementa(checaPrimo(numero));
    // LOG(numero, eh_primo); /* USADO APENAS PARA DEBUGAR */
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
// O programa detectará se todos os números de 1 até N são primos 
class atividade1 {
  public static void main(String[] args) {
    if (args.length < 2) {
      System.err.println("Para rodar o programa, digite: <nome_programa> <quantidade_threads> <numero_final>");
    }
    else {
      int NTHREADS = Integer.parseInt(args[0]); // quantidade de threads
      int QUANTIDADE_TAREFAS = Integer.parseInt(args[1]); // valor do último número a ser checado, N

      // Criação da pool de threads
      FilaTarefas pool = new FilaTarefas(NTHREADS);
      Recurso qtdPrimos = new Recurso();
      PrimoSequencial primoSequencial = new PrimoSequencial(QUANTIDADE_TAREFAS);
  
      // Inicia a execução dos runnables a partir da pool de threads
      // Para cada tarefa (cada i), seu runnable correspondente será 
      // Adicionado à lista e aguardará um sinal. 
      for (int i = 0; i <= QUANTIDADE_TAREFAS; i++) {
        Runnable primo = new Primo(i, qtdPrimos);
        pool.execute(primo);
      }
  
      // Aguardando término das threads. 
      pool.shutdown();
      System.out.println("Sequencial: Entre 1 e " + QUANTIDADE_TAREFAS + " há " + primoSequencial.checaPrimosSequencial() + " números primos!");
      System.out.println("Concorrente: Entre 1 e " + QUANTIDADE_TAREFAS + " há " + qtdPrimos.get() + " números primos!");
    }
  }
}
/* ====================================== */