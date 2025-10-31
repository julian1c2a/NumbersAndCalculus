# AlgoritmiaCombinatoria

cmake_policy(SET CMP0167 NEW)
find_package(Boost 1.80 REQUIRED)

Un proyecto C++23 para algoritmos combinatorios usando Boost::multiprecision y Catch2 para pruebas unitarias.

## Características

- **C++23**: Utiliza las últimas características del estándar C++23
- **Multiplataforma**: Compatible con GCC, Clang y MSVC
- **Boost::multiprecision**: Para cálculos con enteros y racionales de precisión arbitraria
- **Catch2**: Framework moderno para pruebas unitarias
- **CMake**: Sistema de construcción multiplataforma

## Funcionalidades

### 🚀 Literales Constexpr Optimizados
- **Evaluación en compile-time**: Cálculos con cero overhead en runtime  
- **Literales __int128**: `123456789_i128`, `64_pow2_i128`, `999_ui128`
- **Funciones matemáticas constexpr**: Factorial, combinaciones, permutaciones
- **Constantes precalculadas**: Factoriales hasta 20!, potencias de 2, Fibonacci

### 📊 Sistema I/O Unificado para Multiprecisión ⭐
- **Interfaz unificada**: API única para `__int128` y tipos Boost.Multiprecision
- **Detección automática**: Templates que funcionan con cualquier tipo de precisión arbitraria
- **Conversión bidireccional**: String ↔ números con manejo robusto de errores
- **Múltiples formatos**: Decimal, hexadecimal, binario, octal con prefijos automáticos
- **Formateo avanzado**: Separadores de miles, conteo de dígitos, validación
- **Compatible C++14/17**: Funciona con y sin Boost.Multiprecision instalado

### 📊 Tipos de Precisión Arbitraria
- **Boost.Multiprecision**: `cpp_int`, `mpz_int`, `tom_int` con UDL
- **__int128 I/O completo**: Operadores `<<`, `>>`, conversiones string optimizadas
- **Manejo de overflow**: Detección robusta de límites y validación de entrada

### 🧮 Algoritmos Combinatorios
- Cálculo de factoriales con precisión arbitraria
- Combinaciones C(n,k) y permutaciones P(n,k) optimizadas
- Números de Fibonacci constexpr hasta F(100)
- Números de Catalan constexpr
- Generación de todas las permutaciones y combinaciones
- Coeficientes binomiales con números racionales

## Estructura del Proyecto

```
AlgoritmiaCombinatoria/
├── CMakeLists.txt              # Configuración de CMake
├── README.md                   # Este archivo
├── LICENSE                     # Licencia MIT
├── .gitignore                  # Archivos a ignorar
├── build.sh                    # Script de construcción Unix/Linux
├── build.bat                   # Script de construcción Windows
├── .vscode/                    # Configuración VS Code
│   ├── tasks.json             # Tareas de construcción
│   ├── launch.json            # Configuración de depuración
│   └── c_cpp_properties.json  # Propiedades IntelliSense
├── build/                      # Directorio de construcción
├── include/                           # Archivos de cabecera
│   ├── custom_literals.hpp           # Literales UDL para Boost.Multiprecision
│   ├── constexpr_literals.hpp        # Literales constexpr optimizados  
│   ├── int128_io.hpp                 # I/O completo para __int128
│   ├── boost_multiprecision_io.hpp   # I/O para tipos Boost.Multiprecision ⭐
│   ├── multiprecision_io_compatible.hpp  # Sistema I/O unificado ⭐
│   ├── numeric_limits.hpp            # Especializations std::numeric_limits
│   ├── combinatorics.hpp             # Interfaz principal (legacy)
│   └── combinatorics_impl.hpp        # Implementaciones template (legacy)
├── src/                        # Código fuente
│   ├── main.cpp               # Programa principal
│   └── combinatorics.cpp      # Implementaciones
└── tests/                      # Pruebas unitarias
    ├── test_main.cpp          # Configuración de Catch2
    └── test_combinatorics.cpp # Pruebas de funciones
```

## Requisitos

### Software Necesario

- **Compilador C++23**:
  - GCC 11+ con soporte para C++23
  - Clang 14+ con soporte para C++23
  - MSVC 2022 17.0+ (Visual Studio 2022)
- **CMake 3.25+**
- **Boost 1.80+** (especialmente Boost.Multiprecision y Boost.Math)

### Instalación de Dependencias

#### Windows (MSYS2/MinGW-w64)
```bash
# Actualizar paquetes
pacman -Syu

# Instalar compiladores y herramientas
pacman -S mingw-w64-ucrt-x86_64-gcc
pacman -S mingw-w64-ucrt-x86_64-clang
pacman -S mingw-w64-ucrt-x86_64-cmake
pacman -S mingw-w64-ucrt-x86_64-ninja

# Instalar Boost
pacman -S mingw-w64-ucrt-x86_64-boost
```

#### Windows (vcpkg)
```cmd
# Instalar Boost
vcpkg install boost-multiprecision boost-math boost-rational

# Instalar Catch2
vcpkg install catch2
```

#### Ubuntu/Debian
```bash
sudo apt update
sudo apt install build-essential cmake ninja-build
sudo apt install libboost-all-dev
```

#### macOS
```bash
brew install cmake ninja boost
```

## Compilación

### Opción 1: Con CMake (Recomendado)

```bash
# Crear directorio de construcción
mkdir build
cd build

# Configurar el proyecto
cmake .. -G "Ninja"

# O para Visual Studio en Windows
cmake .. -G "Visual Studio 17 2022"

# Compilar
cmake --build . --config Release

# Ejecutar pruebas
ctest --output-on-failure
```

### Opción 2: Con vcpkg (Windows)

```bash
# Configurar con vcpkg
cmake .. -DCMAKE_TOOLCHAIN_FILE="C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake"

# Compilar
cmake --build . --config Release
```

### Opción 3: Compilación Manual

#### Con GCC
```bash
g++ -std=c++23 -O3 -I include src/*.cpp -lboost_system -o AlgoritmiaCombinatoria
```

#### Con Clang
```bash
clang++ -std=c++23 -O3 -I include src/*.cpp -lboost_system -o AlgoritmiaCombinatoria
```

#### Con MSVC
```cmd
cl /std:c++23 /EHsc /O2 /I include src\*.cpp /Fe:AlgoritmiaCombinatoria.exe
```

## Uso

### Ejecutar el Programa Principal
```bash
./AlgoritmiaCombinatoria
```

### Ejecutar Pruebas
```bash
./tests
```

### Configuración Específica para MSYS2

El proyecto está optimizado para usar MSYS2 UCRT64. Las rutas configuradas son:

- **Compiladores**: `C:/msys64/ucrt64/bin/g++.exe`, `C:/msys64/ucrt64/bin/clang++.exe`
- **Debuggers**: `C:/msys64/ucrt64/bin/gdb.exe`, `C:/msys64/ucrt64/bin/lldb.exe`
- **Includes**: `C:/msys64/ucrt64/include/boost`
- **Libs**: Las librerías se detectan automáticamente via CMake

**Comandos de instalación para MSYS2:**
```bash
# Actualizar sistema
pacman -Syu

# Instalar compiladores y herramientas
pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-clang
pacman -S mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-ninja

# Instalar Boost y Catch2
pacman -S mingw-w64-ucrt-x86_64-boost
pacman -S mingw-w64-ucrt-x86_64-catch2
```

### Ejemplo de Uso con Literales Constexpr

```cpp
#include "include/constexpr_literals.hpp"
#include "include/int128_io.hpp"
#include <iostream>

using namespace constexpr_literals;
using namespace int128_io;

int main() {
    // ✅ Literales constexpr - evaluados en compile-time (cero overhead)
    constexpr auto big_number = 123456789012345_i128;
    constexpr auto power_of_2 = 64_pow2_i128;  // 2^64
    constexpr auto factorial = constexpr_factorial(20);
    constexpr auto combination = constexpr_combination(50, 25);
    
    // I/O con __int128
    std::cout << "Número grande: " << big_number << std::endl;
    std::cout << "2^64: " << power_of_2 << std::endl;
    std::cout << "20!: " << factorial << std::endl;
    std::cout << "C(50,25): " << combination << std::endl;
    
    // Conversiones string con __int128
    __int128 from_str = from_string_i128("999999999999999999999999999999");
    std::string to_str = to_string(from_str);
    
    return 0;
}
```

### Ejemplo con Boost.Multiprecision UDL

```cpp
#include "include/custom_literals.hpp"
#include <iostream>

using namespace custom_literals;

int main() {
    // Literales definidos por el usuario para números gigantescos
    auto big1 = "123456789012345678901234567890123456789"_cpp_int;
    auto big2 = 987654321_cpp_int;
    auto binary = "11110000111100001111000011110000"_bin;
    auto hex = "DEADBEEFCAFEBABE123456789ABCDEF0"_hex;
    
    std::cout << "Número gigantesco: " << big1 << std::endl;
    std::cout << "Binario: " << binary << std::endl;
    std::cout << "Hexadecimal: " << hex << std::endl;
    
    return 0;
}
```

## Configuración de Compiladores

### GCC
- Usar `-std=c++23` o `-std=c++2b`
- Flags recomendados: `-Wall -Wextra -Wpedantic -O3`

### Clang
- Usar `-std=c++23` o `-std=c++2b`
- Flags recomendados: `-Wall -Wextra -Wpedantic -O3`

### MSVC
- Usar `/std:c++23` o `/std:c++latest`
- Flags recomendados: `/W4 /O2 /permissive-`

## Características de C++23 Utilizadas

- **Concepts**: Para restricciones de tipos
- **Requires clauses**: Para validación de templates
- **Enhanced constexpr**: Para cálculos en tiempo de compilación
- **Improved auto**: Para deducción de tipos mejorada

## Contribuir

1. Fork el proyecto
2. Crea una rama para tu característica (`git checkout -b feature/AmazingFeature`)
3. Commit tus cambios (`git commit -m 'Add some AmazingFeature'`)
4. Push a la rama (`git push origin feature/AmazingFeature`)
5. Abre un Pull Request

## Licencia

Este proyecto está bajo la Licencia MIT - ver el archivo [LICENSE](LICENSE) para detalles.

## Autor

- Julián Calderón Almendros - [@julian1c2a](https://github.com/julian1c2a)

## Problemas Conocidos

- En MSVC, algunas características de C++23 pueden requerir `/std:c++latest`
- Boost::multiprecision requiere al menos Boost 1.80
- Catch2 v3 es necesario para compatibilidad con C++23