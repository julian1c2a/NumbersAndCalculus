#!/bin/bash

echo "=== Diagnóstico de errores de compilación ==="

cd /ucrt64/home/julian/CppProjects/AlgoritmiaCombinatoria

echo ""
echo "Probando compilación con C++14 (con errores detallados):"
/ucrt64/bin/g++ -std=c++14 -Wall -Wextra simple_int128_test.cpp -o test_cpp14.exe 2>&1

echo ""
echo "Probando compilación con GNU++23 (con errores detallados):"
/ucrt64/bin/g++ -std=gnu++23 -Wall -Wextra simple_int128_test.cpp -o test_gnu23.exe 2>&1