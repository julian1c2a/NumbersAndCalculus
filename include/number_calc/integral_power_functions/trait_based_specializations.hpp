#pragma once

/**
 * @file trait_based_specializations.hpp
 * @brief Especializaciones de funciones de potencia basadas en traits
 * signed/unsigned
 * @author AlgoritmiaCombinatoria Project
 * @date 2024
 * @version 1.0
 *
 * Este archivo implementa especializaciones optimizadas de funciones de
 * potencia que se seleccionan automáticamente basándose en si el tipo es signed
 * o unsigned.
 */

#include "../../type_traits/extended_integral_traits.hpp"
#include "basic_power_functions.hpp"


namespace number_calc {
namespace integral_power_functions {

using namespace number_calc::type_traits;

//==============================================================================
// ESPECIALIZACIONES POR TRAITS: SIGNED VS UNSIGNED
//==============================================================================

/**
 * @brief Implementación especializada para tipos SIGNED
 * @tparam T Tipo entero con signo
 * @tparam U Tipo entero del exponente
 * @param base Base de la potencia
 * @param exp Exponente
 * @return base^exp
 *
 * Esta implementación maneja casos especiales de números negativos:
 * - Optimización especial para base = -1 (alternancia de signo)
 * - Manejo correcto del signo según la paridad del exponente
 */
template <typename T, typename U>
constexpr T int_power_signed_impl(T base, U exp) noexcept {
  static_assert(is_signed_extended_v<T>, "T debe ser un tipo signed");
  static_assert(is_integral_extended_v<U>, "U debe ser un tipo integral");

  if (exp == 0)
    return T{1};
  if (base == 0)
    return T{0};
  if (base == 1)
    return T{1};

  // Optimización especial para -1: resultado depende de paridad
  if (base == -1) {
    return (exp & 1) ? T{-1} : T{1}; // Impar: -1, Par: 1
  }

  // Caso general para números negativos
  if (base < 0) {
    T abs_base = -base;
    T result = T{1};
    T current_base = abs_base;
    U current_exp = exp;

    while (current_exp > 0) {
      if (current_exp & 1) {
        result *= current_base;
      }
      current_base *= current_base;
      current_exp >>= 1;
    }

    // Aplicar signo según paridad del exponente original
    return (exp & 1) ? -result : result;
  }

  // Para números positivos, usar implementación estándar
  return int_power_basic(base, exp);
}

/**
 * @brief Implementación especializada para tipos UNSIGNED
 * @tparam T Tipo entero sin signo
 * @tparam U Tipo entero del exponente
 * @param base Base de la potencia
 * @param exp Exponente
 * @return base^exp
 *
 * Esta implementación está optimizada para tipos unsigned:
 * - No necesita verificar signos
 * - Puede usar optimizaciones más agresivas
 * - Evita comprobaciones innecesarias de signo
 */
template <typename T, typename U>
constexpr T int_power_unsigned_impl(T base, U exp) noexcept {
  static_assert(is_unsigned_extended_v<T>, "T debe ser un tipo unsigned");
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
    if (current_exp & 1) {
      result *= current_base;
    }
    current_base *= current_base;
    current_exp >>= 1;
  }

  return result;
}

//==============================================================================
// SISTEMA DE DISPATCH AUTOMÁTICO
//==============================================================================

/**
 * @brief Función dispatcher que selecciona automáticamente la implementación
 * @tparam T Tipo entero (signed o unsigned)
 * @tparam U Tipo entero del exponente
 * @param base Base de la potencia
 * @param exp Exponente
 * @return base^exp
 *
 * Usa is_signed_extended_v para decidir en tiempo de compilación
 * qué implementación especializada usar.
 */
template <typename T, typename U>
constexpr T int_power_dispatch(T base, U exp) noexcept {
  static_assert(is_integral_extended_v<T>, "T debe ser un tipo integral");
  static_assert(is_integral_extended_v<U>, "U debe ser un tipo integral");

  if constexpr (is_signed_extended_v<T>) {
    return int_power_signed_impl(base, exp);
  } else {
    return int_power_unsigned_impl(base, exp);
  }
}

//==============================================================================
// FUNCIONES AUXILIARES ESPECIALIZADAS
//==============================================================================

/**
 * @brief Versión especializada para tipos signed (helper)
 * @tparam T Tipo signed
 * @tparam U Tipo del exponente
 * @param exp Exponente para 2^exp
 * @return 2^exp usando especialización signed
 */
template <typename T, typename U>
constexpr T int_power_2_signed(U exp) noexcept {
  static_assert(is_signed_extended_v<T>, "T debe ser un tipo signed");
  return int_power_signed_impl(T{2}, exp);
}

/**
 * @brief Versión especializada para tipos unsigned (helper)
 * @tparam T Tipo unsigned
 * @tparam U Tipo del exponente
 * @param exp Exponente para 2^exp
 * @return 2^exp usando especialización unsigned
 */
template <typename T, typename U>
constexpr T int_power_2_unsigned(U exp) noexcept {
  static_assert(is_unsigned_extended_v<T>, "T debe ser un tipo unsigned");
  return int_power_unsigned_impl(T{2}, exp);
}

} // namespace integral_power_functions
} // namespace number_calc