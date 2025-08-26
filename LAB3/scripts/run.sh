#!/bin/bash

gcc ../atividade4_entrega/prod_int_conc.c -o ../bin/programa_concorrente.exe -Wall && \
LAB3/bin/programa_concorrente.exe 4 ../data/teste3.bin

# gcc ../atividade4_entrega/prod_int_seq.c -o ../bin/programa_sequencial.exe -Wall && \
# ../bin/programa_sequencial.exe 100 ../data/teste3.bin