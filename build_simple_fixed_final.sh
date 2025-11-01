#!/bin/bash

# Script de construccion para AlgoritmiaCombinatoria 
# Version arreglada sin funciones problemáticas
# Uso: ./build_simple_fixed.sh [compilador] [tipo] [estandar]

set -e  # Salir en caso de error

# Colores para output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Detectar sistema operativo
detect_os() {
    case "$(uname -s)" in
        MSYS_NT*|MINGW*)
            OS="MINGW64"
            echo -e "${BLUE}[INFO] Entorno Windows detectado: MINGW64${NC}"
            ;;
        Linux*)
            if grep -qi microsoft /proc/version 2>/dev/null; then
                OS="WSL"
                echo -e "${BLUE}[INFO] Windows Subsystem for Linux detectado${NC}"
            else
                OS="Linux"
                echo -e "${BLUE}[INFO] Sistema Linux nativo detectado${NC}"
            fi
            ;;
        Darwin*)
            OS="macOS"
            echo -e "${BLUE}[INFO] Sistema macOS detectado${NC}"
            ;;
        *)
            OS="Unknown"
            echo -e "${YELLOW}[WARNING] Sistema no reconocido: $(uname -s)${NC}"
            ;;
    esac
}

# Detectar generadores disponibles - VERSION SIMPLIFICADA
detect_generator() {
    # Verificar Ninja primero
    if command -v ninja >/dev/null 2>&1; then
        GENERATOR="Ninja"
        echo -e "${GREEN}[INFO] Ninja encontrado${NC}"
        return 0
    fi
    
    # Verificar ninja-build como alternativa  
    if command -v ninja-build >/dev/null 2>&1; then
        GENERATOR="Ninja"
        echo -e "${GREEN}[INFO] Ninja encontrado${NC}"
        return 0
    fi
    
    # Fallback a Make
    if command -v make >/dev/null 2>&1; then
        GENERATOR="Unix Makefiles"
        echo -e "${GREEN}[INFO] Make encontrado${NC}"
        return 0
    fi
    
    echo -e "${RED}[ERROR] No se encontro Ninja ni Make${NC}"
    exit 1
}

# Detectar numero de cores - VERSION SIMPLIFICADA
detect_cores() {
    CORES=16  # Valor fijo para evitar problemas
    echo -e "${BLUE}[INFO] Usando $CORES jobs paralelos${NC}"
}

# Verificación básica de herramientas - SIN VERSIONES DETALLADAS
verify_basic_tools() {
    echo -e "${BLUE}[INFO] Verificando herramientas básicas...${NC}"
    
    # Solo verificar que existan, sin obtener versiones
    if ! command -v cmake >/dev/null 2>&1; then
        echo -e "${RED}[ERROR] CMake no encontrado${NC}"
        exit 1
    fi
    echo -e "${GREEN}[INFO] CMake encontrado${NC}"
    
    # Verificar compilador solicitado SIN obtener versiones
    case "$COMPILER" in
        gcc)
            if ! command -v gcc >/dev/null 2>&1; then
                echo -e "${RED}[ERROR] GCC no encontrado${NC}"
                exit 1
            fi
            echo -e "${GREEN}[INFO] GCC encontrado${NC}"
            ;;
        clang)
            if ! command -v clang >/dev/null 2>&1; then
                echo -e "${RED}[ERROR] Clang no encontrado${NC}"
                exit 1
            fi
            echo -e "${GREEN}[INFO] Clang encontrado${NC}"
            ;;
        msvc)
            echo -e "${GREEN}[INFO] MSVC sera configurado${NC}"
            ;;
        *)
            echo -e "${RED}[ERROR] Compilador '$COMPILER' no soportado${NC}"
            exit 1
            ;;
    esac
}

# Verificar ayuda
if [[ "$1" == "-h" || "$1" == "--help" ]]; then
    echo ""
    echo "================================================"
    echo "         AlgoritmiaCombinatoria Builder        "
    echo "              Modo Simple v2.1                "
    echo "            (Version Bash Arreglada)          "
    echo "================================================"
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

# Configurar valores por defecto
COMPILER="${1:-gcc}"
BUILD_TYPE="${2:-release}"
CPP_STANDARD="${3:-17}"

# Normalizar build type
case "${BUILD_TYPE,,}" in
    debug|d) BUILD_TYPE="Debug" ;;
    *) BUILD_TYPE="Release" ;;
esac

echo ""
echo "================================================"
echo "         AlgoritmiaCombinatoria Builder        "
echo "              Modo Simple v2.1                "
echo "            (Version Bash Arreglada)          "
echo "================================================"
echo ""
echo "Compilador: $COMPILER"
echo "Build Type: $BUILD_TYPE"
echo "Estandar C++: $CPP_STANDARD"
echo ""

# Ejecutar funciones de deteccion
detect_os
detect_generator
detect_cores
verify_basic_tools

# Configurar CMake segun compilador
CMAKE_ARGS=""
case "$COMPILER" in
    gcc)
        echo -e "${BLUE}[INFO] Configurando para GCC con C++$CPP_STANDARD${NC}"
        CMAKE_ARGS="-G \"$GENERATOR\" -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DCMAKE_CXX_STANDARD=$CPP_STANDARD -DCMAKE_CXX_COMPILER=g++ -DCMAKE_C_COMPILER=gcc"
        ;;
    clang)
        echo -e "${BLUE}[INFO] Configurando para Clang con C++$CPP_STANDARD${NC}"
        CMAKE_ARGS="-G \"$GENERATOR\" -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DCMAKE_CXX_STANDARD=$CPP_STANDARD -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang"
        ;;
    msvc)
        echo -e "${BLUE}[INFO] Configurando para MSVC con C++$CPP_STANDARD${NC}"
        CMAKE_ARGS="-G \"Visual Studio 17 2022\" -A x64 -DCMAKE_CXX_STANDARD=$CPP_STANDARD"
        ;;
esac

# Limpiar y crear build directory
if [[ -d "build" ]]; then
    echo -e "${YELLOW}[INFO] Limpiando build anterior...${NC}"
    rm -rf build
fi

mkdir -p build
cd build

# Ejecutar configuración CMake
echo -e "${BLUE}[INFO] Configurando proyecto...${NC}"
if ! eval "cmake .. $CMAKE_ARGS"; then
    echo -e "${RED}[ERROR] Error en la configuracion de CMake${NC}"
    cd ..
    exit 1
fi

# Construir proyecto
echo -e "${BLUE}[INFO] Construyendo con $CORES jobs...${NC}"
if [[ "$COMPILER" == "msvc" ]]; then
    # Para MSVC usar MSBuild
    if ! cmake --build . --config $BUILD_TYPE; then
        echo -e "${RED}[ERROR] Error en la construccion${NC}"
        cd ..
        exit 1
    fi
else
    # Para GCC/Clang usar paralelismo
    if ! cmake --build . --config $BUILD_TYPE --parallel $CORES; then
        echo -e "${RED}[ERROR] Error en la construccion${NC}"
        cd ..
        exit 1
    fi
fi

echo ""
echo -e "${GREEN}[SUCCESS] Construccion completada exitosamente!${NC}"
echo ""

# Mostrar ejecutables
echo -e "${YELLOW}Ejecutables generados:${NC}"
if ls *.exe >/dev/null 2>&1; then
    ls *.exe 2>/dev/null || true
elif find . -maxdepth 1 -type f -executable ! -name "*.so" ! -name "*.a" >/dev/null 2>&1; then
    find . -maxdepth 1 -type f -executable ! -name "*.so" ! -name "*.a" 2>/dev/null || true
else
    echo "Ninguno encontrado en este directorio"
fi

cd ..
echo ""
echo -e "${GREEN}[INFO] Build completado en: build/${NC}"
echo ""
echo -e "${GREEN}[SUCCESS] Proceso completado exitosamente!${NC}"