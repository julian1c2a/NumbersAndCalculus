#pragma once

/**
 * @file basic_power_functions.hpp
 * @brief Funciones básicas de potencia entera con exponenciación binaria
 * @author AlgoritmiaCombinatoria Project
 * @date 2024
 * @version 1.0
 *
 * Este archivo contiene las implementaciones básicas de funciones de potencia
 * entera usando el algoritmo de exponenciación binaria, sin especializaciones
 * por traits.
 */

#include "../../type_traits/extended_integral_traits.hpp"
#include <limits>
#include <stdexcept>
#include <utility>

namespace number_calc {
namespace integral_power_functions {

using namespace number_calc::type_traits;

//==============================================================================
// FUNCIÓN TEMPLATE GENÉRICA DE POTENCIA ENTERA
//==============================================================================

/**
 * @brief Template genérico para potencia entera usando exponenciación binaria
 * @tparam T Tipo entero del resultado y base
 * @tparam U Tipo entero del exponente
 * @param base Base de la potencia
 * @param exp Exponente (debe ser no negativo)
 * @return base^exp
 *
 * Implementa exponenciación binaria O(log n) para cualquier tipo entero.
 * Esta es la implementación base que pueden usar las especializaciones.
 */
template <typename T, typename U>
constexpr T int_power_basic(T base, U exp) noexcept {
  static_assert(is_integral_extended_v<T>, "T debe ser un tipo integral");
  static_assert(is_integral_extended_v<U>, "U debe ser un tipo integral");

  if (exp == 0)
    return T{1};
  if (exp == 1)
    return base;
  if (base == 0)
    return T{0};
  if (base == 1)
    return T{1};

  T result = T{1};
  T current_base = base;
  U current_exp = exp;

  while (current_exp > 0) {
    if (current_exp & 1) { // Si exp es impar
      result *= current_base;
    }
    current_base *= current_base;
    current_exp >>= 1; // Dividir por 2
  }

  return result;
}

//==============================================================================
// FUNCIONES DE VALIDACIÓN Y SEGURIDAD
//==============================================================================

/**
 * @brief Verifica si una operación de potencia es segura (no causa overflow)
 * @tparam T Tipo entero
 * @param base Base de la potencia
 * @param exp Exponente
 * @return true si la operación es segura
 *
 * Implementa verificaciones básicas de overflow para prevenir
 * resultados incorrectos en cálculos de potencia.
 */
template <typename T> constexpr bool int_power_safe(T base, T exp) noexcept {
  static_assert(is_integral_extended_v<T>, "T debe ser un tipo integral");

  if (exp == 0)
    return true; // Siempre 1
  if (base == 0 || base == 1)
    return true; // Siempre seguro
  if (base == -1)
    return true; // Siempre ±1

  // Para tipos con límites conocidos
  if (std::numeric_limits<T>::is_specialized) {
    T max_val = std::numeric_limits<T>::max();
    T min_val = std::numeric_limits<T>::min();

    // Estimación simple: si base^2 > max_val/2, probablemente overflow
    if (base > 0 && base > max_val / base)
      return false;
    if (base < 0 && base < min_val / (-base))
      return false;
  }

  return true; // Asumir seguro si no podemos determinar
}

/**
 * @brief Versión de int_power que lanza excepción en caso de overflow
 * @tparam T Tipo entero
 * @param base Base de la potencia
 * @param exp Exponente
 * @return base^exp
 * @throws std::overflow_error si la operación causaría overflow
 *
 * Versión segura que verifica overflow antes de realizar el cálculo.
 */
template <typename T> constexpr T int_power_checked(T base, T exp) {
  static_assert(is_integral_extended_v<T>, "T debe ser un tipo integral");

  if (!int_power_safe(base, exp)) {
    throw std::overflow_error("int_power: resultado causaría overflow");
  }
  return int_power_basic(base, exp);
}

/**
 * @brief Versión de int_power que devuelve el resultado y un indicador de éxito
 * @tparam T Tipo entero
 * @param base Base de la potencia
 * @param exp Exponente
 * @return std::pair<T, bool> donde el primer elemento es el resultado y el
 * segundo indica si fue exitoso
 *
 * Versión segura que no lanza excepciones, útil para programación funcional.
 */
template <typename T>
constexpr std::pair<T, bool> int_power_safe_pair(T base, T exp) noexcept {
  static_assert(is_integral_extended_v<T>, "T debe ser un tipo integral");

  if (!int_power_safe(base, exp)) {
    return std::make_pair(T{0}, false);
  }
  return std::make_pair(int_power_basic(base, exp), true);
}

//==============================================================================
// FUNCIÓN PRINCIPAL PÚBLICA
//==============================================================================

/**
 * @brief Función principal de potencia entera
 * @tparam T Tipo entero del resultado y base
 * @tparam U Tipo entero del exponente
 * @param base Base de la potencia
 * @param exp Exponente
 * @return base^exp
 *
 * Esta es la función principal que usarán los usuarios. Por ahora
 * delega a int_power_basic, pero puede ser expandida para usar
 * especializaciones automáticas.
 */
template <typename T, typename U>
constexpr T int_power(T base, U exp) noexcept {
  return int_power_basic(base, exp);
}

} // namespace integral_power_functions
} // namespace number_calc