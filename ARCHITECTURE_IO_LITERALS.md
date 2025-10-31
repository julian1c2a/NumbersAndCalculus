/**
 * @file ARCHITECTURE_IO_LITERALS.md
 * @brief Propuesta de arquitectura modular para utilidades de IO y literales
 * @author AlgoritmiaCombinatoria Project
 * @date 2024
 * @version 1.0
 *
 * Este documento describe la nueva arquitectura modular propuesta para
 * organizar las utilidades de entrada/salida (IO) y literales de usuario
 * del proyecto AlgoritmiaCombinatoria.
 */

# Arquitectura Modular para IO y Literales de Usuario

## 🏗️ Estructura de Directorios Propuesta

```
AlgoritmiaCombinatoria/
├── include/
│   ├── io/                                    # 📁 Módulo de entrada/salida
│   │   ├── io.hpp                            # Header principal unificado
│   │   ├── integral_types/                   # 📁 IO para tipos enteros
│   │   │   └── int128_io.hpp                # IO para __int128
│   │   └── multiprecision_types/            # 📁 IO para tipos multiprecisión
│   │
│   ├── literals/                             # 📁 Módulo de literales de usuario
│   │   ├── literals.hpp                     # Header principal unificado
│   │   ├── integral_literals/               # 📁 Literales para tipos enteros
│   │   │   └── int128_literals.hpp          # Literales _i128, _u128, _hex, _bin
│   │   └── multiprecision_literals/         # 📁 Literales multiprecisión
│   │
│   ├── type_traits/                         # 📁 Traits de tipos
│   │   └── extended_integral_traits.hpp    # Traits extendidos
│   │
│   └── number_calc/                         # 📁 Cálculo numérico
│       └── integral_power_functions/       # Funciones de potencias modulares
```

## 🎯 Categorización por Funcionalidad

### **1. Módulo IO (`include/io/`)**

#### **A. Tipos Integrales (`io/integral_types/`)**
- **int128_io.hpp**: Operadores `<<`, `>>` y conversiones para `__int128`
- **standard_io.hpp**: Extensiones para tipos estándar (formato, precisión)
- **formatted_output.hpp**: Salida formateada (hexadecimal, binario, octal)

#### **B. Tipos Multiprecisión (`io/multiprecision_types/`)**
- **boost_multiprecision_io.hpp**: IO para `cpp_int`, `cpp_dec_float`, etc.
- **gmp_io.hpp**: IO para tipos GMP (si Boost.Multiprecision usa GMP)
- **conversion_utilities.hpp**: Funciones de conversión string ↔ número

### **2. Módulo Literales (`include/literals/`)**

#### **A. Literales Integrales (`literals/integral_literals/`)**
- **int128_literals.hpp**: 
  ```cpp
  auto big_num = 123456789012345678901234567890_i128;
  auto ubig_num = 123456789012345678901234567890_u128;
  ```

- **constexpr_literals.hpp**: Literales que funcionen en tiempo de compilación
- **binary_hex_literals.hpp**: 
  ```cpp
  auto binary = 0b1010101010101010_i128;
  auto hex = 0xDEADBEEFCAFEBABE_i128;
  ```

#### **B. Literales Multiprecisión (`literals/multiprecision_literals/`)**
- **cpp_int_literals.hpp**:
  ```cpp
  auto huge = 123456789012345678901234567890123456789_cpp_int;
  auto precise = 3.14159265358979323846264338327950288_cpp_dec;
  ```

- **fixed_precision_literals.hpp**: Para tipos de precisión fija
- **rational_literals.hpp**: Para números racionales `1/3_rational`

## 🚀 API Unificada Propuesta

### **Header Principal IO (`io/io.hpp`)**
```cpp
#include "io.hpp"
using namespace io;

// Uso automático de IO optimizado para cualquier tipo
std::cout << some_int128_number << std::endl;        // Automático
std::cout << some_cpp_int_number << std::endl;       // Automático
std::cout << format_hex(number) << std::endl;        // Formateo
std::cout << format_binary(number, 64) << std::endl; // Formateo con ancho
```

### **Header Principal Literales (`literals/literals.hpp`)**
```cpp
#include "literals.hpp"
using namespace literals;

// Todos los literales disponibles automáticamente
auto n1 = 123_i128;                    // __int128
auto n2 = 456_u128;                    // unsigned __int128  
auto n3 = 789_cpp_int;                 // Boost cpp_int
auto n4 = 3.14159_cpp_dec_50;          // 50 dígitos decimales
auto n5 = 1/3_rational;                // Número racional
auto n6 = 0xDEADBEEF_i128;            // Hexadecimal
auto n7 = 0b10101010_i128;            // Binario
```

## 📋 Plan de Migración

### **Fase 1: Crear Estructura Modular**
1. ✅ Crear directorios `io/` y `literals/`
2. ✅ Crear subdirectorios especializados
3. 🔄 Migrar archivos existentes a nueva estructura
4. 🔄 Crear headers principales unificados

### **Fase 2: Refactorizar Archivos Existentes**
- `int128_io.hpp` → `io/integral_types/int128_io.hpp`
- `boost_multiprecision_io.hpp` → `io/multiprecision_types/boost_multiprecision_io.hpp`
- `custom_literals.hpp` → Dividir en módulos especializados
- `custom_literals_io.hpp` → Integrar en módulos IO correspondientes

### **Fase 3: API Unificada**
- Crear `io/io.hpp` que incluya todos los módulos IO
- Crear `literals/literals.hpp` que incluya todos los literales
- Actualizar `number_calc.hpp` para incluir los nuevos módulos

## 💡 Ventajas de esta Arquitectura

### **🎯 Escalabilidad**
- Fácil agregar nuevos tipos (ej: decimal128, float128)
- Cada tipo de IO en su módulo especializado
- Crecimiento sin saturar namespace

### **🧩 Modularidad**
- Incluir solo lo que necesitas
- Compilación más rápida (headers específicos)
- Mantenimiento simplificado

### **🔧 Flexibilidad**
- API unificada para uso simple
- API especializada para uso avanzado
- Compatible con arquitectura existente

### **📚 Organización Clara**
- Separación lógica: IO vs Literales
- Separación por tipos: Integral vs Multiprecision
- Documentación bien estructurada

## 🎁 Beneficios para el Usuario

```cpp
// Uso simple (todo incluido)
#include "number_calc.hpp"
using namespace number_calc;

// Uso específico (solo lo necesario)
#include "io/integral_types/int128_io.hpp"     // Solo IO int128
#include "literals/integral_literals/int128_literals.hpp"  // Solo literales

// O incluir todo
#include "io/io.hpp"                           // Todo el IO
#include "literals/literals.hpp"               // Todos los literales

// Uso avanzado (control total)
#include "io/multiprecision_types/boost_multiprecision_io.hpp"
#include "literals/multiprecision_literals/cpp_int_literals.hpp"
```

Esta arquitectura mantiene la **compatibilidad hacia atrás** mientras proporciona una **organización escalable** para el futuro crecimiento del proyecto.

// API simple
#include "number_calc.hpp"  // Todo el proyecto
using namespace number_calc;

// API especializada  
#include "io/io.hpp"
#include "literals/literals.hpp"
using namespace io;
using namespace literals;

int main() {
    // Literales funcionan automáticamente
    auto big = 123456789012345678901234567890_i128;
    auto hex = "DEADBEEF"_i128_hex;
    auto bin = "10101010"_i128_bin;
    
    // IO funciona automáticamente  
    std::cout << big << std::endl;              // ✅ Automático
    std::cout << format_hex(big) << std::endl;  // ✅ Formateo
    
    // Funciones de potencias integradas
    auto result = int_power_smart(big, 2);      // ✅ Potencias optimizadas
    
    return 0;
}