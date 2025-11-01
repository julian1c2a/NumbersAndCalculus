#!/bin/bash

# Script de construccion para AlgoritmiaCombinatoria (Equivalente a .bat)
# Uso: ./build_simple_fixed.sh [compilador] [tipo] [estandar]
# Soporte para: gcc, clang, msvc

set -e  # Salir en caso de error

# Colores para output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Verificar si es solicitud de ayuda
if [[ "$1" == "-h" || "$1" == "--help" ]]; then
    echo ""
    echo "================================================"
    echo "         AlgoritmiaCombinatoria Builder        "
    echo "              Modo Simple v2.1                "
    echo "            (Version Bash/Unix)               "
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
    echo "  $0 msvc release 23   # MSVC Release C++23 (solo WSL)"
    echo "  $0                   # GCC Release C++17 (por defecto)"
    echo ""
    exit 0
fi

# Configurar valores por defecto
COMPILER="${1:-gcc}"
BUILD_TYPE="${2:-Release}"
CPP_STANDARD="${3:-17}"

# Convertir a formato CMake
if [[ "${BUILD_TYPE,,}" == "debug" ]]; then
    BUILD_TYPE="Debug"
else
    BUILD_TYPE="Release"
fi

# Detectar sistema operativo
detect_os() {
    case "$(uname -s)" in
        MSYS_NT*|MINGW*)
            OS="Windows-MSYS"
            echo -e "${BLUE}[INFO] Sistema Windows/MSYS2 detectado${NC}"
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

# Detectar generadores disponibles
detect_generator() {
    if command -v ninja &> /dev/null || command -v ninja-build &> /dev/null; then
        GENERATOR="Ninja"
        echo -e "${GREEN}[INFO] Ninja encontrado${NC}"
    elif command -v make &> /dev/null; then
        GENERATOR="Unix Makefiles"
        echo -e "${GREEN}[INFO] Make encontrado${NC}"
    else
        echo -e "${RED}[ERROR] No se encontro Ninja ni Make${NC}"
        exit 1
    fi
}

# Detectar numero de cores
detect_cores() {
    if command -v nproc &> /dev/null; then
        CORES=$(nproc)
    elif [[ "$OS" == "macOS" ]] && command -v sysctl &> /dev/null; then
        CORES=$(sysctl -n hw.ncpu)
    else
        CORES=4
    fi
    echo -e "${BLUE}[INFO] Usando $CORES jobs paralelos${NC}"
}

# Verificar herramientas basicas
verify_tools() {
    echo -e "${BLUE}[INFO] Verificando herramientas...${NC}"
    
    # Verificar CMake
    if ! command -v cmake &> /dev/null; then
        echo -e "${RED}[ERROR] CMake no encontrado${NC}"
        exit 1
    fi
    
    CMAKE_VERSION=$(cmake --version 2>/dev/null | head -n1 | grep -oE '[0-9]+\.[0-9]+' | head -1)
    echo -e "${GREEN}[INFO] CMake $CMAKE_VERSION encontrado${NC}"
    
    # Verificar compilador solicitado
    case "$COMPILER" in
        gcc)
            if ! command -v gcc &> /dev/null; then
                echo -e "${RED}[ERROR] GCC no encontrado${NC}"
                exit 1
            fi
            GCC_VERSION=$(gcc --version 2>/dev/null | head -n1)
            echo -e "${GREEN}[INFO] $GCC_VERSION${NC}"
            ;;
        clang)
            if ! command -v clang &> /dev/null; then
                echo -e "${RED}[ERROR] Clang no encontrado${NC}"
                exit 1
            fi
            CLANG_VERSION=$(clang --version 2>/dev/null | head -n1)
            echo -e "${GREEN}[INFO] $CLANG_VERSION${NC}"
            ;;
        msvc)
            if [[ "$OS" != "WSL" ]]; then
                echo -e "${RED}[ERROR] MSVC solo soportado en WSL${NC}"
                echo -e "${YELLOW}[HELP] En MSYS2/MinGW usa el script .bat${NC}"
                exit 1
            fi
            echo -e "${GREEN}[INFO] MSVC sera configurado via WSL${NC}"
            ;;
        *)
            echo -e "${RED}[ERROR] Compilador '$COMPILER' no soportado${NC}"
            exit 1
            ;;
    esac
}

# Mostrar configuracion
show_config() {
    echo ""
    echo "================================================"
    echo "         AlgoritmiaCombinatoria Builder        "
    echo "              Modo Simple v2.1                "
    echo "            (Version Bash/Unix)               "
    echo "================================================"
    echo ""
    echo "Sistema: $OS"
    echo "Generador: $GENERATOR"
    echo "Compilador: $COMPILER"
    echo "Build Type: $BUILD_TYPE"
    echo "Estandar C++: $CPP_STANDARD"
    echo "Jobs Paralelos: $CORES"
    echo ""
}

# Configurar CMake args
configure_cmake() {
    CMAKE_ARGS="-G \"$GENERATOR\" -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DCMAKE_CXX_STANDARD=$CPP_STANDARD"
    
    case "$COMPILER" in
        gcc)
            CMAKE_ARGS="$CMAKE_ARGS -DCMAKE_CXX_COMPILER=g++ -DCMAKE_C_COMPILER=gcc"
            echo -e "${GREEN}[INFO] Configurando para GCC con C++$CPP_STANDARD${NC}"
            ;;
        clang)
            CMAKE_ARGS="$CMAKE_ARGS -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang"
            echo -e "${GREEN}[INFO] Configurando para Clang con C++$CPP_STANDARD${NC}"
            ;;
        msvc)
            # En WSL, configurar para MSVC
            CMAKE_ARGS="-G \"Visual Studio 17 2022\" -A x64 -DCMAKE_CXX_STANDARD=$CPP_STANDARD"
            echo -e "${GREEN}[INFO] Configurando para MSVC via WSL con C++$CPP_STANDARD${NC}"
            ;;
    esac
}

# Construir proyecto
build_project() {
    # Limpiar build anterior
    if [[ -d "build" ]]; then
        echo -e "${YELLOW}[INFO] Limpiando build anterior...${NC}"
        rm -rf build
    fi
    
    # Crear directorio build
    mkdir -p build
    cd build
    
    # Ejecutar CMake
    echo -e "${BLUE}[INFO] Configurando proyecto...${NC}"
    if ! eval "cmake .. $CMAKE_ARGS"; then
        echo -e "${RED}[ERROR] Error en la configuracion de CMake${NC}"
        cd ..
        exit 1
    fi
    
    # Construir
    echo -e "${BLUE}[INFO] Construyendo con $CORES jobs...${NC}"
    if [[ "$COMPILER" == "msvc" ]]; then
        # Para MSVC en WSL usar MSBuild
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
    find . -name "*.exe" -o \( -type f -executable ! -name "*.so" ! -name "*.a" \) 2>/dev/null | head -10
    
    cd ..
    echo ""
    echo -e "${GREEN}[INFO] Build completado en: build/${NC}"
}

# Ejecutar secuencia principal
main() {
    detect_os
    detect_generator
    detect_cores
    verify_tools
    show_config
    configure_cmake
    build_project
    
    echo -e "${GREEN}[SUCCESS] Proceso completado exitosamente!${NC}"
}

# Ejecutar script principal
main "$@"