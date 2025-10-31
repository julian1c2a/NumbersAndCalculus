#include <iostream>
#include <optional>

// NOTA: Este código requiere compilación con -std=c++17 o superior

namespace number_calc {
namespace error_handling {

/**
 * @brief Enumeración para códigos de error (solo cuando necesitemos detalles
 * específicos)
 */
enum class CalculationError {
  None = 0,
  OutOfRange,
  Overflow,
  Underflow,
  InvalidInput,
  DivisionByZero
};

} // namespace error_handling
} // namespace number_calc

//==============================================================================
// PATRÓN RECOMENDADO: Funciones de cálculo con std::optional (C++17)
//==============================================================================

/**
 * @brief Ejemplo de función de cálculo segura usando std::optional
 * @param a Dividendo
 * @param b Divisor
 * @return std::optional<int> con el resultado o std::nullopt si hay error
 * @note Esta función es noexcept porque no lanza excepciones
 */
std::optional<int> safe_divide(int a, int b) noexcept {
  if (b == 0) {
    return std::nullopt; // Error: división por cero
  }
  return a / b; // Éxito: devuelve el resultado
}

/**
 * @brief Función de cálculo con rango limitado
 * @param exp Exponente (0 <= exp <= 10)
 * @return std::optional<int> con 2^exp o std::nullopt si está fuera de rango
 */
std::optional<int> safe_power_of_2(int exp) noexcept {
  if (exp < 0 || exp > 10) {
    return std::nullopt; // Fuera de rango
  }
  return 1 << exp; // 2^exp
}

/**
 * @brief Función pura matemática (siempre exitosa)
 * @param x Número
 * @return x^2
 * @note Esta función es noexcept porque siempre tiene éxito
 */
constexpr int square(int x) noexcept { return x * x; }

//==============================================================================
// FUNCIONES DE I/O (mantienen excepciones)
//==============================================================================

/**
 * @brief Función de I/O que puede fallar (mantiene excepciones)
 * @param filename Nombre del archivo
 * @throws std::runtime_error si el archivo no se puede abrir
 */
void save_to_file(const std::string &filename, int value) {
  // Simulación de I/O que puede fallar
  if (filename.empty()) {
    throw std::runtime_error("Nombre de archivo vacío");
  }
  std::cout << "Guardando " << value << " en " << filename << std::endl;
}

//==============================================================================
// DEMOSTRACIÓN DEL SISTEMA
//==============================================================================

int main() {
  std::cout << "=== Sistema de Manejo de Errores con std::optional (C++17) ==="
            << std::endl;
  std::cout << std::endl;

  // 1. Funciones de cálculo con std::optional (noexcept)
  std::cout << "1. FUNCIONES DE CÁLCULO (std::optional + noexcept):"
            << std::endl;

  auto result1 = safe_divide(10, 2);
  if (result1.has_value()) {
    std::cout << "   10 / 2 = " << result1.value() << std::endl;
  } else {
    std::cout << "   Error en división" << std::endl;
  }

  auto result2 = safe_divide(10, 0);
  if (!result2) { // Forma abreviada de !result2.has_value()
    std::cout << "   División por cero manejada de forma segura" << std::endl;
  }

  auto power_result = safe_power_of_2(5);
  if (power_result) { // Forma abreviada de power_result.has_value()
    std::cout << "   2^5 = " << *power_result
              << std::endl; // *power_result es como .value()
  }

  auto invalid_power = safe_power_of_2(20);
  if (!invalid_power) {
    std::cout << "   Exponente fuera de rango manejado de forma segura"
              << std::endl;
  }

  // 2. Funciones puras (noexcept, siempre exitosas)
  std::cout << std::endl << "2. FUNCIONES PURAS (noexcept):" << std::endl;
  std::cout << "   square(7) = " << square(7) << std::endl;
  std::cout << "   square(-3) = " << square(-3) << std::endl;

  // 3. Funciones de I/O (mantienen excepciones)
  std::cout << std::endl << "3. FUNCIONES DE I/O (excepciones):" << std::endl;
  try {
    save_to_file("datos.txt", 42);
    std::cout << "   Archivo guardado exitosamente" << std::endl;
  } catch (const std::exception &e) {
    std::cout << "   Error de I/O: " << e.what() << std::endl;
  }

  try {
    save_to_file("", 42); // Esto debería fallar
  } catch (const std::exception &e) {
    std::cout << "   Error de I/O capturado: " << e.what() << std::endl;
  }

  std::cout << std::endl
            << "=== Ventajas de std::optional sobre CalculationResult "
               "personalizado ==="
            << std::endl;
  std::cout << "✓ Estándar C++17, bien conocido por desarrolladores"
            << std::endl;
  std::cout << "✓ Sintaxis más simple: std::nullopt vs make_error()"
            << std::endl;
  std::cout << "✓ Operadores intuitivos: if (result) vs if (result.is_valid())"
            << std::endl;
  std::cout << "✓ Mejor integración con herramientas y IDEs" << std::endl;
  std::cout << "✓ Menos código personalizado que mantener" << std::endl;

  return 0;
}