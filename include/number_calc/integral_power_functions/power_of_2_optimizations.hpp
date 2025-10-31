#pragma once

/**
 * @file power_of_2_optimizations.hpp
 * @brief Optimizaciones específicas para potencias de base 2
 * @author AlgoritmiaCombinatoria Project
 * @date 2024
 * @version 1.0
 *
 * Este archivo contiene optimizaciones específicas para cálculos de potencias
 * donde la base es 2, usando técnicas de bit-shifting para máxima eficiencia.
 */

#include "../../type_traits/extended_integral_traits.hpp"
#include "lookup_tables/power_of_2_lookup_tables.hpp"
#include "trait_based_specializations.hpp"
#include <cstdint>

namespace number_calc {
namespace integral_power_functions {

using namespace number_calc::type_traits;

//==============================================================================
// ESPECIALIZACIÓN PARA POTENCIAS DE 2 (base = 2)
//==============================================================================

/**
 * @brief Función altamente optimizada para potencias de 2 con tablas constexpr
 * @tparam T Tipo entero de retorno
 * @tparam U Tipo del exponente
 * @param exp Exponente (2^exp)
 * @return 2^exp
 *
 * OPTIMIZACIONES POR TIPO:
 * - int8_t/uint8_t/int16_t/uint16_t: Tablas constexpr precalculadas
 * - Tipos medianos: Desplazamiento de bits
 * - Tipos grandes: Fallback a algoritmo general
 */
template <typename T, typename U> constexpr T int_power_2(U exp) noexcept {
  static_assert(is_integral_extended_v<T>, "T debe ser un tipo integral");
  static_assert(is_integral_extended_v<U>, "U debe ser un tipo integral");

  if (exp == 0)
    return T{1};

  // OPTIMIZACIÓN NIVEL 1: Tablas constexpr para tipos pequeños (8 y 16 bits)
  if constexpr (std::is_same_v<T, int8_t> || std::is_same_v<T, uint8_t> ||
                std::is_same_v<T, int16_t> || std::is_same_v<T, uint16_t>) {

    // Verificar si el exponente está en rango válido para tabla lookup
    if (lookup_tables::is_valid_power_of_2_exponent<T>(static_cast<int>(exp))) {
      return lookup_tables::get_power_of_2_from_table<T>(static_cast<int>(exp));
    }
    // Si fuera de rango, fallback a algoritmo general (causará overflow, pero
    // es consistente)
    return int_power_dispatch(T{2}, exp);
  }

  // OPTIMIZACIÓN NIVEL 2: Desplazamiento de bits para tipos medianos
  if constexpr (is_signed_extended_v<T>) {
    // Para tipos signed, verificar límites para evitar overflow
    if constexpr (sizeof(T) == sizeof(int)) {
      if (exp < 31)
        return T{1} << exp;
    } else if constexpr (sizeof(T) == sizeof(long long)) {
      if (exp < 63)
        return T{1} << exp;
    }
#if defined(__GNUC__) || defined(__clang__)
    else if constexpr (sizeof(T) == sizeof(__int128)) {
      if (exp < 127)
        return T{1} << exp;
    }
#endif
  } else {
    // Para tipos unsigned, puede usar un bit más
    if constexpr (sizeof(T) == sizeof(unsigned int)) {
      if (exp < 32)
        return T{1} << exp;
    } else if constexpr (sizeof(T) == sizeof(unsigned long long)) {
      if (exp < 64)
        return T{1} << exp;
    }
#if defined(__GNUC__) || defined(__clang__)
    else if constexpr (sizeof(T) == sizeof(unsigned __int128)) {
      if (exp < 128)
        return T{1} << exp;
    }
#endif
  }

  // OPTIMIZACIÓN NIVEL 3: Fallback para tipos grandes o exponentes grandes
  return int_power_dispatch(T{2}, exp);
} //==============================================================================
// FUNCIONES DE DETECCIÓN AUTOMÁTICA
//==============================================================================

/**
 * @brief Nueva función int_power que usa el sistema de dispatch
 * @tparam T Tipo entero
 * @tparam U Tipo del exponente
 * @param base Base de la potencia
 * @param exp Exponente
 * @return base^exp
 *
 * Esta función detecta automáticamente si base = 2 y usa la optimización
 * correspondiente, o delega al sistema de dispatch por traits.
 */
template <typename T, typename U>
constexpr T int_power_new(T base, U exp) noexcept {
  static_assert(is_integral_extended_v<T>, "T debe ser un tipo integral");
  static_assert(is_integral_extended_v<U>, "U debe ser un tipo integral");

  // Detección automática de potencias de 2
  if (base == T{2}) {
    return int_power_2<T>(exp);
  }

  // Usar dispatch por traits para otros casos
  return int_power_dispatch(base, exp);
}

//==============================================================================
// FUNCIONES DE ANÁLISIS DE OPTIMIZACIÓN
//==============================================================================

/**
 * @brief Verifica si una base es una potencia de 2
 * @tparam T Tipo entero
 * @param base Base a verificar
 * @return true si base es potencia de 2
 *
 * Útil para detectar automáticamente cuando se pueden aplicar
 * optimizaciones de bit-shifting.
 */
template <typename T> constexpr bool is_power_of_2(T base) noexcept {
  static_assert(is_integral_extended_v<T>, "T debe ser un tipo integral");

  if (base <= 0)
    return false;
  return (base & (base - 1)) == 0;
}

/**
 * @brief Encuentra el exponente si base es potencia de 2
 * @tparam T Tipo entero
 * @param base Base a analizar
 * @return exponente tal que 2^exp = base, o -1 si no es potencia de 2
 */
template <typename T> constexpr int find_power_of_2_exponent(T base) noexcept {
  static_assert(is_integral_extended_v<T>, "T debe ser un tipo integral");

  if (!is_power_of_2(base))
    return -1;

  int exp = 0;
  T temp = base;
  while (temp > 1) {
    temp >>= 1;
    ++exp;
  }
  return exp;
}

//==============================================================================
// FUNCIÓN INTELIGENTE CON DETECCIÓN AUTOMÁTICA AVANZADA
//==============================================================================

/**
 * @brief Función inteligente que detecta automáticamente optimizaciones
 * @tparam T Tipo entero
 * @tparam U Tipo del exponente
 * @param base Base de la potencia
 * @param exp Exponente
 * @return base^exp usando la mejor optimización disponible
 *
 * Esta función aplica automáticamente la mejor optimización disponible:
 * - Si base es potencia de 2, usa optimizaciones de bit-shifting
 * - Si no, usa dispatch por traits signed/unsigned
 */
template <typename T, typename U>
constexpr T int_power_smart(T base, U exp) noexcept {
  static_assert(is_integral_extended_v<T>, "T debe ser un tipo integral");
  static_assert(is_integral_extended_v<U>, "U debe ser un tipo integral");

  // Casos triviales
  if (exp == 0)
    return T{1};
  if (exp == 1)
    return base;
  if (base == 0)
    return T{0};
  if (base == 1)
    return T{1};

  // OPTIMIZACIÓN NIVEL 1: Tipos pequeños con tablas constexpr
  if constexpr (std::is_same_v<T, int8_t> || std::is_same_v<T, uint8_t> ||
                std::is_same_v<T, int16_t> || std::is_same_v<T, uint16_t>) {

    // Para tipos pequeños, optimización especial para base = 2
    if (base == T{2}) {
      return int_power_2<T>(exp);
    }

    // Para otros casos de tipos pequeños, usar dispatch normal
    // (las tablas constexpr se aplicarán automáticamente en int_power_2)
  }

  // OPTIMIZACIÓN NIVEL 2: Base = 2 para cualquier tipo
  if (base == T{2}) {
    return int_power_2<T>(exp);
  }

  // OPTIMIZACIÓN NIVEL 3: Base es potencia de 2 (4, 8, 16, ...)
  if (is_power_of_2(base)) {
    int base_exp = find_power_of_2_exponent(base);
    if (base_exp > 0) {
      // base = 2^base_exp, entonces base^exp = 2^(base_exp * exp)
      return int_power_2<T>(base_exp * exp);
    }
  }

  // OPTIMIZACIÓN NIVEL 4: Dispatch normal por traits
  return int_power_dispatch(base, exp);
}

} // namespace integral_power_functions
} // namespace number_calc