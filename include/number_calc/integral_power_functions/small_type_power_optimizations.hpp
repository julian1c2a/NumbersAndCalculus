#pragma once

/**
 * @file small_type_power_optimizations.hpp
 * @brief Especializaciones optimizadas para tipos enteros pequeños
 * @author AlgoritmiaCombinatoria Project
 * @date 2024
 * @version 1.0
 *
 * Este archivo contiene especializaciones altamente optimizadas para cálculos
 * de potencias usando tipos enteros pequeños (8 y 16 bits). Las optimizaciones
 * incluyen:
 *
 * - Tablas constexpr para potencias de 2
 * - Verificaciones de overflow en tiempo de compilación
 * - Dispatch automático a la mejor estrategia por tipo
 */

#include "lookup_tables/power_of_2_lookup_tables.hpp"
#include "trait_based_specializations.hpp"
#include <cstdint>

namespace number_calc {
namespace integral_power_functions {
namespace small_types {

using namespace lookup_tables;

//==============================================================================
// FUNCIONES ESPECIALIZADAS PARA int8_t
//==============================================================================

/**
 * @brief Potencia optimizada para int8_t con base 2
 * @param exp Exponente (0 <= exp <= 6)
 * @return 2^exp como int8_t
 *
 * Usa tabla precalculada constexpr para máximo rendimiento.
 * Exponentes > 6 resultarán en overflow.
 */
constexpr int8_t int_power_2_int8(int exp) noexcept {
  if (exp == 0)
    return 1;
  if (exp < 0 || exp > 6)
    return 0; // Indicar overflow/error
  return power_of_2_int8[exp];
}

/**
 * @brief Potencia general optimizada para int8_t
 * @param base Base de la potencia
 * @param exp Exponente
 * @return base^exp como int8_t
 *
 * Detecta automáticamente si base es potencia de 2 para usar tabla.
 */
constexpr int8_t int_power_int8(int8_t base, int exp) noexcept {
  // Casos especiales
  if (exp == 0)
    return 1;
  if (exp == 1)
    return base;
  if (base == 0)
    return 0;
  if (base == 1)
    return 1;
  if (base == -1)
    return (exp % 2 == 0) ? 1 : -1;

  // Optimización para base = 2
  if (base == 2) {
    return int_power_2_int8(exp);
  }

  // Usar algoritmo general signed
  return int_power_signed_impl<int8_t>(base, exp);
}

//==============================================================================
// FUNCIONES ESPECIALIZADAS PARA uint8_t
//==============================================================================

/**
 * @brief Potencia optimizada para uint8_t con base 2
 * @param exp Exponente (0 <= exp <= 7)
 * @return 2^exp como uint8_t
 */
constexpr uint8_t int_power_2_uint8(int exp) noexcept {
  if (exp == 0)
    return 1;
  if (exp < 0 || exp > 7)
    return 0; // Indicar overflow/error
  return power_of_2_uint8[exp];
}

/**
 * @brief Potencia general optimizada para uint8_t
 * @param base Base de la potencia
 * @param exp Exponente
 * @return base^exp como uint8_t
 */
constexpr uint8_t int_power_uint8(uint8_t base, int exp) noexcept {
  // Casos especiales
  if (exp == 0)
    return 1;
  if (exp == 1)
    return base;
  if (base == 0)
    return 0;
  if (base == 1)
    return 1;

  // Optimización para base = 2
  if (base == 2) {
    return int_power_2_uint8(exp);
  }

  // Usar algoritmo general unsigned
  return int_power_unsigned_impl<uint8_t>(base, exp);
}

//==============================================================================
// FUNCIONES ESPECIALIZADAS PARA int16_t
//==============================================================================

/**
 * @brief Potencia optimizada para int16_t con base 2
 * @param exp Exponente (0 <= exp <= 14)
 * @return 2^exp como int16_t
 */
constexpr int16_t int_power_2_int16(int exp) noexcept {
  if (exp == 0)
    return 1;
  if (exp < 0 || exp > 14)
    return 0; // Indicar overflow/error
  return power_of_2_int16[exp];
}

/**
 * @brief Potencia general optimizada para int16_t
 * @param base Base de la potencia
 * @param exp Exponente
 * @return base^exp como int16_t
 */
constexpr int16_t int_power_int16(int16_t base, int exp) noexcept {
  // Casos especiales
  if (exp == 0)
    return 1;
  if (exp == 1)
    return base;
  if (base == 0)
    return 0;
  if (base == 1)
    return 1;
  if (base == -1)
    return (exp % 2 == 0) ? 1 : -1;

  // Optimización para base = 2
  if (base == 2) {
    return int_power_2_int16(exp);
  }

  // Optimización para otras potencias de 2 (4, 8, 16, ...)
  if (is_power_of_2(base)) {
    int base_exp = find_power_of_2_exponent(base);
    if (base_exp > 0) {
      int total_exp = base_exp * exp;
      if (total_exp <= 14) {
        return power_of_2_int16[total_exp];
      }
    }
  }

  // Usar algoritmo general signed
  return int_power_signed_impl<int16_t>(base, exp);
}

//==============================================================================
// FUNCIONES ESPECIALIZADAS PARA uint16_t
//==============================================================================

/**
 * @brief Potencia optimizada para uint16_t con base 2
 * @param exp Exponente (0 <= exp <= 15)
 * @return 2^exp como uint16_t
 */
constexpr uint16_t int_power_2_uint16(int exp) noexcept {
  if (exp == 0)
    return 1;
  if (exp < 0 || exp > 15)
    return 0; // Indicar overflow/error
  return power_of_2_uint16[exp];
}

/**
 * @brief Potencia general optimizada para uint16_t
 * @param base Base de la potencia
 * @param exp Exponente
 * @return base^exp como uint16_t
 */
constexpr uint16_t int_power_uint16(uint16_t base, int exp) noexcept {
  // Casos especiales
  if (exp == 0)
    return 1;
  if (exp == 1)
    return base;
  if (base == 0)
    return 0;
  if (base == 1)
    return 1;

  // Optimización para base = 2
  if (base == 2) {
    return int_power_2_uint16(exp);
  }

  // Optimización para otras potencias de 2 (4, 8, 16, ...)
  if (is_power_of_2(base)) {
    int base_exp = find_power_of_2_exponent(base);
    if (base_exp > 0) {
      int total_exp = base_exp * exp;
      if (total_exp <= 15) {
        return power_of_2_uint16[total_exp];
      }
    }
  }

  // Usar algoritmo general unsigned
  return int_power_unsigned_impl<uint16_t>(base, exp);
}

//==============================================================================
// FUNCIÓN TEMPLATE GENÉRICA CON ESPECIALIZACIÓN AUTOMÁTICA
//==============================================================================

/**
 * @brief Función template genérica que especializa automáticamente por tipo
 * @tparam T Tipo entero pequeño (int8_t, uint8_t, int16_t, uint16_t)
 * @param base Base de la potencia
 * @param exp Exponente
 * @return base^exp usando la optimización más apropiada
 */
template <typename T>
constexpr T int_power_small_type(T base, int exp) noexcept {
  if constexpr (std::is_same_v<T, int8_t>) {
    return int_power_int8(base, exp);
  } else if constexpr (std::is_same_v<T, uint8_t>) {
    return int_power_uint8(base, exp);
  } else if constexpr (std::is_same_v<T, int16_t>) {
    return int_power_int16(base, exp);
  } else if constexpr (std::is_same_v<T, uint16_t>) {
    return int_power_uint16(base, exp);
  } else {
    static_assert(std::is_same_v<T, int8_t> || std::is_same_v<T, uint8_t> ||
                      std::is_same_v<T, int16_t> || std::is_same_v<T, uint16_t>,
                  "int_power_small_type solo soporta tipos pequeños");
    return T{0}; // Nunca debería llegar aquí
  }
}

/**
 * @brief Verifica si un tipo es considerado "pequeño" para optimizaciones
 * @tparam T Tipo a verificar
 * @return true si T es int8_t, uint8_t, int16_t o uint16_t
 */
template <typename T> constexpr bool is_small_integral_type() {
  return std::is_same_v<T, int8_t> || std::is_same_v<T, uint8_t> ||
         std::is_same_v<T, int16_t> || std::is_same_v<T, uint16_t>;
}

} // namespace small_types
} // namespace integral_power_functions
} // namespace number_calc