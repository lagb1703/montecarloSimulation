#!/bin/sh
# Crear la carpeta csv si no existe
gcc -O3 -march=native -fno-math-errno pi.c -lm -o pi.out
gcc -O3 -march=native -fno-math-errno ./Buffon\'sNeedle.c -lm -o bn.out
mkdir -p csv
echo "0,1000000000,2000000000,3000000000,4000000000,5000000000,6000000000" >csv/pi.csv
for z in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30; do
    for j in 2 4 8 16 32; do
        echo -n $j >>csv/pi.csv
        echo -n "," >>csv/pi.csv
    for i in 1000000000 2000000000 3000000000 4000000000 5000000000 6000000000; do
        ./pi.out $i $j >>csv/pi.csv
        if [ $i != 4000 ]; then
            echo -n "," >>csv/pi.csv
        fi
    done
    echo "" >>csv/pi.csv
    done
done

echo "0,1000000000,2000000000,3000000000,4000000000,5000000000,6000000000" >csv/bn.csv
for z in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30; do
    for j in 2 4 8 16 32; do
        echo -n $j >>csv/bn.csv
        echo -n "," >>csv/bn.csv
    for i in 1000000000 2000000000 3000000000 4000000000 5000000000 6000000000; do
        ./bn.out $i $j >>csv/bn.csv
        if [ $i != 4000 ]; then
            echo -n "," >>csv/bn.csv
        fi
    done
    echo "" >>csv/bn.csv
    done
done