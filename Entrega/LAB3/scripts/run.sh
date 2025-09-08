#!/bin/bash
mkdir ../bin/
gcc ../src/prod_int_conc.c -o ../bin/programa_concorrente.exe -Wall && ../bin/programa_concorrente.exe 4 ../data/teste3.bin
