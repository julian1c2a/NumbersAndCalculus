#!/bin/bash

# Script de construccion multiplataforma con directorios separados
# Uso: ./build_multi_dirs.sh [compilador] [tipo] [estandar]
# Crea: build_[compilador]_[tipo]_cpp[estandar]/

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
    echo "    AlgoritmiaCombinatoria Multi-Build System   "
    echo "              Directorios Separados             "
    echo "================================================"
    echo ""
    echo "SINTAXIS: $0 [compilador] [tipo] [estandar]"
    echo ""
    echo "COMPILADORES: gcc, clang, msvc"
    echo "TIPOS: debug, release" 
    echo "ESTANDARES C++: 14, 17, 20, 23"
    echo ""
    echo "DIRECTORIOS DE BUILD:"
    echo "  build_gcc_debug_cpp17/     # GCC Debug C++17"
    echo "  build_clang_release_cpp20/ # Clang Release C++20"
    echo "  build_msvc_release_cpp23/  # MSVC Release C++23"
    echo ""
    echo "EJEMPLOS:"
    echo "  $0 gcc debug 17      # -> build_gcc_debug_cpp17/"
    echo "  $0 clang release 20  # -> build_clang_release_cpp20/"
    echo "  $0 msvc release 23   # -> build_msvc_release_cpp23/"
    echo "  $0                   # -> build_gcc_release_cpp17/"
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

# Crear nombre del directorio de build específico
BUILD_DIR="build_${COMPILER,,}_${BUILD_TYPE,,}_cpp${CPP_STANDARD}"

echo ""
echo "================================================"
echo "    AlgoritmiaCombinatoria Multi-Build System   "
echo "              Directorios Separados             "
echo "================================================"
echo ""
echo "Compilador: $COMPILER"
echo "Build Type: $BUILD_TYPE"
echo "Estandar C++: $CPP_STANDARD"
echo "Directorio: $BUILD_DIR"
echo ""

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

# Verificar herramientas básicas
verify_tools() {
    echo -e "${BLUE}[INFO] Verificando herramientas...${NC}"
    
    # Verificar CMake
    if ! command -v cmake >/dev/null 2>&1; then
        echo -e "${RED}[ERROR] CMake no encontrado${NC}"
        exit 1
    fi
    echo -e "${GREEN}[INFO] CMake encontrado${NC}"
    
    # Verificar Ninja
    if command -v ninja >/dev/null 2>&1; then
        GENERATOR="Ninja"
        echo -e "${GREEN}[INFO] Ninja encontrado${NC}"
    elif command -v make >/dev/null 2>&1; then
        GENERATOR="Unix Makefiles"
        echo -e "${GREEN}[INFO] Make encontrado${NC}"
    else
        echo -e "${RED}[ERROR] No se encontro Ninja ni Make${NC}"
        exit 1
    fi
    
    # Verificar compilador
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

# Configurar CMake
configure_cmake() {
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
}

# Construir proyecto en directorio específico
build_project() {
    # Limpiar directorio específico si existe
    if [[ -d "$BUILD_DIR" ]]; then
        echo -e "${YELLOW}[INFO] Limpiando $BUILD_DIR anterior...${NC}"
        rm -rf "$BUILD_DIR"
    fi
    
    # Crear directorio específico
    mkdir -p "$BUILD_DIR"
    cd "$BUILD_DIR"
    
    # Ejecutar configuración CMake
    echo -e "${BLUE}[INFO] Configurando en $BUILD_DIR...${NC}"
    if ! eval "cmake .. $CMAKE_ARGS"; then
        echo -e "${RED}[ERROR] Error en la configuracion de CMake${NC}"
        cd ..
        exit 1
    fi
    
    # Construir
    echo -e "${BLUE}[INFO] Construyendo en $BUILD_DIR...${NC}"
    if [[ "$COMPILER" == "msvc" ]]; then
        if ! cmake --build . --config $BUILD_TYPE; then
            echo -e "${RED}[ERROR] Error en la construccion${NC}"
            cd ..
            exit 1
        fi
    else
        if ! cmake --build . --config $BUILD_TYPE --parallel 16; then
            echo -e "${RED}[ERROR] Error en la construccion${NC}"
            cd ..
            exit 1
        fi
    fi
    
    echo ""
    echo -e "${GREEN}[SUCCESS] Construccion completada en $BUILD_DIR!${NC}"
    echo ""
    
    # Mostrar ejecutables
    echo -e "${YELLOW}Ejecutables en $BUILD_DIR:${NC}"
    if ls *.exe >/dev/null 2>&1; then
        ls *.exe 2>/dev/null || true
    elif find . -maxdepth 1 -type f -executable ! -name "*.so" ! -name "*.a" >/dev/null 2>&1; then
        find . -maxdepth 1 -type f -executable ! -name "*.so" ! -name "*.a" 2>/dev/null || true
    else
        echo "Ninguno encontrado"
    fi
    
    cd ..
}

# Mostrar resumen final
show_summary() {
    echo ""
    echo "================================================"
    echo "           BUILD COMPLETADO EXITOSAMENTE        "
    echo "================================================"
    echo ""
    echo "Configuración: $COMPILER $BUILD_TYPE C++$CPP_STANDARD"
    echo "Directorio: $BUILD_DIR"
    echo ""
    
    # Mostrar todos los directorios de build existentes
    echo -e "${YELLOW}Directorios de build existentes:${NC}"
    if ls -d build_*/ >/dev/null 2>&1; then
        ls -d build_*/ 2>/dev/null | sort || true
    else
        echo "Solo: $BUILD_DIR/"
    fi
    
    echo ""
    echo -e "${GREEN}[INFO] Puedes cambiar al directorio con: cd $BUILD_DIR${NC}"
    echo -e "${GREEN}[INFO] Para limpiar todos los builds: rm -rf build_*/build${NC}"
}

# Ejecutar secuencia principal
main() {
    detect_os
    verify_tools
    configure_cmake
    build_project
    show_summary
}

# Ejecutar
main "$@"