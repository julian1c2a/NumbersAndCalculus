#!/bin/bash

# Build Script Simple para AlgoritmiaCombinatoria  
# Uso: ./build_simple.sh [gcc|clang|msvc] [debug|release] [cpp_version]
# Compatible con versión .bat - ACTUALIZADO CON SOPORTE C++ STANDARDS

set -e  # Salir si cualquier comando falla

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
    echo -e "${BLUE}║              Modo Simple v2.0                ║${NC}"
    echo -e "${BLUE}╚══════════════════════════════════════════════╝${NC}"
    echo ""
    echo -e "${YELLOW}MODO SIMPLE (compatible con .bat):${NC}"
    echo ""
    echo "  $0 [compilador] [tipo] [cpp_std]"
    echo ""
    echo -e "${GREEN}COMPILADORES:${NC} gcc, clang, msvc"
    echo -e "${GREEN}TIPOS:${NC} debug, release"
    echo -e "${GREEN}ESTÁNDARES C++:${NC} 14, 17, 20, 23 (o latest para MSVC)"
    echo ""
    echo -e "${BLUE}EJEMPLOS:${NC}"
    echo "  $0 gcc debug           # GCC Debug C++17 (por defecto)"
    echo "  $0 clang release       # Clang Release C++17"
    echo "  $0 gcc debug 20        # GCC Debug C++20"
    echo "  $0 clang release 23    # Clang Release C++23"
    echo "  $0 msvc release 23     # MSVC Release C++23 (mapea a latest)"
    echo "  $0                     # GCC Release C++17 (por defecto)"
    echo ""
    echo -e "${PURPLE}FUNCIONES ADICIONALES (LEGADO):${NC}"
    echo "  $0 clean          - Limpiar archivos compilados"
    echo "  $0 test           - Ejecutar todos los programas compilados"
    echo "  $0 benchmark      - Ejecutar solo benchmarks"
}

# Función para build con CMake (modo simple)
cmake_build_simple() {
    # Limpiar build anterior
    if [[ -d "build" ]]; then
        echo -e "${YELLOW}[INFO] Limpiando build anterior...${NC}"
        rm -rf build
    fi
    
    # Crear directorio build
    mkdir -p build
    cd build
    
    # Configurar argumentos CMake
    local CMAKE_ARGS="-DCMAKE_BUILD_TYPE=$BUILD_TYPE -DCMAKE_CXX_STANDARD=$CPP_STANDARD"
    
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
            echo -e "${RED}[ERROR] MSVC no soportado en bash. Usa la versión .bat${NC}"
            cd ..
            exit 1
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
    find . -name "*.exe" -o -name "*" -type f -executable | grep -v "\\.so" | head -10
    
    cd ..
    echo ""
    echo -e "${GREEN}[INFO] Build completado en: build/${NC}"
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
            if [[ "$COMPILER" == "msvc" ]]; then
                echo -e "${YELLOW}[INFO] C++23 para MSVC se mapea a 'latest'${NC}"
                CPP_STANDARD="latest"
            else
                CPP_STANDARD="23"
            fi
            ;;
        "latest")
            if [[ "$COMPILER" == "msvc" ]]; then
                CPP_STANDARD="latest"
            else
                echo -e "${YELLOW}[WARNING] 'latest' solo válido para MSVC, usando C++23${NC}"
                CPP_STANDARD="23"
            fi
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

if [[ "$1" == "clean" ]]; then
    echo -e "${YELLOW}Limpiando archivos compilados...${NC}"
    rm -rf build_*/ *.exe *.out
    echo -e "${GREEN}Limpieza completada${NC}"
    exit 0
fi

# Manejar funciones especiales (modo legado)
if [[ "$1" == "test" ]]; then
    BUILD_TYPE="test"
    CPP_STANDARD="17" # Reset para modo legado
fi

if [[ "$1" == "benchmark" ]]; then
    BUILD_TYPE="benchmark" 
    CPP_STANDARD="17" # Reset para modo legado
fi

# Detección de modo simple (compatible con .bat)
SIMPLE_MODE="false"
if [[ "$1" =~ ^(gcc|clang|msvc)$ && "$2" =~ ^(debug|release)$ ]]; then
    SIMPLE_MODE="true"
    echo -e "${GREEN}[INFO] Modo simple: $COMPILER $BUILD_TYPE C++$CPP_STANDARD${NC}"
elif [[ "$1" == "" ]]; then
    SIMPLE_MODE="true"
    echo -e "${GREEN}[INFO] Usando configuración por defecto: GCC release C++17${NC}"
fi

# Validar argumentos
if [[ ! "$COMPILER" =~ ^(gcc|clang|msvc)$ ]]; then
    echo -e "${RED}Error: Compilador '$COMPILER' no válido${NC}"
    echo -e "Opciones válidas: gcc, clang, msvc"
    exit 1
fi

if [[ ! "$BUILD_TYPE" =~ ^(debug|release|test|benchmark)$ ]]; then
    echo -e "${RED}Error: Tipo de build '$BUILD_TYPE' no válido${NC}"
    echo -e "Opciones válidas: debug, release, test, benchmark"
    exit 1
fi

# Mostrar header solo en modo simple
if [[ "$SIMPLE_MODE" == "true" ]]; then
    echo ""
    echo -e "${PURPLE}╔══════════════════════════════════════════════╗${NC}"
    echo -e "${PURPLE}║        AlgoritmiaCombinatoria Builder        ║${NC}"
    echo -e "${PURPLE}║              Modo Simple v2.0                ║${NC}"
    echo -e "${PURPLE}╚══════════════════════════════════════════════╝${NC}"
    echo ""
    echo -e "Compilador: ${YELLOW}$COMPILER${NC}"
    echo -e "Build Type: ${YELLOW}$BUILD_TYPE${NC}"
    echo -e "Estándar C++: ${YELLOW}$CPP_STANDARD${NC}"
    echo ""
    
    # Usar CMake en modo simple (como .bat)
    cmake_build_simple
    exit 0
fi

# Modo legado para test/benchmark
echo -e "${PURPLE}╔════════════════════════════════════════╗${NC}"
echo -e "${PURPLE}║     AlgoritmiaCombinatoria Builder     ║${NC}"
echo -e "${PURPLE}╚════════════════════════════════════════╝${NC}"
echo -e "Compiler: ${YELLOW}$COMPILER${NC}"
echo -e "Build Type: ${YELLOW}$BUILD_TYPE${NC}"
echo ""

# Configurar compilador
case $COMPILER in
    gcc)
        CXX=g++
        CC=gcc
        ;;
    clang)
        CXX=clang++
        CC=clang
        ;;
    msvc)
        CXX=cl
        CC=cl
        echo -e "${RED}MSVC support no implementado en este script${NC}"
        echo -e "Usa el script build.sh con CMake para MSVC"
        exit 1
        ;;
esac

# Verificar que el compilador esté disponible
if ! command -v $CXX &> /dev/null; then
    echo -e "${RED}Error: $CXX no encontrado${NC}"
    exit 1
fi

# Configurar flags
INCLUDE_DIR="-I include"
COMMON_FLAGS="-Wall -Wextra"

case $BUILD_TYPE in
    debug)
        OPT_FLAGS="-O0 -g -DDEBUG"
        BUILD_DIR="build_${COMPILER}_debug"
        ;;
    release)
        OPT_FLAGS="-O2 -DNDEBUG"
        BUILD_DIR="build_${COMPILER}_release"
        ;;
    test|benchmark)
        OPT_FLAGS="-O2 -DNDEBUG"
        BUILD_DIR="build_${COMPILER}_release"
        ;;
esac

# Crear directorio de build
mkdir -p "$BUILD_DIR"

# Función para compilar un archivo
compile_file() {
    local file=$1
    local std_version=${2:-c++17}
    local output_name=$(basename "$file" .cpp)
    local full_path="$BUILD_DIR/$output_name"
    
    echo -n "  Compilando $(basename $file) ... "
    
    local cmd="$CXX -std=$std_version $COMMON_FLAGS $OPT_FLAGS $INCLUDE_DIR $file -o $full_path"
    
    if $cmd >/dev/null 2>&1; then
        echo -e "${GREEN}OK${NC}"
        return 0
    else
        echo -e "${RED}FAILED${NC}"
        # Mostrar error si es en modo debug
        if [[ "$BUILD_TYPE" == "debug" ]]; then
            echo -e "${YELLOW}Error details:${NC}"
            $cmd 2>&1 | head -5
        fi
        return 1
    fi
}

# Listas de archivos organizadas por categoría
BENCHMARKS=(
    "comprehensive_benchmarks.cpp"
    "simple_benchmark.cpp"
    "optimization_benchmarks.cpp"
    "memory_benchmarks.cpp"
    "minimal_benchmark.cpp"
)

TESTS=(
    "test_large_type_optimization.cpp"
    "test_noexcept_optional_system.cpp"
    "test_optional_cpp17.cpp"
    "test_optional_simple.cpp"
    "simple_test.cpp"
    "test_simple.cpp"
    "simple_test_constexpr.cpp"
    "simple_int128_test.cpp"
    "standalone_int128_test.cpp"
    "test_cpp_version.cpp"
    "test_small_type_optimizations.cpp"
    "test_optional_cpp17_demo.cpp"
)

CPP20_TESTS=(
    "tests/test_basic_traits.cpp"
    "tests/test_simple_unified.cpp"
)

DEMOS=(
    "demo_big_integers.cpp"
    "factorial_optimization_demo.cpp"
)

# Contadores
SUCCESS_COUNT=0
FAILED_COUNT=0
TOTAL_COUNT=0

# Función para compilar una lista de archivos
compile_list() {
    local list_name=$1
    local std_version=${2:-c++17}
    shift 2
    local files=("$@")
    
    if [[ ${#files[@]} -eq 0 ]]; then
        return
    fi
    
    echo -e "${BLUE}=== Compilando $list_name ===${NC}"
    
    for file in "${files[@]}"; do
        if [[ -f "$file" ]]; then
            ((TOTAL_COUNT++))
            if compile_file "$file" "$std_version"; then
                ((SUCCESS_COUNT++))
            else
                ((FAILED_COUNT++))
            fi
        fi
    done
    echo ""
}

# Función para ejecutar programas
run_programs() {
    local category=$1
    shift
    local programs=("$@")
    
    echo -e "${BLUE}=== Ejecutando $category ===${NC}"
    
    for prog in "${programs[@]}"; do
        local exec_name="$BUILD_DIR/$(basename $prog .cpp)"
        if [[ -x "$exec_name" ]]; then
            echo -e "${YELLOW}→ Ejecutando $(basename $prog .cpp)...${NC}"
            if [[ "$category" == "Benchmarks" ]]; then
                # Para benchmarks, ejecutar con --quick si está disponible
                timeout 30s "$exec_name" --quick 2>/dev/null || timeout 30s "$exec_name" 2>/dev/null || echo "  Timeout o error"
            else
                # Para tests, ejecutar normalmente
                timeout 15s "$exec_name" 2>/dev/null || echo "  Timeout o error"
            fi
            echo ""
        fi
    done
}

# Compilar según el tipo de build
case $BUILD_TYPE in
    debug|release)
        compile_list "Benchmarks" "c++17" "${BENCHMARKS[@]}"
        compile_list "Tests" "c++17" "${TESTS[@]}"
        compile_list "Tests C++20" "c++20" "${CPP20_TESTS[@]}"
        compile_list "Demos (experimental)" "c++20" "${DEMOS[@]}"
        ;;
    test)
        compile_list "Tests" "c++17" "${TESTS[@]}"
        compile_list "Tests C++20" "c++20" "${CPP20_TESTS[@]}"
        echo -e "${BLUE}=== Ejecutando Tests ===${NC}"
        run_programs "Tests" "${TESTS[@]}"
        run_programs "Tests C++20" "${CPP20_TESTS[@]}"
        ;;
    benchmark)
        compile_list "Benchmarks" "c++17" "${BENCHMARKS[@]}"
        echo -e "${BLUE}=== Ejecutando Benchmarks ===${NC}"
        run_programs "Benchmarks" "${BENCHMARKS[@]}"
        ;;
esac

# Mostrar resumen
echo -e "${PURPLE}╔════════════════════════════════════════╗${NC}"
echo -e "${PURPLE}║               RESUMEN                  ║${NC}"
echo -e "${PURPLE}╚════════════════════════════════════════╝${NC}"
echo -e "Total archivos: $TOTAL_COUNT"
echo -e "${GREEN}Exitosos: $SUCCESS_COUNT${NC}"
if [[ $FAILED_COUNT -gt 0 ]]; then
    echo -e "${RED}Fallidos: $FAILED_COUNT${NC}"
fi
echo -e "Build directory: ${YELLOW}$BUILD_DIR/${NC}"

if [[ $SUCCESS_COUNT -gt 0 ]]; then
    echo ""
    echo -e "${GREEN}=== Ejecutables disponibles ===${NC}"
    ls -la "$BUILD_DIR/" 2>/dev/null | grep -v '^d' | head -10
    
    if [[ "$BUILD_TYPE" == "release" || "$BUILD_TYPE" == "debug" ]]; then
        echo ""
        echo -e "${BLUE}Sugerencias de uso:${NC}"
        echo -e "  $0 test       - Ejecutar todos los tests"
        echo -e "  $0 benchmark  - Ejecutar todos los benchmarks"
        echo -e "  ./$BUILD_DIR/comprehensive_benchmarks --quick"
        echo -e "  ./$BUILD_DIR/test_large_type_optimization"
    fi
fi

echo ""
echo -e "${GREEN}=== Build completado ===${NC}"