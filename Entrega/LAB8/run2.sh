# Definições iniciais, incluindo uma definição para o script parar se houver algum erro.
#!bin/bash
set -e

# Compila
mkdir -p bin
gcc "src/prod_seq.c" -o "bin/prod_seq.out" -Wall -lm

for i in {1..10}; do 
    echo "Teste $i:"
    bin/./prod_seq.out 100 7
    echo "---------"
done > tests/teste6.txt