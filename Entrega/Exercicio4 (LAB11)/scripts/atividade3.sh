#!/bin/bash

# Configurações
SRC_DIR="src"
BIN_DIR="bin"
TEST_DIR="tests"
MAIN_CLASS="atividade3"
RESULTS_FILE="$TEST_DIR/teste_atividade3.txt"

# Conjuntos de parâmetros
THREADS_LIST=(2 4 8 12)
N_LIST=(1000 10000 100000)

# Cria diretórios se não existirem
mkdir -p "$BIN_DIR" "$TEST_DIR"

# Compila o código-fonte
javac -d "$BIN_DIR" "$SRC_DIR"/"$MAIN_CLASS".java || { echo "Erro na compilação"; exit 1; }

# Limpa ou cria o arquivo de resultados
echo "=== Resultados dos testes ===" > "$RESULTS_FILE"
echo "" >> "$RESULTS_FILE"

# Loop de execuções
for N in "${N_LIST[@]}"; do
    echo "========== Números primos até $N ======================" >> "$RESULTS_FILE"
    for nthreads in "${THREADS_LIST[@]}"; do
        echo "Executando com nthreads=$nthreads" >> "$RESULTS_FILE"
        echo "-----------------------------------" >> "$RESULTS_FILE"
        for run in {1..5}; do
            echo "Execução #$run" >> "$RESULTS_FILE"
            java -cp "$BIN_DIR" "$MAIN_CLASS" "$nthreads" "$N" >> "$RESULTS_FILE" 2>&1 
            echo "" >> "$RESULTS_FILE"
        done
        echo "" >> "$RESULTS_FILE"
    done
done

echo "Todos os testes concluídos"
