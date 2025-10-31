#!/bin/bash

# Build Script Simple para AlgoritmiaCombinatoria - MSYS2 Compatible
# Uso: ./build_simple.sh [gcc|clang] [debug|release] [cpp_version]
# Compatible con versión .bat - Optimizado para MSYS2

# Colores para output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
NC='\033[0m' # No Color

# Función para mostrar ayuda
show_help() {
    echo -e "${BLUE}╔══════════════════════════════════════════════╗${NC}"
    echo -e "${BLUE}║        AlgoritmiaCombinatoria Builder        ║${NC}"
    echo -e "${BLUE}║         Modo Simple v2.0 - MSYS2            ║${NC}"
    echo -e "${BLUE}╚══════════════════════════════════════════════╝${NC}"
    echo ""
    echo -e "${YELLOW}MODO SIMPLE (compatible con .bat):${NC}"
    echo ""
    echo "  $0 [compilador] [tipo] [cpp_std]"
    echo ""
    echo -e "${GREEN}COMPILADORES:${NC} gcc, clang"
    echo -e "${GREEN}TIPOS:${NC} debug, release"
    echo -e "${GREEN}ESTÁNDARES C++:${NC} 14, 17, 20, 23"
    echo ""
    echo -e "${BLUE}EJEMPLOS:${NC}"
    echo "  $0 gcc debug           # GCC Debug C++17 (por defecto)"
    echo "  $0 clang release       # Clang Release C++17"
    echo "  $0 gcc debug 20        # GCC Debug C++20"
    echo "  $0 clang release 23    # Clang Release C++23"
    echo "  $0                     # GCC Release C++17 (por defecto)"
    echo ""
    echo -e "${PURPLE}NOTA:${NC} Para MSVC usa la versión .bat desde PowerShell"
}

# Función para configurar estándar C++
set_cpp_standard() {
    local cpp_std="$1"
    case "$cpp_std" in
        ""|"17")
            CPP_STANDARD="17"
            ;;
        "14")
            CPP_STANDARD="14"
            ;;
        "20")
            CPP_STANDARD="20"
            ;;
        "23")
            CPP_STANDARD="23"
            ;;
        *)
            echo -e "${YELLOW}[WARNING] Versión C++ '$cpp_std' no válida, usando C++17 por defecto${NC}"
            CPP_STANDARD="17"
            ;;
    esac
}

# Valores por defecto
COMPILER=${1:-gcc}
BUILD_TYPE=${2:-release}
CPP_STANDARD="17"

# Configurar estándar C++
set_cpp_standard "$3"

# Manejar comandos especiales
if [[ "$1" == "-h" || "$1" == "--help" ]]; then
    show_help
    exit 0
fi

# Validar argumentos
if [[ ! "$COMPILER" =~ ^(gcc|clang)$ ]]; then
    echo -e "${RED}Error: Compilador '$COMPILER' no válido para MSYS2${NC}"
    echo -e "Opciones válidas: gcc, clang"
    echo -e "Para MSVC usa la versión .bat"
    exit 1
fi

if [[ ! "$BUILD_TYPE" =~ ^(debug|release)$ ]]; then
    echo -e "${RED}Error: Tipo de build '$BUILD_TYPE' no válido${NC}"
    echo -e "Opciones válidas: debug, release"
    exit 1
fi

# Mostrar información
echo -e "${GREEN}[INFO] Modo simple: $COMPILER $BUILD_TYPE C++$CPP_STANDARD${NC}"
echo ""
echo -e "${PURPLE}╔══════════════════════════════════════════════╗${NC}"
echo -e "${PURPLE}║        AlgoritmiaCombinatoria Builder        ║${NC}"
echo -e "${PURPLE}║         Modo Simple v2.0 - MSYS2            ║${NC}"
echo -e "${PURPLE}╚══════════════════════════════════════════════╝${NC}"
echo ""
echo -e "Compilador: ${YELLOW}$COMPILER${NC}"
echo -e "Build Type: ${YELLOW}$BUILD_TYPE${NC}"
echo -e "Estándar C++: ${YELLOW}$CPP_STANDARD${NC}"
echo ""

# Limpiar build anterior (usar comandos nativos de Windows)
if [ -d "build" ]; then
    echo -e "${YELLOW}[INFO] Limpiando build anterior...${NC}"
    rm -rf build 2>/dev/null || rmdir /s /q build 2>/dev/null || true
fi

# Crear directorio build
mkdir -p build
cd build

# Configurar argumentos CMake
CMAKE_ARGS="-DCMAKE_BUILD_TYPE=$BUILD_TYPE -DCMAKE_CXX_STANDARD=$CPP_STANDARD"

case "$COMPILER" in
    gcc)
        CMAKE_ARGS="$CMAKE_ARGS -DCMAKE_CXX_COMPILER=C:/msys64/ucrt64/bin/g++.exe -DCMAKE_C_COMPILER=C:/msys64/ucrt64/bin/gcc.exe"
        echo -e "${GREEN}[INFO] Configurando para GCC con C++$CPP_STANDARD${NC}"
        ;;
    clang)
        CMAKE_ARGS="$CMAKE_ARGS -DCMAKE_CXX_COMPILER=C:/msys64/ucrt64/bin/clang++.exe -DCMAKE_C_COMPILER=C:/msys64/ucrt64/bin/clang.exe"
        echo -e "${GREEN}[INFO] Configurando para Clang con C++$CPP_STANDARD${NC}"
        ;;
esac

# Ejecutar CMake
echo -e "${BLUE}[INFO] Configurando proyecto...${NC}"
if ! cmake .. $CMAKE_ARGS; then
    echo -e "${RED}[ERROR] Error en la configuración${NC}"
    cd ..
    exit 1
fi

# Construir
echo -e "${BLUE}[INFO] Construyendo...${NC}"
if ! cmake --build . --config $BUILD_TYPE --parallel 4; then
    echo -e "${RED}[ERROR] Error en la construcción${NC}"
    cd ..
    exit 1
fi

echo ""
echo -e "${GREEN}[SUCCESS] ¡Construcción completada!${NC}"
echo ""

# Mostrar ejecutables
echo -e "${YELLOW}Ejecutables generados:${NC}"
ls -la *.exe 2>/dev/null || echo "  (ejecutables en subdirectorios)"
find . -name "*.exe" -type f 2>/dev/null | head -10

cd ..
echo ""
echo -e "${GREEN}[INFO] Build completado en: build/${NC}"

# Mostrar ejemplos de uso
echo ""
echo -e "${BLUE}Ejemplos de ejecución:${NC}"
echo "  ./build/AlgoritmiaCombinatoria.exe"
echo "  ./build/factorial_demo.exe" 
echo "  ./build/tests.exe"