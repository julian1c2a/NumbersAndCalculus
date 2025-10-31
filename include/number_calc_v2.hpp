#pragma once

/**
 * @file number_calc.hpp
 * @brief Cálculos y funciones auxiliares completamente especializadas
 * para todos los tipos enteros con especializaciones por traits
 * @author AlgoritmiaCombinatoria Project
 * @date 2024
 * @version 2.0
 *
 * Este archivo implementa especializaciones basadas en traits para funciones
 * de potencia entera, usando std::is_signed_v y std::is_unsigned_v para
 * despachar automáticamente a las implementaciones optimizadas.
 */

#include <cstdint>
#include <limits>
#include <stdexcept>
#include <type_traits>

namespace number_calc_impl {

//==============================================================================
// FUNCIÓN TEMPLATE GENÉRICA DE POTENCIA ENTERA
//==============================================================================

/**
 * @brief Template genérico para potencia entera usando exponenciación binaria
 * @tparam T Tipo entero
 * @param base Base de la potencia
 * @param exp Exponente (debe ser no negativo)
 * @return base^exp
 *
 * Implementa exponenciación binaria O(log n) para cualquier tipo entero.
 * Las especializaciones pueden proporcionar optimizaciones específicas.
 */
template <typename T, typename U>
constexpr T int_power(T base, U exp) noexcept {
  static_assert(std::is_integral_v<T>, "T debe ser un tipo integral");
  static_assert(std::is_integral_v<U>, "U debe ser un tipo integral");

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
// ESPECIALIZACIONES POR TRAITS: SIGNED VS UNSIGNED
//==============================================================================

/**
 * @brief Implementación especializada para tipos SIGNED
 * @tparam T Tipo entero con signo
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
  static_assert(std::is_signed_v<T>, "T debe ser un tipo signed");
  static_assert(std::is_integral_v<U>, "U debe ser un tipo integral");

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
  return int_power(base, exp);
}

/**
 * @brief Implementación especializada para tipos UNSIGNED
 * @tparam T Tipo entero sin signo
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
  static_assert(std::is_unsigned_v<T>, "T debe ser un tipo unsigned");
  static_assert(std::is_integral_v<U>, "U debe ser un tipo integral");

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

/**
 * @brief Función dispatcher que selecciona automáticamente la implementación
 * @tparam T Tipo entero (signed o unsigned)
 * @param base Base de la potencia
 * @param exp Exponente
 * @return base^exp
 *
 * Usa std::is_signed_v para decidir en tiempo de compilación
 * qué implementación especializada usar.
 */
template <typename T, typename U>
constexpr T int_power_dispatch(T base, U exp) noexcept {
  static_assert(std::is_integral_v<T>, "T debe ser un tipo integral");
  static_assert(std::is_integral_v<U>, "U debe ser un tipo integral");

  if constexpr (std::is_signed_v<T>) {
    return int_power_signed_impl(base, exp);
  } else {
    return int_power_unsigned_impl(base, exp);
  }
}

//==============================================================================
// ESPECIALIZACIÓN PARA POTENCIAS DE 2 (base = 2)
//==============================================================================

/**
 * @brief Función altamente optimizada para potencias de 2
 * @tparam T Tipo entero de retorno
 * @tparam U Tipo del exponente
 * @param exp Exponente (2^exp)
 * @return 2^exp
 *
 * Usa desplazamiento de bits para máxima eficiencia.
 * Incluye verificaciones de overflow para tipos pequeños.
 */
template <typename T, typename U> constexpr T int_power_2(U exp) noexcept {
  static_assert(std::is_integral_v<T>, "T debe ser un tipo integral");
  static_assert(std::is_integral_v<U>, "U debe ser un tipo integral");

  if (exp == 0)
    return T{1};

  // Usar desplazamiento de bits para tipos pequeños/medianos
  if constexpr (std::is_signed_v<T>) {
    // Para tipos signed, verificar límites para evitar overflow
    if constexpr (sizeof(T) == sizeof(int)) {
      if (exp < 31)
        return T{1} << exp;
    } else if constexpr (sizeof(T) == sizeof(long long)) {
      if (exp < 63)
        return T{1} << exp;
    }
  } else {
    // Para tipos unsigned, puede usar un bit más
    if constexpr (sizeof(T) == sizeof(unsigned int)) {
      if (exp < 32)
        return T{1} << exp;
    } else if constexpr (sizeof(T) == sizeof(unsigned long long)) {
      if (exp < 64)
        return T{1} << exp;
    }
  }

  // Fallback para tipos grandes o exponentes grandes
  return int_power_dispatch(T{2}, exp);
}

/**
 * @brief Versión especializada de int_power_2 para tipos signed
 */
template <typename T, typename U>
constexpr T int_power_2_signed(U exp) noexcept {
  static_assert(std::is_signed_v<T>, "T debe ser un tipo signed");
  return int_power_2<T>(exp);
}

/**
 * @brief Versión especializada de int_power_2 para tipos unsigned
 */
template <typename T, typename U>
constexpr T int_power_2_unsigned(U exp) noexcept {
  static_assert(std::is_unsigned_v<T>, "T debe ser un tipo unsigned");
  return int_power_2<T>(exp);
}

/**
 * @brief Nueva función int_power que usa el sistema de dispatch
 * @tparam T Tipo entero
 * @param base Base de la potencia
 * @param exp Exponente
 * @return base^exp
 *
 * Esta función detecta automáticamente si base = 2 y usa la optimización
 * correspondiente, o delega al sistema de dispatch por traits.
 */
template <typename T, typename U>
constexpr T int_power_new(T base, U exp) noexcept {
  static_assert(std::is_integral_v<T>, "T debe ser un tipo integral");
  static_assert(std::is_integral_v<U>, "U debe ser un tipo integral");

  // Detección automática de potencias de 2
  if (base == T{2}) {
    return int_power_2<T>(exp);
  }

  // Usar dispatch por traits para otros casos
  return int_power_dispatch(base, exp);
}

//==============================================================================
// FUNCIONES DE SEGURIDAD Y VALIDACIÓN
//==============================================================================

/**
 * @brief Verifica si una operación de potencia es segura (no causa overflow)
 * @tparam T Tipo entero
 * @param base Base de la potencia
 * @param exp Exponente
 * @return true si la operación es segura
 */
template <typename T> constexpr bool int_power_safe(T base, T exp) noexcept {
  static_assert(std::is_integral_v<T>, "T debe ser un tipo integral");

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
 */
template <typename T> constexpr T int_power_checked(T base, T exp) {
  static_assert(std::is_integral_v<T>, "T debe ser un tipo integral");

  if (!int_power_safe(base, exp)) {
    throw std::overflow_error("int_power: resultado causaría overflow");
  }
  return int_power_new(base, exp);
}

} // namespace number_calc_impl

//==============================================================================
// ALIAS PÚBLICOS EN EL NAMESPACE GLOBAL
//==============================================================================

// Exportar las funciones principales al namespace global
using number_calc_impl::int_power;
using number_calc_impl::int_power_2;
using number_calc_impl::int_power_checked;
using number_calc_impl::int_power_dispatch;
using number_calc_impl::int_power_new;
using number_calc_impl::int_power_safe;
using number_calc_impl::int_power_signed_impl;
using number_calc_impl::int_power_unsigned_impl;