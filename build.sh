#!/bin/bash

# Script de construcción para AlgoritmiaCombinatoria
# Soporta múltiples compiladores y configuraciones

set -e

# Colores para output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Función para mostrar ayuda
show_help() {
    echo "Uso: $0 [OPCIONES]"
    echo ""
    echo "Opciones:"
    echo "  -c, --compiler COMP    Especificar compilador (gcc, clang, msvc)"
    echo "  -t, --type TYPE        Tipo de construcción (Debug, Release)"
    echo "  -g, --generator GEN    Generador de CMake (Ninja, 'Unix Makefiles', etc.)"
    echo "  -j, --jobs JOBS        Número de trabajos paralelos"
    echo "  -r, --run              Ejecutar después de construir"
    echo "  -T, --test             Ejecutar pruebas después de construir"
    echo "  -C, --clean            Limpiar antes de construir"
    echo "  -h, --help             Mostrar esta ayuda"
    echo ""
    echo "Ejemplos:"
    echo "  $0 -c gcc -t Release -g Ninja"
    echo "  $0 --compiler clang --type Debug --test"
    echo "  $0 --clean --run"
}

# Valores por defecto
COMPILER=""
BUILD_TYPE="Release"
GENERATOR="Ninja"
JOBS=4
RUN_AFTER=false
TEST_AFTER=false
CLEAN_BEFORE=false

# Parsear argumentos
while [[ $# -gt 0 ]]; do
    case $1 in
        -c|--compiler)
            COMPILER="$2"
            shift 2
            ;;
        -t|--type)
            BUILD_TYPE="$2"
            shift 2
            ;;
        -g|--generator)
            GENERATOR="$2"
            shift 2
            ;;
        -j|--jobs)
            JOBS="$2"
            shift 2
            ;;
        -r|--run)
            RUN_AFTER=true
            shift
            ;;
        -T|--test)
            TEST_AFTER=true
            shift
            ;;
        -C|--clean)
            CLEAN_BEFORE=true
            shift
            ;;
        -h|--help)
            show_help
            exit 0
            ;;
        *)
            echo "Opción desconocida: $1"
            show_help
            exit 1
            ;;
    esac
done

# Función para log con colores
log_info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

log_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

log_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

log_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Detectar sistema operativo
detect_os() {
    case "$(uname -s)" in
        Linux*)     OS=Linux;;
        Darwin*)    OS=macOS;;
        CYGWIN*)    OS=Windows;;
        MINGW*)     OS=Windows;;
        MSYS*)      OS=Windows;;
        *)          OS="Unknown";;
    esac
    log_info "Sistema detectado: $OS"
}

# Configurar compilador
setup_compiler() {
    local cmake_args=()
    
    case $COMPILER in
        gcc)
            if [[ "$OS" == "Windows" ]]; then
                cmake_args+=("-DCMAKE_CXX_COMPILER=C:/msys64/ucrt64/bin/g++.exe")
                cmake_args+=("-DCMAKE_C_COMPILER=C:/msys64/ucrt64/bin/gcc.exe")
                log_info "Usando MSYS2 GCC"
            else
                cmake_args+=("-DCMAKE_CXX_COMPILER=g++")
                cmake_args+=("-DCMAKE_C_COMPILER=gcc")
                log_info "Usando GCC"
            fi
            ;;
        clang)
            if [[ "$OS" == "Windows" ]]; then
                cmake_args+=("-DCMAKE_CXX_COMPILER=C:/msys64/ucrt64/bin/clang++.exe")
                cmake_args+=("-DCMAKE_C_COMPILER=C:/msys64/ucrt64/bin/clang.exe")
                log_info "Usando MSYS2 Clang"
            else
                cmake_args+=("-DCMAKE_CXX_COMPILER=clang++")
                cmake_args+=("-DCMAKE_C_COMPILER=clang")
                log_info "Usando Clang"
            fi
            ;;
        msvc)
            if [[ "$OS" != "Windows" ]]; then
                log_error "MSVC solo está disponible en Windows"
                exit 1
            fi
            log_info "Usando MSVC"
            ;;
        "")
            if [[ "$OS" == "Windows" ]]; then
                # Por defecto usar MSYS2 GCC en Windows
                cmake_args+=("-DCMAKE_CXX_COMPILER=C:/msys64/ucrt64/bin/g++.exe")
                cmake_args+=("-DCMAKE_C_COMPILER=C:/msys64/ucrt64/bin/gcc.exe")
                log_info "Usando MSYS2 GCC por defecto"
            else
                log_info "Usando compilador por defecto del sistema"
            fi
            ;;
        *)
            log_error "Compilador desconocido: $COMPILER"
            exit 1
            ;;
    esac
    
    echo "${cmake_args[@]}"
}

# Función principal de construcción
build_project() {
    log_info "Iniciando construcción del proyecto AlgoritmiaCombinatoria"
    
    # Limpiar si se solicita
    if [[ "$CLEAN_BEFORE" == true ]]; then
        log_info "Limpiando directorio de construcción..."
        rm -rf build/
    fi
    
    # Crear directorio de construcción
    mkdir -p build
    cd build
    
    # Configurar CMake
    log_info "Configurando proyecto con CMake..."
    local cmake_args=($(setup_compiler))
    cmake_args+=("-G" "$GENERATOR")
    cmake_args+=("-DCMAKE_BUILD_TYPE=$BUILD_TYPE")
    
    # Buscar vcpkg si existe
    if [[ -n "${VCPKG_ROOT:-}" ]]; then
        cmake_args+=("-DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake")
        log_info "Usando vcpkg en: $VCPKG_ROOT"
    fi
    
    cmake .. "${cmake_args[@]}"
    
    # Construir
    log_info "Construyendo proyecto..."
    cmake --build . --config "$BUILD_TYPE" --parallel "$JOBS"
    
    log_success "Construcción completada exitosamente"
}

# Ejecutar proyecto
run_project() {
    if [[ "$RUN_AFTER" == true ]]; then
        log_info "Ejecutando proyecto..."
        if [[ "$OS" == "Windows" ]]; then
            ./"$BUILD_TYPE"/AlgoritmiaCombinatoria.exe || ./AlgoritmiaCombinatoria.exe
        else
            ./AlgoritmiaCombinatoria
        fi
    fi
}

# Ejecutar pruebas
run_tests() {
    if [[ "$TEST_AFTER" == true ]]; then
        log_info "Ejecutando pruebas..."
        ctest --output-on-failure --parallel "$JOBS"
        log_success "Todas las pruebas pasaron"
    fi
}

# Función principal
main() {
    detect_os
    build_project
    run_tests
    run_project
    
    log_success "¡Proceso completado!"
}

# Ejecutar solo si el script se ejecuta directamente
if [[ "${BASH_SOURCE[0]}" == "${0}" ]]; then
    main "$@"
fi