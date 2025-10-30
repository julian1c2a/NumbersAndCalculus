#!/bin/bash

echo "=== Probando soporte de __int128 en GCC ==="

cd /ucrt64/home/julian/CppProjects/AlgoritmiaCombinatoria

echo ""
echo "1. Compilando con C++14:"
if /ucrt64/bin/g++ -std=c++14 simple_int128_test.cpp -o test_cpp14.exe; then
    echo "   ✓ Compilación exitosa"
    ./test_cpp14.exe
else
    echo "   ✗ Error de compilación"
fi

echo ""
echo "2. Compilando con C++17:"
if /ucrt64/bin/g++ -std=c++17 simple_int128_test.cpp -o test_cpp17.exe; then
    echo "   ✓ Compilación exitosa"
    ./test_cpp17.exe
else
    echo "   ✗ Error de compilación"
fi

echo ""
echo "3. Compilando con C++20:"
if /ucrt64/bin/g++ -std=c++20 simple_int128_test.cpp -o test_cpp20.exe; then
    echo "   ✓ Compilación exitosa"
    ./test_cpp20.exe
else
    echo "   ✗ Error de compilación"
fi

echo ""
echo "4. Compilando con GNU++23:"
if /ucrt64/bin/g++ -std=gnu++23 simple_int128_test.cpp -o test_gnu23.exe; then
    echo "   ✓ Compilación exitosa"
    ./test_gnu23.exe
else
    echo "   ✗ Error de compilación"
fi

echo ""
echo "5. Compilando con C++23:"
if /ucrt64/bin/g++ -std=c++23 simple_int128_test.cpp -o test_cpp23.exe; then
    echo "   ✓ Compilación exitosa"
    ./test_cpp23.exe
else
    echo "   ✗ Error de compilación"
fi