# Definições iniciais, incluindo uma definição para o script parar se houver algum erro.
#!bin/bash
set -e

# Variáveis
SRC="src/primalidade.c"
BIN="bin/primalidade.out"

# Compila
mkdir -p bin
gcc "$SRC" -o "$BIN" -Wall -lm

# Executa testes
for i in {1..5}; do 
    echo "Teste $i:"
    "$BIN" 1000 1
    echo "---------"
done > tests/teste1.txt

for i in {1..5}; do 
    echo "Teste $i:"
    "$BIN" 1000 2
    echo "---------"
done > tests/teste2.txt

for i in {1..5}; do 
    echo "Teste $i:"
    "$BIN" 1000 4
    echo "---------"
done > tests/teste3.txt

for i in {1..5}; do 
    echo "Teste $i:"
    "$BIN" 1000 8
    echo "---------"
done > tests/teste4.txt

for i in {1..5}; do 
    echo "Teste $i:"
    "$BIN" 1000000 1
    echo "---------"
done > tests/teste5.txt

for i in {1..5}; do 
    echo "Teste $i:"
    "$BIN" 1000000 2
    echo "---------"
done > tests/teste6.txt

for i in {1..5}; do 
    echo "Teste $i:"
    "$BIN" 1000000 4
    echo "---------"
done > tests/teste7.txt

for i in {1..5}; do 
    echo "Teste $i:"
    "$BIN" 1000000 8
    echo "---------"
done > tests/teste8.txt