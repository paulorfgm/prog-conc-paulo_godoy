for i in {1..5}; do
    echo "Execução $i:"
    ./run.sh
    echo "----------------------"
done > ../testes/resultado_teste9.txt
