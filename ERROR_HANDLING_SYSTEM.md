# 🛡️ SISTEMA DE MANEJO DE ERRORES DIFERENCIADO

## 📋 ESTRATEGIA IMPLEMENTADA

### 🎯 **Principio Fundamental**
- **Funciones de CÁLCULO**: `CalculationResult<T>` + `noexcept`
- **Funciones de I/O**: Excepciones (`throw`/`catch`)
- **Funciones PURAS**: `noexcept` siempre que sea posible

## 🔧 IMPLEMENTACIÓN TÉCNICA

### 📊 **CalculationResult<T>**
```cpp
template<typename T>
struct CalculationResult {
    T value;                    // Valor del resultado
    CalculationError error;     // Código de error
    
    constexpr bool is_valid() const noexcept;
    constexpr bool has_error() const noexcept;
    constexpr T get_value() const noexcept;
    constexpr CalculationError get_error() const noexcept;
};
```

### 🚨 **Códigos de Error**
```cpp
enum class CalculationError {
    None = 0,           // Sin error
    OutOfRange,         // Parámetro fuera del rango válido
    Overflow,           // Overflow aritmético
    Underflow,          // Underflow aritmético
    InvalidInput,       // Entrada inválida
    DivisionByZero      // División por cero
};
```

## 📚 EJEMPLOS DE USO

### ✅ **Funciones de Cálculo (noexcept)**
```cpp
// ANTES (con excepciones):
constexpr int8_t get_power_of_2_int8(int exp) {
    if (exp < 0 || exp >= 7) {
        throw std::out_of_range("Fuera de rango");  // ❌ NO noexcept
    }
    return power_table[exp];
}

// DESPUÉS (con CalculationResult):
constexpr CalculationResult<int8_t> get_power_of_2_int8_safe(int exp) noexcept {
    if (exp < 0 || exp >= 7) {
        return make_error<int8_t>(CalculationError::OutOfRange);  // ✅ noexcept
    }
    return make_success(power_table[exp]);
}
```

### 🔄 **Uso en Código Cliente**
```cpp
// Manejo de errores sin excepciones
auto result = get_power_of_2_int8_safe(5);
if (result.is_valid()) {
    std::cout << "Resultado: " << result.get_value() << std::endl;
} else {
    std::cout << "Error: " << error_to_string(result.get_error()) << std::endl;
}
```

### 📤 **Funciones de I/O (con excepciones)**
```cpp
// CORRECTO para I/O:
void print_power_result(int exp) {  // NO noexcept
    if (exp < 0) {
        throw std::invalid_argument("Exponente inválido para salida");
    }
    
    auto calc_result = get_power_of_2_int8_safe(exp);  // Función de cálculo
    if (!calc_result.is_valid()) {
        throw std::runtime_error("No se puede imprimir resultado inválido");
    }
    
    std::cout << "2^" << exp << " = " << calc_result.get_value() << std::endl;
}
```

### 🧮 **Funciones Puras (noexcept)**
```cpp
// Funciones que nunca fallan:
constexpr bool is_power_of_2(int32_t value) noexcept {
    return value > 0 && (value & (value - 1)) == 0;
}

constexpr int get_max_exponent_int8() noexcept {
    return 6;
}

constexpr bool is_valid_exponent_int8(int exp) noexcept {
    return exp >= 0 && exp <= 6;
}
```

## 🚀 BENEFICIOS

### ⚡ **Rendimiento**
- **Zero-cost abstractions**: No overhead en release builds
- **constexpr compatible**: Evaluación en tiempo de compilación
- **noexcept functions**: Optimizaciones del compilador mejoradas

### 🛡️ **Seguridad**
- **Manejo explícito de errores**: No errores silenciosos
- **API clara**: Diferenciación entre cálculo e I/O
- **Verificación en compilación**: static_assert funciona

### 🔧 **Mantenibilidad**
- **Separación de responsabilidades**: Cálculo vs I/O claramente diferenciados
- **Evolución gradual**: Funciones legacy coexisten con nuevas
- **Testing simplificado**: Errores predecibles sin excepciones

## 📈 CASOS DE USO

### ✅ **Usar CalculationResult + noexcept**:
- Funciones matemáticas puras
- Algoritmos de cálculo
- Conversiones de tipo
- Verificaciones y validaciones
- Operaciones bit-wise
- Lookup tables y cachés

### ✅ **Usar Excepciones**:
- Lectura/escritura de archivos
- Parsing de entrada de usuario
- Comunicación de red
- Operaciones de base de datos
- Validación de formato de datos
- Interfaz con APIs externas

### ✅ **Usar noexcept siempre**:
- Getters simples
- Funciones constexpr
- Operaciones triviales
- Funciones que nuncan pueden fallar
- Destructores
- Move constructors/assignments

## 🎯 PLAN DE MIGRACIÓN

### 🔄 **Fase 1: Funciones de Cálculo**
- [x] Crear sistema CalculationResult
- [x] Refactorizar lookup tables
- [ ] Refactorizar basic_power_functions
- [ ] Refactorizar integral_power_functions

### 🔄 **Fase 2: Funciones I/O**
- [ ] Verificar que funciones I/O usen excepciones
- [ ] Documentar APIs de I/O existentes
- [ ] Asegurar consistencia en manejo de errores I/O

### 🔄 **Fase 3: noexcept**
- [ ] Marcar todas las funciones puras como noexcept
- [ ] Marcar todas las constexpr como noexcept
- [ ] Verificar getters y funciones triviales

### 🔄 **Fase 4: Testing y Documentación**
- [ ] Crear tests para nuevo sistema
- [ ] Documentar APIs refactorizadas
- [ ] Crear guías de migración

## 💡 EJEMPLO COMPLETO

```cpp
#include "number_calc/error_handling.hpp"

using namespace number_calc::error_handling;

// CÁLCULO: CalculationResult + noexcept
constexpr CalculationResult<int32_t> factorial_safe(int n) noexcept {
    if (n < 0) return make_error<int32_t>(CalculationError::InvalidInput);
    if (n > 12) return make_error<int32_t>(CalculationError::Overflow);
    
    int32_t result = 1;
    for (int i = 2; i <= n; ++i) {
        result *= i;
    }
    return make_success(result);
}

// I/O: Excepciones
void print_factorial_table(int max_n) {
    if (max_n < 0) {
        throw std::invalid_argument("max_n debe ser no negativo");
    }
    
    for (int i = 0; i <= max_n; ++i) {
        auto result = factorial_safe(i);
        if (result.is_valid()) {
            std::cout << i << "! = " << result.get_value() << std::endl;
        } else {
            throw std::runtime_error("Error calculando " + std::to_string(i) + "!");
        }
    }
}

// PURA: noexcept
constexpr bool is_factorial_result(int32_t value) noexcept {
    if (value <= 0) return false;
    int32_t factorial = 1;
    int n = 1;
    while (factorial < value) {
        factorial *= ++n;
    }
    return factorial == value;
}
```

**¡Sistema de manejo de errores diferenciado implementado exitosamente!**