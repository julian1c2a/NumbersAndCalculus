# Guión de Construcción del Proyecto AlgoritmiaCombinatoria

## Objetivo del Proyecto

Crear un proyecto C++ avanzado de algoritmia combinatoria con las siguientes características:

### Requisitos Funcionales

1. **Biblioteca de Combinatoria Avanzada**
   - Implementar algoritmos de factorial, combinaciones, permutaciones
   - Soporte para números enteros de 128 bits (`__int128`)
   - Funciones constexpr para cálculos en tiempo de compilación
   - Literales personalizados para facilitar el uso
   - Soporte completo de E/S para tipos `__int128`

2. **Sistema de Tipos Avanzado**
   - Traits para detectión automática de tipos numéricos
   - Soporte para Boost.Multiprecision
   - Integración con tipos estándar y extendidos

3. **Aplicaciones de Demostración**
   - Demo principal interactivo
   - Demo de optimización de factoriales
   - Suite completa de pruebas unitarias

### Requisitos Técnicos

1. **Multi-Compilador**
   - GCC (MSYS2/MinGW)
   - Clang (MSYS2)
   - MSVC (Visual Studio 2022)

2. **Multi-Estándar**
   - C++17, C++20, C++23

3. **Sistema de Build Robusto**
   - CMake con Ninja
   - Scripts de build automatizados
   - **Directorios separados por configuración**: `build_[compilador]_[tipo]_cpp[estándar]`
   - Ejemplos: `build_gcc_debug_cpp17`, `build_clang_release_cpp20`, `build_msvc_release_cpp23`

4. **Gestión de Dependencias**
   - Boost (via vcpkg para MSVC, sistema para GCC/Clang)
   - Catch2 (como dependencia de CMake)
   - FetchContent para dependencias automáticas

### Estructura de Archivos Requerida

```
AlgoritmiaCombinatoria/
├── CMakeLists.txt                          # Build system principal
├── README.md                              # Documentación
├── include/
│   ├── combinatorics.hpp                  # Header principal de la biblioteca
│   ├── constexpr_literals.hpp             # Literales constexpr y funciones
│   ├── int128_io.hpp                      # E/O completa para __int128
│   └── number_type_traits.hpp             # Traits de tipos numéricos
├── src/
│   ├── main.cpp                          # Demo principal interactivo
│   └── combinatorics.cpp                 # Implementación de la biblioteca
├── tests/
│   └── test_combinatorics.cpp            # Suite de pruebas unitarias
├── factorial_optimization_demo.cpp        # Demo de optimizaciones
└── scripts de build/
    ├── build_multi_final.bat             # Script Windows multi-directorio
    └── build_simple.sh                   # Script bash corregido
```

### Características Específicas de Implementación

#### 1. Literales Constexpr Avanzados
```cpp
// Debe permitir uso como:
auto fact = 20_i128;
auto power = 10_pow2_i128;  // 2^10
auto combo = constexpr_combination(50_i128, 25_i128);
```

#### 2. Sistema de E/O Completo para __int128
```cpp
// Debe soportar:
__int128 big_number = 123456789012345678901234567890_i128;
std::cout << big_number << std::endl;
std::cin >> big_number;
std::string str = int128_io::to_string(big_number);
```

#### 3. Traits de Detección Automática
```cpp
// Debe detectar automáticamente tipos numéricos:
template<typename T>
constexpr bool is_extended_integer_v = /* detección automática */;
```

#### 4. CMakeLists.txt Multi-Compilador
- Detección automática de compilador
- Configuración específica por compilador
- Integración con vcpkg (MSVC) y sistema (GCC/Clang)
- FetchContent para Catch2

### Scripts de Build Requeridos

#### 1. Script Multi-Directorio (build_multi_final.bat)
```bash
# Debe permitir:
./build_multi_final.bat gcc debug 17      # → build_gcc_debug_cpp17/
./build_multi_final.bat clang release 20  # → build_clang_release_cpp20/
./build_multi_final.bat msvc release 23   # → build_msvc_release_cpp23/
```

#### 2. Script Bash No-Bloqueante (build_simple.sh)
- **CRÍTICO**: No debe colgarse en "STOP 1"
- Debe terminar en segundos, no minutos
- Evitar funciones complejas de detección de versiones
- Manejo simple y directo de dependencias

### Problemas Conocidos a Evitar

1. **Script Bash Colgado**
   - No usar subshells complejos para detección de versiones
   - Evitar funciones que causen deadlocks en MSYS2
   - Implementar timeouts y salidas de emergencia

2. **Dependencias de Boost**
   - MSVC requiere vcpkg
   - GCC/Clang usan instalación del sistema
   - Manejar graciosamente la ausencia de dependencias

3. **Compatibilidad Multi-Compilador**
   - Warnings específicos por compilador
   - Flags de compilación diferenciados
   - Rutas de herramientas específicas

### Resultados Esperados

Al final del proyecto debe:

1. **Compilar exitosamente** en todas las configuraciones
2. **Ejecutar pruebas** sin errores
3. **Generar directorios separados** por configuración
4. **Scripts funcionales** que no se cuelguen
5. **Demos interactivos** que muestren las capacidades

### Comandos de Verificación

```bash
# Debe funcionar sin colgarse:
./build_multi_final.bat gcc debug 17
./build_multi_final.bat clang release 20  
./build_multi_final.bat msvc release 23

# Script bash debe terminar rápido:
bash build_simple.sh

# Debe generar estos directorios:
build_gcc_debug_cpp17/
build_clang_release_cpp20/
build_msvc_release_cpp23/
```

### Notas Importantes

- **Prioridad máxima**: Sistema multi-directorio funcional
- **Prioridad alta**: Scripts que no se cuelguen
- **Foco técnico**: __int128, constexpr, y combinatoria avanzada
- **Compatibilidad**: Windows con MSYS2, Visual Studio, y herramientas nativas

---

## Instrucciones de Uso del Guión

Dame este guión y yo construiré el proyecto completo con:
- Todos los archivos fuente
- Sistema de build robusto
- Scripts automatizados
- Configuración multi-compilador
- Tests y documentación

El resultado será un proyecto C++ avanzado de combinatoria con sistema de build multi-directorio completamente funcional.