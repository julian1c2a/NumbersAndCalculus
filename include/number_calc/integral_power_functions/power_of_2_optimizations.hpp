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
#include <limits>

namespace number_calc {
namespace integral_power_functions {

using namespace number_calc::type_traits;

//==============================================================================
// ESPECIALIZACIÓN PARA POTENCIAS DE 2 (base = 2)
//==============================================================================

/**
 * @brief Optimización para tipos grandes usando resultado de 32 bits como base
 * @tparam T Tipo entero grande (> 32 bits)
 * @tparam U Tipo del exponente
 * @param exp Exponente (2^exp)
 * @return 2^exp optimizado para tipos grandes
 *
 * Estrategia: Si exp > 32, calcular 2^32 y usar como base para reducir
 * iteraciones. Por ejemplo: 2^100 = (2^32)^3 * 2^4, reduciendo de 100 a 3
 * iteraciones principales.
 */
template <typename T, typename U>
constexpr T int_power_2_large_optimized(U exp) noexcept {
  static_assert(sizeof(T) > sizeof(uint32_t),
                "Esta optimización es solo para tipos > 32 bits");
  static_assert(is_integral_extended_v<T>, "T debe ser un tipo integral");
  static_assert(is_integral_extended_v<U>, "U debe ser un tipo integral");

  // Casos triviales
  if (exp == 0)
    return T{1};
  if (exp == 1)
    return T{2};

  // Para exponentes pequeños, usar desplazamiento directo
  if (exp < 64) {
    return T{1} << exp;
  }

  // OPTIMIZACIÓN CLAVE: Para exponentes grandes, partir del máximo de 32 bits
  constexpr U max_32bit_exp = is_signed_extended_v<T> ? 30 : 31;
  constexpr T power_32_base = T{1} << max_32bit_exp; // 2^30 o 2^31

  if (exp <= max_32bit_exp) {
    return T{1} << exp;
  }

  // Calcular cuántas veces cabe max_32bit_exp en exp
  U quotient = exp / max_32bit_exp;
  U remainder = exp % max_32bit_exp;

  // Resultado = (2^max_32bit_exp)^quotient * 2^remainder
  T result = T{1};
  T base_32 = power_32_base;

  // Exponenciación binaria para (2^max_32bit_exp)^quotient
  while (quotient > 0) {
    if (quotient & 1) {
      result *= base_32;
    }
    base_32 *= base_32;
    quotient >>= 1;
  }

  // Multiplicar por 2^remainder
  if (remainder > 0) {
    result *= (T{1} << remainder);
  }

  return result;
}

/**
 * @brief Función altamente optimizada para potencias de 2
 * @tparam T Tipo entero de retorno
 * @tparam U Tipo del exponente
 * @param exp Exponente (2^exp)
 * @return 2^exp
 *
 * OPTIMIZACIONES POR TIPO:
 * - int8_t/uint8_t/int16_t/uint16_t: Tablas constexpr precalculadas
 * - Tipos medianos: Desplazamiento de bits
 * - Tipos grandes (>32 bits): Optimización usando base de 32 bits como punto de
 * partida
 * - Fallback: Algoritmo general para casos extremos
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

  // OPTIMIZACIÓN NIVEL 3: Para tipos muy grandes, usar resultado de 32 bits
  // como base
  if constexpr (sizeof(T) > sizeof(uint32_t)) {
    return int_power_2_large_optimized<T>(exp);
  }

  // OPTIMIZACIÓN NIVEL 4: Fallback para casos restantes
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
// DECLARACIÓN ADELANTADA PARA OPTIMIZACIÓN DE TIPOS GRANDES
//==============================================================================

/**
 * @brief Declaración adelantada para optimización de tipos grandes
 */
template <typename T, typename U>
constexpr T int_power_large_optimized(T base, U exp) noexcept;

//==============================================================================
// FUNCIÓN INTELIGENTE CON DETECCIÓN AUTOMÁTICA DE OPTIMIZACIONES
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

  // OPTIMIZACIÓN NIVEL 4: Para tipos grandes (>64 bits), usar resultado de 32
  // bits como base
  if constexpr (sizeof(T) > sizeof(uint64_t)) {
    return int_power_large_optimized(base, exp);
  }

  // OPTIMIZACIÓN NIVEL 5: Dispatch normal por traits
  return int_power_dispatch(base, exp);
}

//==============================================================================
// OPTIMIZACIÓN GENERAL PARA TIPOS GRANDES
//==============================================================================

/**
 * @brief Optimización general para tipos grandes usando potencias de 32 bits
 * como base
 * @tparam T Tipo entero grande (> 64 bits)
 * @tparam U Tipo del exponente
 * @param base Base de la potencia
 * @param exp Exponente
 * @return base^exp optimizado para tipos grandes
 *
 * Estrategia: Si exp es grande, calcular potencias intermedias usando tipos más
 * pequeños y usar esos resultados como punto de partida para reducir
 * iteraciones.
 *
 * Ejemplo: Para calcular 3^100 en __int128:
 * 1. Calcular 3^32 usando uint32_t (más eficiente)
 * 2. Usar 3^32 como nueva base: (3^32)^3 * 3^4
 * 3. Reducir de 100 iteraciones a 3 iteraciones principales
 */
template <typename T, typename U>
constexpr T int_power_large_optimized(T base, U exp) noexcept {
  static_assert(sizeof(T) > sizeof(uint64_t),
                "Esta optimización es solo para tipos > 64 bits");
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

  // Para exponentes pequeños, usar algoritmo normal
  if (exp <= 32) {
    return int_power_dispatch(base, exp);
  }

  // OPTIMIZACIÓN CLAVE: Partir del resultado de 32 bits
  constexpr U chunk_size = 32;

  // Calcular base^chunk_size usando tipo más pequeño si es posible
  T base_chunk;
  if (base <= std::numeric_limits<uint32_t>::max()) {
    // Si la base cabe en uint32_t, usar esa optimización
    uint32_t base_32 = static_cast<uint32_t>(base);
    uint32_t result_32 = int_power_dispatch(base_32, chunk_size);
    base_chunk = static_cast<T>(result_32);
  } else {
    // Si la base es muy grande, calcular directamente
    base_chunk = int_power_dispatch(base, chunk_size);
  }

  // Descomponer el exponente: exp = quotient * chunk_size + remainder
  U quotient = exp / chunk_size;
  U remainder = exp % chunk_size;

  // Resultado = (base^chunk_size)^quotient * base^remainder
  T result = T{1};
  T current_base_chunk = base_chunk;

  // Exponenciación binaria para (base^chunk_size)^quotient
  while (quotient > 0) {
    if (quotient & 1) {
      result *= current_base_chunk;
    }
    current_base_chunk *= current_base_chunk;
    quotient >>= 1;
  }

  // Multiplicar por base^remainder
  if (remainder > 0) {
    T remainder_result = int_power_dispatch(base, remainder);
    result *= remainder_result;
  }

  return result;
}

} // namespace integral_power_functions
} // namespace number_calc