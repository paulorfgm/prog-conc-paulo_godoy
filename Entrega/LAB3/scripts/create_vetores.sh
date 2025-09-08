#!/bin/bash
mkdir ../bin/
gcc ../src/prod_int_seq.c -o ../bin/programa_sequencial.exe -Wall && ../bin/programa_sequencial.exe 1000 ../data/teste1.bin
gcc ../src/prod_int_seq.c -o ../bin/programa_sequencial.exe -Wall && ../bin/programa_sequencial.exe 10000 ../data/teste2.bin
gcc ../src/prod_int_seq.c -o ../bin/programa_sequencial.exe -Wall && ../bin/programa_sequencial.exe 100000 ../data/teste3.bin
