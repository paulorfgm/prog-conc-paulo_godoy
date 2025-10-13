# Definições iniciais, incluindo uma definição para o script parar se houver algum erro.
#!bin/bash
set -e

# Compila
mkdir -p bin
gcc "src/prod_seq.c" -o "bin/prod_seq.out" -Wall -lm

# Realizando testes
bin/./prod_seq.out 1 1
bin/./prod_seq.out 10 2
bin/./prod_seq.out 100 5
bin/./prod_seq.out 10000 20

for i in {1..10}; do 
    echo "Teste $i:"
    bin/./prod_seq.out 1 1
    echo "---------"
done > tests/teste1.txt

for i in {1..10}; do 
    echo "Teste $i:"
    bin/./prod_seq.out 10 2
    echo "---------"
done > tests/teste2.txt

for i in {1..10}; do 
    echo "Teste $i:"
    bin/./prod_seq.out 100 5
    echo "---------"
done > tests/teste3.txt

for i in {1..10}; do 
    echo "Teste $i:"
    bin/./prod_seq.out 10000 20
    echo "---------"
done > tests/teste4.txt

