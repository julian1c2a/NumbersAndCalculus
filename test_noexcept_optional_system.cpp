#include "include/number_calc/error_handling.hpp"
#include <cstdint>
#include <iostream>


/**
 * @file test_noexcept_optional_system.cpp
 * @brief Prueba del nuevo sistema de manejo de errores diferenciado
 */

using namespace number_calc::error_handling;

// Versiones simplificadas de las tablas para demostrar el concepto
constexpr int8_t power_table_int8[] = {1, 2, 4, 8, 16, 32, 64};
constexpr uint8_t power_table_uint8[] = {1, 2, 4, 8, 16, 32, 64, 128};

/**
 * @brief Función de cálculo PURA - usa CalculationResult y es noexcept
 */
constexpr CalculationResult<int8_t> get_power_of_2_int8_safe(int exp) noexcept {
  if (exp < 0 || exp >= 7) {
    return make_error<int8_t>(CalculationError::OutOfRange);
  }
  return make_success(power_table_int8[exp]);
}

/**
 * @brief Función de cálculo PURA - usa CalculationResult y es noexcept
 */
constexpr CalculationResult<uint8_t>
get_power_of_2_uint8_safe(int exp) noexcept {
  if (exp < 0 || exp >= 8) {
    return make_error<uint8_t>(CalculationError::OutOfRange);
  }
  return make_success(power_table_uint8[exp]);
}

/**
 * @brief Función de IO - mantiene excepciones para manejo de errores
 * @note Esta función NO es noexcept porque es de I/O
 */
void print_power_result_io(int exp) {
  // Simular función de IO que puede fallar
  if (exp < 0) {
    throw std::invalid_argument(
        "print_power_result_io: exponente negativo no válido para salida");
  }

  auto result = get_power_of_2_int8_safe(exp);
  if (result.is_valid()) {
    std::cout << "2^" << exp << " = " << static_cast<int>(result.get_value())
              << std::endl;
  } else {
    throw std::runtime_error(
        "print_power_result_io: no se puede imprimir resultado inválido");
  }
}

/**
 * @brief Función auxiliar PURA - noexcept
 */
constexpr bool is_power_of_2(int32_t value) noexcept {
  return value > 0 && (value & (value - 1)) == 0;
}

/**
 * @brief Getter PURO - noexcept
 */
constexpr int get_max_supported_exponent_int8() noexcept { return 6; }

int main() {
  std::cout << "=== DEMO: SISTEMA DE MANEJO DE ERRORES DIFERENCIADO ===\n\n";

  // 1. FUNCIONES DE CÁLCULO - usan CalculationResult, son noexcept
  std::cout << "1. FUNCIONES DE CÁLCULO (noexcept + CalculationResult):\n";

  // Casos exitosos
  for (int i = 0; i <= 6; ++i) {
    auto result = get_power_of_2_int8_safe(i);
    if (result.is_valid()) {
      std::cout << "   ✅ 2^" << i << " = "
                << static_cast<int>(result.get_value()) << "\n";
    }
  }

  // Casos de error - SIN EXCEPCIONES
  auto error_result = get_power_of_2_int8_safe(10); // Fuera de rango
  if (error_result.has_error()) {
    std::cout << "   ❌ 2^10: " << error_to_string(error_result.get_error())
              << "\n";
  }

  auto error_result2 = get_power_of_2_int8_safe(-1); // Negativo
  if (error_result2.has_error()) {
    std::cout << "   ❌ 2^(-1): " << error_to_string(error_result2.get_error())
              << "\n";
  }

  // 2. FUNCIONES PURAS - noexcept
  std::cout << "\n2. FUNCIONES PURAS (noexcept):\n";
  std::cout << "   is_power_of_2(16): "
            << (is_power_of_2(16) ? "✅ Sí" : "❌ No") << "\n";
  std::cout << "   is_power_of_2(15): "
            << (is_power_of_2(15) ? "✅ Sí" : "❌ No") << "\n";
  std::cout << "   get_max_supported_exponent_int8(): "
            << get_max_supported_exponent_int8() << "\n";

  // 3. FUNCIONES DE IO - usan excepciones
  std::cout << "\n3. FUNCIONES DE I/O (con excepciones):\n";

  try {
    print_power_result_io(3);  // Éxito
    print_power_result_io(5);  // Éxito
    print_power_result_io(10); // Error de cálculo -> excepción de IO
  } catch (const std::exception &e) {
    std::cout << "   ❌ Excepción de I/O capturada: " << e.what() << "\n";
  }

  try {
    print_power_result_io(-1); // Error de entrada -> excepción de IO
  } catch (const std::exception &e) {
    std::cout << "   ❌ Excepción de I/O capturada: " << e.what() << "\n";
  }

  // 4. CONSTEXPR - evaluación en tiempo de compilación
  std::cout << "\n4. EVALUACIÓN CONSTEXPR (tiempo de compilación):\n";

  constexpr auto compile_time_result = get_power_of_2_int8_safe(4);
  static_assert(compile_time_result.is_valid(),
                "Resultado constexpr debe ser válido");
  static_assert(compile_time_result.get_value() == 16, "2^4 debe ser 16");

  constexpr auto compile_time_error = get_power_of_2_int8_safe(10);
  static_assert(compile_time_error.has_error(), "Exponente 10 debe dar error");
  static_assert(compile_time_error.get_error() == CalculationError::OutOfRange,
                "Debe ser OutOfRange");

  std::cout << "   ✅ constexpr 2^4 = "
            << static_cast<int>(compile_time_result.get_value()) << "\n";
  std::cout << "   ✅ constexpr verificaciones de error pasaron\n";

  std::cout << "\n=== RESUMEN DE LA ESTRATEGIA ===\n";
  std::cout << "✅ CÁLCULOS: CalculationResult<T> + noexcept\n";
  std::cout << "✅ I/O: Excepciones (throw/catch)\n";
  std::cout << "✅ FUNCIONES PURAS: noexcept\n";
  std::cout << "✅ CONSTEXPR: Funciona perfectamente\n";
  std::cout << "✅ SIN OVERHEAD: Zero-cost abstractions\n";

  return 0;
}