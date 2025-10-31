#pragma once

/**
 * @file extended_integral_traits.hpp
 * @brief Traits extendidos que incluyen soporte para __int128 y tipos no
 * estándar
 * @author AlgoritmiaCombinatoria Project
 * @date 2024
 * @version 1.0
 *
 * Este archivo define traits personalizados que extienden los traits estándar
 * de C++ para incluir tipos como __int128 que no son reconocidos por
 * std::is_integral, std::is_signed, etc.
 */

#include "number_calc.hpp"
#include <type_traits>

using namespace number_calc;

namespace number_calc {
namespace type_traits {

//==============================================================================
// TRAITS EXTENDIDOS PARA TIPOS INTEGRALES
//==============================================================================

/**
 * @brief Trait extendido que incluye __int128 como tipo integral
 * @tparam T Tipo a verificar
 *
 * Extiende std::is_integral para incluir __int128 y unsigned __int128
 * en compiladores que los soportan.
 */
template <typename T> struct is_integral_extended : std::is_integral<T> {};

#if defined(__GNUC__) || defined(__clang__)
template <> struct is_integral_extended<__int128> : std::true_type {};

template <> struct is_integral_extended<unsigned __int128> : std::true_type {};
#endif

template <typename T>
constexpr bool is_integral_extended_v = is_integral_extended<T>::value;

//==============================================================================
// TRAITS EXTENDIDOS PARA TIPOS SIGNED
//==============================================================================

/**
 * @brief Trait extendido que incluye __int128 como tipo signed
 * @tparam T Tipo a verificar
 *
 * Extiende std::is_signed para incluir __int128 correctamente.
 */
template <typename T> struct is_signed_extended : std::is_signed<T> {};

#if defined(__GNUC__) || defined(__clang__)
template <> struct is_signed_extended<__int128> : std::true_type {};

template <> struct is_signed_extended<unsigned __int128> : std::false_type {};
#endif

template <typename T>
constexpr bool is_signed_extended_v = is_signed_extended<T>::value;

//==============================================================================
// TRAITS EXTENDIDOS PARA TIPOS UNSIGNED
//==============================================================================

/**
 * @brief Trait extendido que incluye unsigned __int128 como tipo unsigned
 * @tparam T Tipo a verificar
 *
 * Extiende std::is_unsigned para incluir unsigned __int128 correctamente.
 */
template <typename T> struct is_unsigned_extended : std::is_unsigned<T> {};

#if defined(__GNUC__) || defined(__clang__)
template <> struct is_unsigned_extended<__int128> : std::false_type {};

template <> struct is_unsigned_extended<unsigned __int128> : std::true_type {};
#endif

template <typename T>
constexpr bool is_unsigned_extended_v = is_unsigned_extended<T>::value;

//==============================================================================
// TRAIT HELPERS PARA VALIDACIÓN
//==============================================================================

/**
 * @brief Concept-like helper para validar tipos integrales extendidos
 * @tparam T Tipo a validar
 */
template <typename T> constexpr bool is_valid_integral_type() {
  return is_integral_extended_v<T>;
}

/**
 * @brief Concept-like helper para validar tipos signed extendidos
 * @tparam T Tipo a validar
 */
template <typename T> constexpr bool is_valid_signed_type() {
  return is_integral_extended_v<T> && is_signed_extended_v<T>;
}

/**
 * @brief Concept-like helper para validar tipos unsigned extendidos
 * @tparam T Tipo a validar
 */
template <typename T> constexpr bool is_valid_unsigned_type() {
  return is_integral_extended_v<T> && is_unsigned_extended_v<T>;
}

} // namespace type_traits
} // namespace number_calc

// Función inteligente (recomendada)
int result = int_power_smart(2, 10); // Usa optimización bit-shifting

// Funciones específicas
auto pow2 = int_power_2<int>(20);            // Especializada para 2^n
auto [res, ok] = int_power_safe_pair(3, 15); // Con verificación