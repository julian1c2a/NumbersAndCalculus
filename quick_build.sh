#!/bin/bash

# Build Script Ultra Simple
# Uso: ./quick_build.sh [gcc|clang] [debug|release]

COMPILER=${1:-gcc}
BUILD_TYPE=${2:-release}

# Configurar compilador
if [[ "$COMPILER" == "clang" ]]; then
    CXX=clang++
else
    CXX=g++  # default gcc
fi

# Configurar flags
if [[ "$BUILD_TYPE" == "debug" ]]; then
    FLAGS="-std=c++17 -O0 -g -Wall"
    SUFFIX="_debug"
else
    FLAGS="-std=c++17 -O2 -Wall -DNDEBUG"
    SUFFIX="_release"
fi

echo "Compilando con: $CXX $FLAGS"
echo "=================================="

BUILD_DIR="build${SUFFIX}"
mkdir -p "$BUILD_DIR"

# Lista de archivos principales
FILES=(
    "comprehensive_benchmarks.cpp"
    "test_large_type_optimization.cpp"
    "optimization_benchmarks.cpp"
    "memory_benchmarks.cpp"
    "simple_benchmark.cpp"
    "test_optional_cpp17.cpp"
    "test_simple.cpp"
    "simple_test.cpp"
)

SUCCESS=0
TOTAL=0

for file in "${FILES[@]}"; do
    if [[ -f "$file" ]]; then
        ((TOTAL++))
        output="$BUILD_DIR/$(basename $file .cpp)"
        echo -n "Compilando $file ... "
        
        if $CXX $FLAGS "$file" -o "$output" 2>/dev/null; then
            echo "OK"
            ((SUCCESS++))
        else
            echo "FAILED"
        fi
    fi
done

echo "=================================="
echo "Compilados: $SUCCESS/$TOTAL"
echo "Ejecutables en: $BUILD_DIR/"

# Mostrar algunos ejecutables
if [[ $SUCCESS -gt 0 ]]; then
    echo ""
    echo "Prueba ejecutar:"
    ls "$BUILD_DIR"/* 2>/dev/null | head -3 | while read exe; do
        echo "  $exe"
    done
fi