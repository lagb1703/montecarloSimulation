#!/bin/sh
# Crear la carpeta csv si no existe
gcc -O3 -march=native -fno-math-errno pi.c -lm -o pi.out
gcc -O3 -march=native -fno-math-errno ./Buffon\'sNeedle.c -lm -o bn.out
mkdir -p csv
echo "1000000000,1500000000,2000000000,2500000000,3000000000" >csv/pi.csv
for z in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30; do
    for i in 1000000000 1500000000 2000000000 2500000000 3000000000; do
        ./pi.out $i >>csv/pi.csv
        if [ $i != 3000000000 ]; then
            echo -n "," >>csv/pi.csv
        fi
    done
    echo "" >>csv/pi.csv
done

echo "1000000000,1500000000,2000000000,2500000000,3000000000" >csv/bn.csv
for z in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30; do
    for i in 1000000000 1500000000 2000000000 2500000000 3000000000; do
        ./bn.out $i >>csv/bn.csv
        if [ $i != 3000000000 ]; then
            echo -n "," >>csv/bn.csv
        fi
    done
    echo "" >>csv/bn.csv
done