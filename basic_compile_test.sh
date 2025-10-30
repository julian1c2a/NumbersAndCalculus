#!/bin/bash

echo "=== Test básico de compilación ==="

cd /ucrt64/home/julian/CppProjects/AlgoritmiaCombinatoria

echo ""
echo "1. Compilando test básico con C++14:"
if /ucrt64/bin/g++ -std=c++14 basic_test.cpp -o basic_test_cpp14.exe; then
    echo "   ✓ Compilación exitosa"
    echo "   Ejecutando:"
    ./basic_test_cpp14.exe
else
    echo "   ✗ Error de compilación"
fi

echo ""
echo "2. Compilando test básico con GNU++23:"
if /ucrt64/bin/g++ -std=gnu++23 basic_test.cpp -o basic_test_gnu23.exe; then
    echo "   ✓ Compilación exitosa"
    echo "   Ejecutando:"
    ./basic_test_gnu23.exe
else
    echo "   ✗ Error de compilación"
fi