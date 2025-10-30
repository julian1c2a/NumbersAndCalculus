#!/bin/bash
cd /c/msys64/ucrt64/home/julian/CppProjects/AlgoritmiaCombinatoria
g++ -std=gnu++23 -Wall -O2 complete_test_int128_io.cpp -o complete_test.exe
if [ $? -eq 0 ]; then
    echo "Compilación exitosa. Ejecutando..."
    ./complete_test.exe
else
    echo "Error en la compilación"
fi