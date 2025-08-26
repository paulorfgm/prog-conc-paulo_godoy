/*
ARQUIVO COM A SOLUÇÃO DO PRODUTO INTERNO DE FORMA SEQUENCIAL
*/

// BIBLIOTECAS ===============================
#include <stdio.h>
#include <stdlib.h> 
#include <time.h>
#include "../../Headers/timer.h"

#define MAX 1000 //valor máximo de um elemento do vetor

// VARIÁVEIS GLOBAIS =========================
float *vetor_A; 
float *vetor_B;

double start, finish, elapsed;

// FUNÇÕES AUXILIARES =========================
// OBS: preferi criar funções auxiliares para ficar mais fácil

//função que recebe o tamanho de um vetor e o inicializa. Retorna NULL em caso de erro de memória.
float* inicializaVetor(int tamanho) {
    float* vetor = (float*) malloc(sizeof(float) * tamanho);
    if(!vetor) {
        fprintf(stderr, "Erro de alocao da memoria do vetor\n");
        return NULL;
    }
    return vetor;
}

//função que recebe o tamanho de um vetor e ele, e gera valores aleatórios para preenchê-lo
void geraValoresAleatorios(int tamanho, float *vetor) {
    float elemento_gerado;
    int fator = 1;
    for(long int i = 0; i < tamanho; i++) {
        elemento_gerado = (rand() % MAX)/3.0 * fator;
        vetor[i] = elemento_gerado;
        fator *= -1;
    }
}

//função que retorna o produto interno de dois vetores
float produtoInterno(int tamanho, float *vetor1, float *vetor2) {
    float somatorio = 0;
    for(int i = 0; i < tamanho; i++) {
        somatorio += vetor1[i]*vetor2[i];
    }
    return somatorio;
}

//função que recebe um vetor e seu tamanho, e mostra seus elementos na tela
void printaVetor(int tamanho, float *vetor) {
    printf("Vetor: ");
    for(int i = 0; i < tamanho; i++) {
        printf(" [%.2f] ", vetor[i]);
    }
    printf("\n");
}

// MAIN ======================================
int main(int argc, char *argv[]) {
    /*Variáveis*/
    long int tamanho_vetor;
    float produto_interno;
    FILE* arquivoSaida;
    size_t ret;

    /*Checando Argumentos de Entrada*/
    if(argc < 3) {
        fprintf(stderr, "Digite: %s <dimensao do vetor> <nome arquivo saida>\n", argv[0]);
        return 1;
    }
    tamanho_vetor = atoi(argv[1]);

    /*Inicializando os vetores A e B*/
    vetor_A = inicializaVetor(tamanho_vetor);
    if(vetor_A == NULL) return 2;

    vetor_B = inicializaVetor(tamanho_vetor);
    if(vetor_B == NULL) return 2;

    /*Preenchendo os vetores A e B com valores aleatórios*/
    srand(time(NULL));

    geraValoresAleatorios(tamanho_vetor, vetor_A);
    printaVetor(tamanho_vetor, vetor_A);

    geraValoresAleatorios(tamanho_vetor, vetor_B);
    printaVetor(tamanho_vetor, vetor_B);

    /*Calculando produto interno de forma sequencial + tomada de tempo*/
    GET_TIME(start);
    produto_interno = produtoInterno(tamanho_vetor, vetor_A, vetor_B);
    GET_TIME(finish);
    elapsed = finish - start;
    printf("\nO código levou %e segundos\n", elapsed);
    printf("\nProduto Interno: %f\n", produto_interno);

    /*Escrevendo no arquivo, usando escrita binária*/
    arquivoSaida = fopen(argv[2], "wb");
    if(!arquivoSaida) {
        fprintf(stderr, "Erro de abertura do arquivo\n");
        return 3;
    }

    ret = fwrite(&tamanho_vetor, sizeof(long int), 1, arquivoSaida);
    if(ret < 1) {
        fprintf(stderr, "Erro de escrita do tamanho do vetor\n");
        return 4;
    }

    ret = fwrite(vetor_A, sizeof(float), tamanho_vetor, arquivoSaida);
    if(ret < tamanho_vetor) {
        fprintf(stderr, "Erro de escrita no vetor A\n");
        return 4;
    }
    
    ret = fwrite(vetor_B, sizeof(float), tamanho_vetor, arquivoSaida);
    if(ret < tamanho_vetor) {
        fprintf(stderr, "Erro de escrita no vetor B\n");
        return 4;
    }

    ret = fwrite(&produto_interno, sizeof(float), 1, arquivoSaida);
    if(ret < 1) {
        fprintf(stderr, "Erro de escrita no produto interno\n");
        return 4;
    }

    ret = fwrite(&elapsed, sizeof(double), 1, arquivoSaida);
    if(ret < 1) {
        fprintf(stderr, "Erro de escrita do tempo decorrido\n");
        return 4;
    }

    /*Finalizando Programa*/
    fclose(arquivoSaida);
    free(vetor_A);
    free(vetor_B);

    return 0;
}
