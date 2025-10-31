#!/bin/bash

# AlgoritmiaCombinatoria Build System v2.0 - Windows Compatible
# Script mejorado con soporte nativo para Windows/MSYS2

set -e

# Colores para output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
NC='\033[0m' # No Color

# Función para mostrar ayuda
show_help() {
    echo -e "${BLUE}AlgoritmiaCombinatoria Build System v2.0${NC}"
    echo ""
    echo "Uso: $0 [OPCIONES] o $0 [COMPILADOR] [TIPO]"
    echo ""
    echo "Modo Simple (como pediste):"
    echo "  $0 gcc debug           Compilar con GCC en modo debug"
    echo "  $0 clang release       Compilar con Clang en modo release"
    echo "  $0 msvc release        Compilar con MSVC en modo release"
    echo ""
    echo "Opciones Avanzadas:"
    echo "  -c, --compiler COMP    Especificar compilador (gcc, clang, msvc)"
    echo "  -t, --type TYPE        Tipo de construcción (Debug, Release)"
    echo "  -j, --jobs JOBS        Número de trabajos paralelos"
    echo "  -T, --test             Ejecutar pruebas después de construir"
    echo "  -B, --benchmarks       Ejecutar benchmarks después de construir"
    echo "  -A, --all              Construir, probar y ejecutar benchmarks"
    echo "  -C, --clean            Limpiar antes de construir"
    echo "  -L, --list             Listar ejecutables disponibles"
    echo "  -S, --stats            Mostrar estadísticas de compilación"
    echo "  -h, --help             Mostrar esta ayuda"
    echo ""
    echo "Ejemplos:"
    echo "  $0 gcc debug           # Modo simple"
    echo "  $0 -A                  # Todo: construir, tests y benchmarks"
    echo "  $0 -c clang -t Release -T -B"
}

# Valores por defecto
COMPILER=""
BUILD_TYPE="Release"
JOBS=4
RUN_AFTER=false
TEST_AFTER=false
CLEAN_BEFORE=false
BENCHMARKS_AFTER=false
ALL_MODE=false
LIST_MODE=false
STATS_MODE=false
SIMPLE_MODE=false

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

# Banner de inicio
show_banner() {
    echo -e "${PURPLE}╔══════════════════════════════════════════════╗${NC}"
    echo -e "${PURPLE}║        AlgoritmiaCombinatoria Builder        ║${NC}"
    echo -e "${PURPLE}║              Version 2.0 Enhanced           ║${NC}"
    echo -e "${PURPLE}╚══════════════════════════════════════════════╝${NC}"
    echo ""
}

# Función para parsear argumentos
parse_arguments() {
    # Modo simple: detectar argumentos simples primero
    if [[ $# -eq 2 && "$1" =~ ^(gcc|clang|msvc)$ && "$2" =~ ^(debug|release)$ ]]; then
        SIMPLE_MODE=true
        COMPILER="$1"
        case "$2" in
            debug) BUILD_TYPE="Debug" ;;
            release) BUILD_TYPE="Release" ;;
        esac
        log_info "Modo simple activado: $COMPILER $BUILD_TYPE"
        return
    elif [[ $# -eq 1 && "$1" =~ ^(gcc|clang|msvc)$ ]]; then
        SIMPLE_MODE=true
        COMPILER="$1"
        BUILD_TYPE="Release"
        log_info "Modo simple activado: $COMPILER Release (por defecto)"
        return
    fi

    # Parsear argumentos avanzados
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
            -B|--benchmarks)
                BENCHMARKS_AFTER=true
                shift
                ;;
            -A|--all)
                ALL_MODE=true
                TEST_AFTER=true
                BENCHMARKS_AFTER=true
                shift
                ;;
            -L|--list)
                LIST_MODE=true
                shift
                ;;
            -S|--stats)
                STATS_MODE=true
                shift
                ;;
            -h|--help)
                show_help
                exit 0
                ;;
            *)
                echo -e "${RED}Opción desconocida: $1${NC}"
                show_help
                exit 1
                ;;
        esac
    done
}

# Configurar compilador para CMake
setup_compiler() {
    local cmake_args=()
    
    case $COMPILER in
        gcc)
            cmake_args+=("-DCMAKE_CXX_COMPILER=g++")
            cmake_args+=("-DCMAKE_C_COMPILER=gcc")
            log_info "Usando GCC"
            ;;
        clang)
            cmake_args+=("-DCMAKE_CXX_COMPILER=clang++")
            cmake_args+=("-DCMAKE_C_COMPILER=clang")
            log_info "Usando Clang"
            ;;
        msvc)
            log_info "Usando MSVC (configuración automática)"
            ;;
        "")
            log_info "Usando compilador por defecto"
            ;;
        *)
            log_error "Compilador desconocido: $COMPILER"
            exit 1
            ;;
    esac
    
    echo "${cmake_args[@]}"
}

# Mostrar estadísticas
show_stats() {
    if [[ "$STATS_MODE" == true ]]; then
        log_info "Estadísticas del proyecto:"
        echo ""
        
        # Contar archivos usando métodos compatibles con Windows
        local cpp_count=0
        local h_count=0
        
        # Buscar archivos .cpp
        for file in *.cpp **/*.cpp; do
            [[ -f "$file" ]] && ((cpp_count++))
        done 2>/dev/null
        
        # Buscar archivos .h y .hpp
        for file in *.h *.hpp **/*.h **/*.hpp; do
            [[ -f "$file" ]] && ((h_count++))
        done 2>/dev/null
        
        echo "📁 Archivos fuente:"
        echo "   C++ files: $cpp_count"
        echo "   Headers: $h_count"
        
        # Estadísticas del build
        if [[ -d "build" ]]; then
            echo ""
            echo "🔨 Build directory: build/"
            
            local exe_count=0
            for file in build/*.exe build/**/*.exe; do
                [[ -f "$file" ]] && ((exe_count++))
            done 2>/dev/null
            
            echo "   Ejecutables: $exe_count"
        fi
        
        echo ""
        echo "🛠️  Configuración:"
        echo "   Compilador: ${COMPILER:-"default"}"
        echo "   Build Type: $BUILD_TYPE"
        echo "   Jobs: $JOBS"
        
        exit 0
    fi
}

# Listar ejecutables disponibles
list_executables() {
    if [[ "$LIST_MODE" == true ]]; then
        log_info "Ejecutables disponibles:"
        echo ""
        
        if [[ -d "build" ]]; then
            cd build
            local found_any=false
            
            echo "En build/:"
            for file in *.exe **/*.exe; do
                if [[ -f "$file" ]]; then
                    echo "  $file"
                    found_any=true
                fi
            done 2>/dev/null
            
            if [[ "$found_any" != true ]]; then
                log_warning "No se encontraron ejecutables"
            fi
            
            cd ..
        else
            log_warning "Directorio build/ no existe"
        fi
        exit 0
    fi
}

# Función principal de construcción
build_project() {
    log_info "Iniciando construcción del proyecto AlgoritmiaCombinatoria"
    
    # Limpiar si se solicita
    if [[ "$CLEAN_BEFORE" == true ]]; then
        log_info "Limpiando directorio de construcción..."
        [[ -d "build" ]] && rm -rf build/
    fi
    
    # Crear directorio de construcción
    [[ ! -d "build" ]] && mkdir -p build
    cd build
    
    # Configurar CMake
    log_info "Configurando proyecto con CMake..."
    local cmake_args=($(setup_compiler))
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

# Ejecutar pruebas
run_tests() {
    if [[ "$TEST_AFTER" == true ]]; then
        log_info "Ejecutando pruebas..."
        if ctest --output-on-failure --parallel "$JOBS"; then
            log_success "Todas las pruebas pasaron"
        else
            log_warning "Algunas pruebas fallaron, pero continuando..."
        fi
    fi
}

# Ejecutar benchmarks
run_benchmarks() {
    if [[ "$BENCHMARKS_AFTER" == true ]]; then
        log_info "Ejecutando benchmarks..."
        
        local benchmarks=(
            "comprehensive_benchmarks"
            "optimization_benchmarks"
            "memory_benchmarks"
            "simple_benchmark"
        )
        
        local executed=0
        for bench in "${benchmarks[@]}"; do
            local bench_path=""
            
            # Buscar el ejecutable
            for ext in ".exe" ""; do
                for dir in "." "$BUILD_TYPE"; do
                    local candidate="$dir/${bench}${ext}"
                    if [[ -f "$candidate" && -x "$candidate" ]]; then
                        bench_path="$candidate"
                        break 2
                    fi
                done
            done
            
            if [[ -n "$bench_path" ]]; then
                log_info "→ Ejecutando $bench..."
                if timeout 60s "$bench_path" 2>/dev/null; then
                    ((executed++))
                    echo ""
                else
                    log_warning "  Timeout o error en $bench"
                fi
            fi
        done
        
        if [[ $executed -gt 0 ]]; then
            log_success "Ejecutados $executed benchmarks"
        else
            log_warning "No se encontraron benchmarks ejecutables"
        fi
    fi
}

# Función principal
main() {
    show_banner
    parse_arguments "$@"
    
    # Manejar modos especiales que no requieren build
    list_executables
    show_stats
    
    # Si estamos en modo ALL, activar limpieza automática
    if [[ "$ALL_MODE" == true ]]; then
        CLEAN_BEFORE=true
        log_info "Modo ALL activado: limpieza, construcción, tests y benchmarks"
    fi
    
    # Mostrar configuración en modo simple
    if [[ "$SIMPLE_MODE" == true ]]; then
        echo -e "${YELLOW}═══════════════════════════════════════${NC}"
        echo -e "${YELLOW}  Modo Simple: $COMPILER $BUILD_TYPE${NC}"
        echo -e "${YELLOW}═══════════════════════════════════════${NC}"
    fi
    
    build_project
    run_tests
    run_benchmarks
    
    # Mostrar resumen final
    echo ""
    echo -e "${GREEN}═══════════════════════════════════════${NC}"
    echo -e "${GREEN}         ¡CONSTRUCCIÓN COMPLETADA!     ${NC}"
    echo -e "${GREEN}═══════════════════════════════════════${NC}"
    
    if [[ -d "build" ]]; then
        cd build
        local exe_count=0
        for file in *.exe **/*.exe; do
            [[ -f "$file" ]] && ((exe_count++))
        done 2>/dev/null
        
        echo -e "Ejecutables generados: ${YELLOW}$exe_count${NC}"
        echo -e "Build type: ${YELLOW}$BUILD_TYPE${NC}"
        echo -e "Compilador: ${YELLOW}${COMPILER:-"default"}${NC}"
        
        echo ""
        echo -e "${BLUE}Comandos útiles:${NC}"
        echo -e "  $0 -L              # Listar ejecutables"
        echo -e "  $0 -S              # Mostrar estadísticas"
        echo -e "  $0 -T              # Solo ejecutar tests"
        echo -e "  $0 -B              # Solo ejecutar benchmarks"
        
        cd ..
    fi
}

# Ejecutar solo si el script se ejecuta directamente
if [[ "${BASH_SOURCE[0]}" == "${0}" ]]; then
    main "$@"
fi