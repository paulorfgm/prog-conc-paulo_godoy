### Resultados obtidos
---

<br>

> Para cada um dos testes abaixo, o código foi rodado 5 vezes, e a média do tempo gasto foi calculada.

<br>

Para rodar o código, basta rodar ./run.sh a partir da pasta do LAB4. Ele automaticamente compila o código, e roda todos os casos de teste, que são então salvos na pasta "tests".

<br>

---

<br>

| Teste | N | Quantidade de threads | Quantidade de Primos | Tempo Gasto (segundos) |
|---|---|-----------------------|----------------------|-------------|
|Teste 1|$10^3$|1|168|0.0002524|
|Teste 2|$10^3$|2|168|0.0002384|
|Teste 3|$10^3$|4|168|0.0003218|
|Teste 4|$10^3$|8|168|0.0004002|
|Teste 5|$10^6$|1|78498|0.0893212|
|Teste 6|$10^6$|2|78498|0.0607302|
|Teste 7|$10^6$|4|78498|0.0762768|
|Teste 8|$10^6$|8|78498|0.1125466|

<br>

Tomando como base da execução sequencial os casos de teste com apenas 1 thread, temos as seguintes acelerações e eficiências:

| Teste | Aceleração | Eficiência |
|---|:---:|:---:|
|Teste 1|1|1|
|Teste 2|1.058724832|0.530|
|Teste 3|0.784338098|0.196|
|Teste 4|0.630684657|0.079|
|Teste 5|1|1|
|Teste 6|1.47074311|0.735|
|Teste 7|1.171013991|0.293|
|Teste 8|0.793637479|0.099|

<br>

Analisando os resultados da eficiência, pode-se perceber um salto bem grande quando utilizamos 2 threads, o que acaba sendo perdido com 4 ou 8 threads. Não sei exatamente qual seria a causa disso, considerando que a carga está sendo distribuída, e a parte mais complexa do problema (a função de calcular se é primo ou não) nem apresenta nenhum lock. 

<br>

    Contudo, me pergunto se utilizar um segundo mutex (um para atualizar cada variável global) seria mais eficiente.

