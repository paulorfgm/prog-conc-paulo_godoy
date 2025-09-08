### Resultados Obtidos
---

> Para todos os testes abaixo, o código foi efetuado 5 vezes, e os valores individuais coletados, para o cálculo das médias.   
> Os testes 1, 2 e 3  foram feitos com base em tests1.bin;  
> Os testes 4, 5 e 6 foram feitos com base em tests2.bin;  
> Os testes 7, 8 e 9 foram feitos com base em tests3.bin;

---

<br>

Para rodar o código, basta checar os comandos apresentados na pasta "scripts".

1. create_vetores.sh -> cria todos os arquivos binários que serão usados como entrada dos testes.
2. run.sh -> executa uma única vez o código.
3. run_multiplo.sh -> executa 5 vezes o código descrito em "run.sh", e salva os resultados em um txt.  

<br>

Assim, foi possível analisar as métricas abaixo de forma relativamente rápida.

OBS: nunca havia usado nenhum script antes, então talvez tenha alguma coisa errada. Achei interessante para automatizar e agilizar um pouco a parte dos testes. 

Para rodar o script, navegue para a pasta "scripts" e o rode com ./run.sh (ou outro dos scripts acima) a partir do terminal!  

---

- Teste 1:
    - Dimensão do vetor: 1000 (mil)
    - Número de Threads: 1
    - Média do tempo de Execução Sequencial: 0.000001
    - Média do tempo de Execução Concorrente: 0.000348
    - Média da variação relativa do resultado: 0.000000

- Teste 2:
    - Dimensão do vetor: 1000 (mil)
    - Número de Threads: 2
    - Média do tempo de Execução Sequencial: 0.000001
    - Média do tempo de Execução Concorrente: 0.000348
    - Média da variação relativa do resultado: 0.0000004221

- Teste 3:
    - Dimensão do vetor: 1000 (mil)
    - Número de Threads: 4
    - Média do tempo de Execução Sequencial: 0.000001
    - Média do tempo de Execução Concorrente: 0.000367
    - Média da variação relativa do resultado: 0.0000005628

- Teste 4:
    - Dimensão do vetor: 10000 (10 mil)
    - Número de Threads: 1
    - Média do tempo de Execução Sequencial: 0.000018
    - Média do tempo de Execução Concorrente: 0.000216
    - Média da variação relativa do resultado: 0.000000

- Teste 5:
    - Dimensão do vetor: 10000 (10 mil)
    - Número de Threads: 2
    - Média do tempo de Execução Sequencial: 0.000018
    - Média do tempo de Execução Concorrente: 0.000211
    - Média da variação relativa do resultado: 0.0000010342

- Teste 6:
    - Dimensão do vetor: 10000 (10 mil)
    - Número de Threads: 4
    - Média do tempo de Execução Sequencial: 0.000018
    - Média do tempo de Execução Concorrente: 0.000505
    - Média da variação relativa do resultado: 0.0000003447

- Teste 7:
    - Dimensão do vetor: 100000 (100 mil)
    - Número de Threads: 1
    - Média do tempo de Execução Sequencial: 0.000176
    - Média do tempo de Execução Concorrente: 0.000559
    - Média da variação relativa do resultado: 0.000000

- Teste 8:
    - Dimensão do vetor: 100000 (100 mil)
    - Número de Threads: 2
    - Média do tempo de Execução Sequencial: 0.000176
    - Média do tempo de Execução Concorrente: 0.000380
    - Média da variação relativa do resultado: 0.0000106945

- Teste 9:
    - Dimensão do vetor: 100000 (100 mil)
    - Número de Threads: 4
    - Média do tempo de Execução Sequencial: 0.000176
    - Média do tempo de Execução Concorrente: 0.000407
    - Média da variação relativa do resultado: 0.0000114320

---