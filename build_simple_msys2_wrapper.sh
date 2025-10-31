#!/bin/bash

# Build Script Simple MSYS2 - Llamada a .bat desde bash
# Uso: ./build_simple_msys2_wrapper.sh [gcc|clang|msvc] [debug|release] [cpp_version]

# Función para mostrar ayuda
show_help() {
    echo "╔══════════════════════════════════════════════╗"
    echo "║      AlgoritmiaCombinatoria Builder MSYS2    ║"
    echo "║            Wrapper para .bat                 ║"
    echo "╚══════════════════════════════════════════════╝"
    echo ""
    echo "MODO SIMPLE:"
    echo "  $0 [compilador] [tipo] [cpp_std]"
    echo ""
    echo "COMPILADORES: gcc, clang, msvc"
    echo "TIPOS: debug, release"
    echo "ESTÁNDARES C++: 14, 17, 20, 23"
    echo ""
    echo "EJEMPLOS:"
    echo "  $0 gcc debug 17"
    echo "  $0 clang release 20"
    echo "  $0 msvc release 23"
    echo ""
    echo "NOTA: Este script usa el .bat internamente para compatibilidad total"
}

# Manejar ayuda
if [[ "$1" == "-h" || "$1" == "--help" ]]; then
    show_help
    exit 0
fi

# Valores por defecto
COMPILER=${1:-gcc}
BUILD_TYPE=${2:-release}
CPP_VERSION=${3:-17}

echo "[INFO] MSYS2 -> Delegando a build_simple.bat..."
echo "Argumentos: $COMPILER $BUILD_TYPE $CPP_VERSION"
echo ""

# Ejecutar el .bat desde MSYS2
./build_simple.bat "$COMPILER" "$BUILD_TYPE" "$CPP_VERSION"

exit_code=$?

echo ""
if [[ $exit_code -eq 0 ]]; then
    echo "[SUCCESS] Build completado desde MSYS2!"
    echo ""
    echo "Ejecutables disponibles en: build/"
    echo "Ejemplos de uso desde MSYS2:"
    echo "  ./build/AlgoritmiaCombinatoria.exe"
    echo "  ./build/factorial_demo.exe"
    echo "  ./build/tests.exe"
else
    echo "[ERROR] Build falló (código: $exit_code)"
fi

exit $exit_code