#pragma once

/**
 * @file error_handling.hpp
 * @brief Utilidades para manejo de errores diferenciado según propósito con
 * std::optional (C++17)
 * @author AlgoritmiaCombinatoria Project
 * @date 2024
 * @version 2.0
 *
 * Este archivo define estrategias de manejo de errores usando C++17:
 * - Para funciones de cálculo: usar std::optional<T> (requiere C++17)
 * - Para funciones de I/O: usar excepciones (throw/catch)
 * - Para funciones puras: marcar como noexcept cuando sea posible
 *
 * NOTA: Este código requiere compilación con -std=c++17 o superior
 */

#include <array>
#include <cstdint>
#include <optional>

namespace number_calc {
namespace error_handling {

// Lookup table para potencias de 2 (int8_t)
constexpr std::array<int8_t, 7> power_of_2_int8 = {1, 2, 4, 8, 16, 32, 64};

/**
 * @brief Enumeración para códigos de error en cálculos (opcional, para casos
 * donde necesitemos detalles específicos)
 */
enum class CalculationError {
  None = 0,      ///< Sin error
  OutOfRange,    ///< Parámetro fuera del rango válido
  DomainError,   ///< Valor fuera del dominio de la función matemática
  Overflow,      ///< Overflow aritmético
  Underflow,     ///< Underflow aritmético
  InvalidInput,  ///< Entrada inválida (formato incorrecto, null, etc.)
  DivisionByZero ///< División por cero
};

/**
 * @brief Convierte código de error a string (para debugging)
 */
constexpr const char *error_to_string(CalculationError error) noexcept {
  switch (error) {
  case CalculationError::None:
    return "No error";
  case CalculationError::OutOfRange:
    return "Out of range";
  case CalculationError::DomainError:
    return "Domain error";
  case CalculationError::Overflow:
    return "Overflow";
  case CalculationError::Underflow:
    return "Underflow";
  case CalculationError::InvalidInput:
    return "Invalid input";
  case CalculationError::DivisionByZero:
    return "Division by zero";
  default:
    return "Unknown error";
  }
}

// ============================================================================
// PATRÓN RECOMENDADO: Usar std::optional<T> para funciones de cálculo
// ============================================================================

/**
 * @brief Ejemplo de función de cálculo usando std::optional (recomendado)
 *
 * Para la mayoría de funciones de cálculo, usar std::optional<T> es suficiente:
 * - Devuelve std::nullopt en caso de error
 * - Devuelve el valor encapsulado en caso de éxito
 * - Es noexcept porque no lanza excepciones
 *
 * Ejemplo de uso:
 * @code
 * std::optional<int> safe_divide(int a, int b) noexcept {
 *     if (b == 0) return std::nullopt;
 *     return a / b;
 * }
 * @endcode
 */

/**
 * @brief Para casos donde necesitemos información específica sobre el error,
 * podemos combinar std::optional con el enum CalculationError:
 *
 * Ejemplo de uso avanzado:
 * @code
 * struct DetailedResult {
 *     std::optional<int> value;
 *     CalculationError error_code;
 * };
 *
 * DetailedResult detailed_divide(int a, int b) noexcept {
 *     if (b == 0) return {std::nullopt, CalculationError::DivisionByZero};
 *     if (a > INT_MAX / b) return {std::nullopt, CalculationError::Overflow};
 *     return {{a / b}, CalculationError::None};
 * }
 *
 * // Ejemplo de uso de DomainError para funciones matemáticas:
 * DetailedResult safe_sqrt(double x) noexcept {
 *     if (x < 0.0) return {std::nullopt, CalculationError::DomainError};
 *     return {{std::sqrt(x)}, CalculationError::None};
 * }
 *
 * DetailedResult safe_log(double x) noexcept {
 *     if (x <= 0.0) return {std::nullopt, CalculationError::DomainError};
 *     return {{std::log(x)}, CalculationError::None};
 * }
 * @endcode
 */

// Antes:
// constexpr CalculationResult<int8_t> get_power_of_2_int8_safe(int exp)
// noexcept {
//     if (exp < 0 || exp >= 7) {
//         return make_error<int8_t>(CalculationError::OutOfRange);
//     }
//     return make_success(power_of_2_int8[exp]);
// }

// Después:
constexpr std::optional<int8_t> get_power_of_2_int8_safe(int exp) noexcept {
  if (exp < 0 || exp >= 7) {
    return std::nullopt;
  }
  return power_of_2_int8[exp];
}

} // namespace error_handling
} // namespace number_calc