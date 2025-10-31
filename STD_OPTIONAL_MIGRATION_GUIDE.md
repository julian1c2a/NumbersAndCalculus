# Sistema de Manejo de Errores con std::optional (C++17)

## Actualización del Sistema de Error Handling

Este documento describe la **VERSIÓN 2.0** del sistema de manejo de errores, que reemplaza el `CalculationResult<T>` personalizado con `std::optional<T>` estándar de C++17.

## Requisitos

**IMPORTANTE**: Este sistema requiere compilación con `-std=c++17` o superior.

```bash
g++ -std=c++17 -O2 your_file.cpp -o your_program
```

## Estrategia de Manejo de Errores por Propósito

### 1. Funciones de Cálculo → `std::optional<T>` + `noexcept`

Para funciones que realizan cálculos matemáticos o procesan datos:

```cpp
#include <optional>

// ✅ RECOMENDADO: Usar std::optional para cálculos
std::optional<int> safe_divide(int a, int b) noexcept {
    if (b == 0) {
        return std::nullopt;  // Error: división por cero
    }
    return a / b;  // Éxito
}

// Uso:
auto result = safe_divide(10, 2);
if (result.has_value()) {
    std::cout << "Resultado: " << result.value() << std::endl;
} else {
    std::cout << "Error en el cálculo" << std::endl;
}

// Sintaxis abreviada:
if (result) {
    std::cout << "Resultado: " << *result << std::endl;
}
```

### 2. Funciones de I/O → Excepciones

Para funciones que interactúan con archivos, red, base de datos, etc.:

```cpp
// ✅ CORRECTO: I/O mantiene excepciones
void save_to_file(const std::string& filename, const Data& data) {
    if (filename.empty()) {
        throw std::invalid_argument("Filename cannot be empty");
    }
    
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    
    file << data;
}

// Uso:
try {
    save_to_file("data.txt", my_data);
} catch (const std::exception& e) {
    std::cerr << "I/O Error: " << e.what() << std::endl;
}
```

### 3. Funciones Puras → `noexcept`

Para funciones que siempre tienen éxito:

```cpp
// ✅ CORRECTO: Funciones puras son noexcept
constexpr int square(int x) noexcept {
    return x * x;
}

constexpr bool is_even(int x) noexcept {
    return x % 2 == 0;
}
```

## Ventajas de std::optional sobre CalculationResult Personalizado

| Aspecto               | std::optional (C++17)               | CalculationResult (personalizado)   |
| --------------------- | ----------------------------------- | ----------------------------------- |
| **Estándar**          | ✅ Parte de C++17 estándar           | ❌ Código personalizado              |
| **Sintaxis**          | ✅ `std::nullopt`, `if (result)`     | ❌ `make_error()`, `is_valid()`      |
| **Mantenimiento**     | ✅ Sin código que mantener           | ❌ Código personalizado que mantener |
| **Herramientas**      | ✅ Soporte completo IDE/debugger     | ⚠️ Soporte limitado                  |
| **Comunidad**         | ✅ Bien conocido por desarrolladores | ❌ Curva de aprendizaje              |
| **Interoperabilidad** | ✅ Funciona con otras librerías      | ⚠️ Conversiones necesarias           |

## Migración del Sistema Anterior

### Antes (CalculationResult):
```cpp
CalculationResult<int> old_function(int x) noexcept {
    if (x < 0) {
        return make_error<int>(CalculationError::InvalidInput);
    }
    return make_success(x * 2);
}

// Uso:
auto result = old_function(5);
if (result.is_valid()) {
    std::cout << result.get_value() << std::endl;
} else {
    std::cout << "Error: " << error_to_string(result.get_error()) << std::endl;
}
```

### Después (std::optional):
```cpp
std::optional<int> new_function(int x) noexcept {
    if (x < 0) {
        return std::nullopt;
    }
    return x * 2;
}

// Uso:
auto result = new_function(5);
if (result) {
    std::cout << *result << std::endl;
} else {
    std::cout << "Error en el cálculo" << std::endl;
}
```

## Patrón para Errores Detallados (Opcional)

Cuando necesites información específica sobre el tipo de error:

```cpp
struct DetailedResult {
    std::optional<int> value;
    CalculationError error_code;
};

DetailedResult detailed_divide(int a, int b) noexcept {
    if (b == 0) {
        return {std::nullopt, CalculationError::DivisionByZero};
    }
    if (a > INT_MAX / b) {
        return {std::nullopt, CalculationError::Overflow};
    }
    return {{a / b}, CalculationError::None};
}
```

## Compilación y Testing

### Script de Compilación
```batch
@echo off
echo Compilando con C++17...
"C:\msys64\ucrt64\bin\g++.exe" -std=c++17 -Wall -Wextra -O2 your_file.cpp -o your_program.exe
if %errorlevel% equ 0 (
    echo ✓ Compilación exitosa
    your_program.exe
) else (
    echo ✗ Error de compilación
)
```

### Verificar C++17
```cpp
#include <iostream>

int main() {
    std::cout << "C++ standard: " << __cplusplus << std::endl;
    
    #if __cplusplus >= 201703L
        std::cout << "✓ C++17 or later supported" << std::endl;
    #else
        std::cout << "❌ C++17 required" << std::endl;
    #endif
    
    return 0;
}
```

## Ejemplos Completos

### Función de Lookup Table Refactorizada
```cpp
// Antes:
constexpr CalculationResult<int8_t> get_power_of_2_int8_safe(int exp) noexcept {
    if (exp < 0 || exp >= 7) {
        return make_error<int8_t>(CalculationError::OutOfRange);
    }
    return make_success(power_of_2_table[exp]);
}

// Después:
constexpr std::optional<int8_t> get_power_of_2_int8_safe(int exp) noexcept {
    if (exp < 0 || exp >= 7) {
        return std::nullopt;
    }
    return power_of_2_table[exp];
}
```

## Conclusión

La migración a `std::optional` simplifica el código, mejora la mantenibilidad y sigue estándares establecidos. El único requisito es compilar con C++17 o superior, lo cual es ampliamente soportado en compiladores modernos.

**Próximos pasos**:
1. Actualizar flags de compilación a `-std=c++17`
2. Refactorizar funciones existentes gradualmente
3. Mantener funciones legacy con `@deprecated` durante la transición