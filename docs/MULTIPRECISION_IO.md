# Sistema I/O Unificado para Multiprecisión

## Descripción

Este sistema unifica las funciones de entrada/salida (I/O) para tipos de precisión arbitraria, proporcionando una interfaz consistente que funciona tanto con `__int128` como con tipos de **Boost.Multiprecision**.

## Archivos del Sistema

### Archivos Principales

1. **`int128_io.hpp`** - I/O para tipos `__int128` y `unsigned __int128`
2. **`boost_multiprecision_io.hpp`** - I/O para tipos Boost.Multiprecision
3. **`multiprecision_io_compatible.hpp`** - Sistema unificado compatible con C++14/17

### Archivos de Prueba

1. **`test_int128_io.cpp`** - Pruebas para int128_io.hpp
2. **`test_boost_mp_io.cpp`** - Pruebas para boost_multiprecision_io.hpp
3. **`test_simple_unified.cpp`** - Pruebas del sistema unificado

## Características Principales

### Funciones Unificadas

```cpp
// Funciona con cualquier tipo de precisión arbitraria
template<typename T>
std::string universal_to_string(const T& value, int base = 10);

template<typename T>
std::string universal_to_hex(const T& value, bool uppercase = true, bool prefix = true);

template<typename T>
size_t universal_count_digits(const T& value);

template<typename T>
std::string universal_format_thousands(const T& value, char separator = ',');
```

### Detección Automática de Tipos

El sistema utiliza SFINAE (C++14/17 compatible) para detectar automáticamente:
- Si el tipo tiene método `.str()`
- Si el tipo tiene método `.str(int base)`
- Si Boost.Multiprecision está disponible

## Compatibilidad

### Compiladores Soportados
- **GCC** 7.0+ (para `__int128`)
- **Clang** 6.0+ (para `__int128`)
- **MSVC** (tipos estándar solamente)

### Estándares C++
- **C++14** - Mínimo requerido
- **C++17** - Recomendado
- **C++20+** - Soporte completo

### Dependencias Opcionales
- **Boost.Multiprecision** - Para tipos de precisión arbitraria completos
- **GMP** - Backend de alto rendimiento (opcional)
- **TomMath** - Backend alternativo (opcional)

## Instalación y Uso

### Uso Básico (Solo __int128)

```cpp
#include "multiprecision_io_compatible.hpp"
using namespace multiprecision_io_compat;

int main() {
    __int128 big_number = static_cast<__int128>(123456789012345LL) * 1000000LL;
    
    std::cout << "Valor: " << universal_to_string(big_number) << std::endl;
    std::cout << "Hex: " << universal_to_hex(big_number) << std::endl;
    std::cout << "Con separadores: " << universal_format_thousands(big_number, ',') << std::endl;
    
    return 0;
}
```

### Compilación

```bash
# Básico (solo __int128)
g++ -std=c++17 -I../include tu_programa.cpp -o tu_programa

# Con Boost.Multiprecision
g++ -std=c++17 -I../include -DHAS_BOOST_AVAILABLE tu_programa.cpp -lboost_system -o tu_programa
```

### Uso con Boost.Multiprecision

```cpp
#include <boost/multiprecision/cpp_int.hpp>
#include "multiprecision_io_compatible.hpp"

using namespace multiprecision_io_compat;
using namespace mp; // Namespace mp para tipos Boost.Multiprecision

int main() {
    cpp_int huge_number("123456789012345678901234567890123456789012345678901234567890");
    
    std::cout << "Valor: " << universal_to_string(huge_number) << std::endl;
    std::cout << "Dígitos: " << universal_count_digits(huge_number) << std::endl;
    std::cout << "Formateado: " << universal_format_thousands(huge_number, '.') << std::endl;
    
    // Tipos de precisión fija
    int256_t fixed_256("98765432109876543210987654321098765432109876543210");
    std::cout << "int256_t: " << to_string(fixed_256) << std::endl;
    std::cout << "int256_t hex: " << to_hex_string(fixed_256) << std::endl;
    
    return 0;
}
```

## Tipos de Precisión Fija Soportados

### Enteros de Precisión Fija

```cpp
// Tipos disponibles en el namespace mp
using int128_t = number<cpp_int_backend<128, 128, signed_magnitude, unchecked, void>>;
using uint128_t = number<cpp_int_backend<128, 128, unsigned_magnitude, unchecked, void>>;
using int256_t = number<cpp_int_backend<256, 256, signed_magnitude, unchecked, void>>;
using uint256_t = number<cpp_int_backend<256, 256, unsigned_magnitude, unchecked, void>>;
using int512_t = number<cpp_int_backend<512, 512, signed_magnitude, unchecked, void>>;
using uint512_t = number<cpp_int_backend<512, 512, unsigned_magnitude, unchecked, void>>;
using int1024_t = number<cpp_int_backend<1024, 1024, signed_magnitude, unchecked, void>>;
using uint1024_t = number<cpp_int_backend<1024, 1024, unsigned_magnitude, unchecked, void>>;
```

### Punto Flotante de Precisión Fija

```cpp  
// Tipos de punto flotante decimal
using cpp_dec_float_50 = cpp_dec_float<50>;   // 50 dígitos decimales
using cpp_dec_float_100 = cpp_dec_float<100>; // 100 dígitos decimales

// Tipos de punto flotante binario
using cpp_bin_float_quad = cpp_bin_float_quad; // Precisión cuádruple
using cpp_bin_float_oct = cpp_bin_float_oct;   // Precisión óctuple
```

### Funciones Template Genéricas

```cpp
// Estas funciones funcionan con cualquier tipo de precisión fija
template<typename Backend>
std::string to_string(const number<Backend>& value, int base = 10, bool uppercase = false);

template<typename Backend>
std::string to_hex_string(const number<Backend>& value, bool uppercase = true, bool prefix = true);

template<typename Backend>
size_t count_digits(const number<Backend>& value);

template<typename Backend>
std::string format_with_thousands_separator(const number<Backend>& value, char separator = ',');

template<typename Backend>
std::istream& safe_input(std::istream& is, number<Backend>& value);
```

## Ejemplos de Uso

### Formateo con Separadores

```cpp
__int128 num = static_cast<__int128>(1234567890123456LL);

// Diferentes separadores
std::cout << universal_format_thousands(num, ',') << std::endl;  // 1,234,567,890,123,456
std::cout << universal_format_thousands(num, '.') << std::endl;  // 1.234.567.890.123.456
std::cout << universal_format_thousands(num, '\'') << std::endl; // 1'234'567'890'123'456
```

### Conversiones Hexadecimales

```cpp
__int128 num = 255;

std::cout << universal_to_hex(num, true, true) << std::endl;   // 0XFF
std::cout << universal_to_hex(num, false, true) << std::endl;  // 0xff
std::cout << universal_to_hex(num, true, false) << std::endl;  // FF
std::cout << universal_to_hex(num, false, false) << std::endl; // ff
```

### Cálculos Combinatorios

```cpp
// Calcular factoriales grandes
__int128 factorial = 1;
for (int i = 1; i <= 25; ++i) {
    factorial *= i;
    std::cout << i << "! = " << universal_format_thousands(factorial, ',') 
              << " (" << universal_count_digits(factorial) << " dígitos)" << std::endl;
}
```

## Ventajas del Sistema Unificado

1. **Interfaz Consistente**: Una sola API para todos los tipos
2. **Detección Automática**: No necesitas saber qué tipo estás usando
3. **Compatibilidad**: Funciona con y sin Boost.Multiprecision
4. **Rendimiento**: Utiliza la implementación más eficiente disponible
5. **Extensibilidad**: Fácil agregar soporte para nuevos tipos

## Detección de Características

El sistema puede detectar automáticamente:

```cpp
// Información del sistema
show_system_info();

// Salida ejemplo:
// ✅ __int128 disponible
// ✅ Boost.Multiprecision disponible
// ✅ Backend GMP disponible
// Estándar C++: C++17
```

## Manejo de Errores

```cpp
try {
    __int128 result = from_string_i128("invalid_number");
} catch (const std::invalid_argument& e) {
    std::cerr << "Error de conversión: " << e.what() << std::endl;
} catch (const std::out_of_range& e) {
    std::cerr << "Número fuera de rango: " << e.what() << std::endl;
}
```

## FAQ

### ¿Qué hago si no tengo Boost.Multiprecision?

El sistema funciona perfectamente solo con `__int128`. Todas las funciones unificadas trabajarán con tipos estándar y `__int128`.

### ¿Puedo mezclar tipos diferentes?

Sí, las funciones template funcionan con cualquier tipo que tengas disponible:

```cpp
int small = 42;
__int128 medium = 123456789012345LL;
// cpp_int large("123456789..."); // Si tienes Boost

std::cout << universal_to_string(small) << std::endl;
std::cout << universal_to_string(medium) << std::endl;
// std::cout << universal_to_string(large) << std::endl;
```

### ¿Es compatible con proyectos existentes?

Sí, el sistema es completamente header-only y no interfiere con código existente. Simplemente incluye los headers necesarios.

## Contribuciones

Este sistema forma parte del proyecto **AlgoritmiaCombinatoria**. Para contribuir:

1. Fork el repositorio
2. Crea una rama para tu feature
3. Implementa pruebas para nuevas funcionalidades
4. Envía un pull request

## Licencia

Este código está bajo la Licencia MIT. Ver el archivo `LICENSE` para detalles completos.

## Autor

**Julian Calderon Almendros (julian1c2a)**  
Email: julian.calderon.almendros@gmail.com  
Proyecto: AlgoritmiaCombinatoria  
Año: 2024