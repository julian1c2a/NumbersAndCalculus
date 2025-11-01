#!/bin/bash

# AlgoritmiaCombinatoria Builder - Version Bash Equivalente
# Uso: ./build_equivalent.sh [compilador] [tipo] [estandar] 
# Equivalente exacto del script .bat funcional

set -e  # Salir en caso de error

# Colores para output
RED='\033[0;31m'
GREEN='\033[0;32m' 
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Verificar ayuda
if [[ "$1" == "-h" || "$1" == "--help" || "$1" == "/?" ]]; then
    echo ""
    echo "================================================"
    echo "        AlgoritmiaCombinatoria Builder        "
    echo "             Modo Simple v2.1                "
    echo "            (Version Bash/Unix)               "
    echo "================================================"
    echo ""
    echo "Uso: usando CMake y ninja no se puede arreglar?"
    echo ""
    echo "SINTAXIS: $0 [compilador] [tipo] [estandar]"
    echo ""
    echo "COMPILADORES: gcc, clang, msvc"
    echo "TIPOS: debug, release"
    echo "ESTANDARES C++: 14, 17, 20, 23"
    echo ""
    echo "EJEMPLOS:"
    echo "  $0 gcc debug 17      # GCC Debug C++17"
    echo "  $0 clang release 20  # Clang Release C++20"  
    echo "  $0 msvc release 23   # MSVC Release C++23"
    echo "  $0                   # GCC Release C++17 (por defecto)"
    echo ""
    exit 0
fi

# Configurar valores por defecto (identicos al .bat)
COMPILER="${1:-gcc}"
BUILD_TYPE="${2:-release}"
CPP_STANDARD="${3:-17}"

# Normalizar build type (identico al .bat)
case "${BUILD_TYPE,,}" in
    debug|d) BUILD_TYPE="Debug" ;;
    *) BUILD_TYPE="Release" ;;
esac

echo ""
echo "================================================"
echo "        AlgoritmiaCombinatoria Builder        "
echo "             Modo Simple v2.1                "
echo "            (Version Bash/Unix)               "
echo "================================================"
echo ""
echo "Compilador: $COMPILER"
echo "Build Type: $BUILD_TYPE" 
echo "Estandar C++: $CPP_STANDARD"
echo ""

# Limpiar build anterior
if [[ -d "build" ]]; then
    echo -e "${YELLOW}[INFO] Limpiando build anterior...${NC}"
    rm -rf build
fi

# Crear directorio build
mkdir -p build
cd build

# Configurar CMake segun compilador (logica identica al .bat)
CMAKE_ARGS=""
case "$COMPILER" in
    gcc)
        echo -e "${BLUE}[INFO] Configurando para GCC con C++$CPP_STANDARD${NC}"
        CMAKE_ARGS="-G \"Ninja\" -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DCMAKE_CXX_STANDARD=$CPP_STANDARD -DCMAKE_CXX_COMPILER=g++ -DCMAKE_C_COMPILER=gcc"
        ;;
    clang)
        echo -e "${BLUE}[INFO] Configurando para Clang con C++$CPP_STANDARD${NC}"
        CMAKE_ARGS="-G \"Ninja\" -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DCMAKE_CXX_STANDARD=$CPP_STANDARD -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang"
        ;;
    msvc)
        echo -e "${BLUE}[INFO] Configurando para MSVC con C++$CPP_STANDARD${NC}"
        CMAKE_ARGS="-G \"Visual Studio 17 2022\" -A x64 -DCMAKE_CXX_STANDARD=$CPP_STANDARD"
        ;;
    *)
        echo -e "${RED}[ERROR] Compilador '$COMPILER' no soportado${NC}"
        echo -e "${YELLOW}[INFO] Compiladores soportados: gcc, clang, msvc${NC}"
        cd ..
        exit 1
        ;;
esac

# Ejecutar configuracion CMake
echo -e "${BLUE}[INFO] Configurando proyecto...${NC}"
if ! eval "cmake .. $CMAKE_ARGS"; then
    echo -e "${RED}[ERROR] Error en la configuracion de CMake${NC}"
    cd ..
    exit 1
fi

# Construir proyecto
echo -e "${BLUE}[INFO] Construyendo...${NC}"
if ! cmake --build . --config $BUILD_TYPE; then
    echo -e "${RED}[ERROR] Error en la construccion${NC}" 
    cd ..
    exit 1
fi

echo ""
echo -e "${GREEN}[SUCCESS] Construccion completada${NC}"
echo ""

# Mostrar ejecutables generados (identico al .bat)
echo -e "${YELLOW}Ejecutables generados:${NC}"
if ls *.exe &>/dev/null; then
    ls *.exe 2>/dev/null
elif find . -maxdepth 1 -type f -executable ! -name "*.so" ! -name "*.a" &>/dev/null; then
    find . -maxdepth 1 -type f -executable ! -name "*.so" ! -name "*.a" 2>/dev/null
else
    echo "Ninguno encontrado en este directorio"
fi

cd ..
echo ""
echo -e "${BLUE}[INFO] Build completado en: build${NC}"

exit 0