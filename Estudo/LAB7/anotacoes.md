### Anotações do Lab
---

- Semáforo usa a biblioteca semaphore.h
- Barreira0 está errada por dois motivos:
  - mandamos o sinal antes de tomar barreira = 0, fazendo algumas threads "atravessarem" a 2° barreira em diante
  - uma das threads pode "correr rápido" e pegar um sinal de sem_post que ainda estava sendo enviado da primeira barreira, passando pela 2° diretamente
- Barreira1 parece correta, mas como fazemos barreira-- depois de enviar o sinal, pode ocorrer de uma thread enviar um "sinal fantasma", acreditando não ser a última da sequência. 

- Na implementação da atividade 5, aproveitei que temos apenas uma thread produtora/consumidora para usar variáveis static e facilitar o código. 