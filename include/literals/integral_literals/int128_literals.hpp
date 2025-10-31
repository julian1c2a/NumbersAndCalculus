#pragma once

/**
 * @file int128_literals.hpp
 * @brief Literales de usuario para tipos __int128 y unsigned __int128
 * @author AlgoritmiaCombinatoria Project
 * @date 2024
 * @version 1.0
 *
 * Este archivo define literales de usuario (UDL) para crear valores
 * __int128 y unsigned __int128 de forma conveniente en el código fuente.
 *
 * Parte del módulo literals/integral_literals/ en la nueva arquitectura
 * modular.
 */

#include "../../io/integral_types/int128_io.hpp"
#include "../../type_traits/extended_integral_traits.hpp"
#include <stdexcept>
#include <string>


namespace literals {
namespace integral_literals {

#if defined(__GNUC__) || defined(__clang__)

//==============================================================================
// LITERALES PARA __INT128 (SIGNED)
//==============================================================================

/**
 * @brief Literal para crear valores __int128 desde strings decimales
 * @param str String literal que representa el número
 * @param len Longitud del string
 * @return Valor __int128
 *
 * Uso: auto big = "123456789012345678901234567890"_i128;
 */
constexpr __int128 operator"" _i128(const char *str, size_t len) {
  // Implementación constexpr limitada - solo para casos simples
  if (len == 0)
    return 0;

  __int128 result = 0;
  bool negative = false;
  size_t start = 0;

  if (str[0] == '-') {
    negative = true;
    start = 1;
  } else if (str[0] == '+') {
    start = 1;
  }

  for (size_t i = start; i < len; ++i) {
    if (str[i] < '0' || str[i] > '9') {
      throw std::invalid_argument("Carácter inválido en literal _i128");
    }
    result = result * 10 + (str[i] - '0');
  }

  return negative ? -result : result;
}

/**
 * @brief Literal para crear valores __int128 desde enteros
 * @param value Valor entero literal
 * @return Valor __int128
 *
 * Uso: auto big = 123456789_i128;
 */
constexpr __int128 operator"" _i128(unsigned long long value) {
  return static_cast<__int128>(value);
}

//==============================================================================
// LITERALES PARA UNSIGNED __INT128 (UNSIGNED)
//==============================================================================

/**
 * @brief Literal para crear valores unsigned __int128 desde strings decimales
 * @param str String literal que representa el número
 * @param len Longitud del string
 * @return Valor unsigned __int128
 *
 * Uso: auto big = "123456789012345678901234567890"_u128;
 */
constexpr unsigned __int128 operator"" _u128(const char *str, size_t len) {
  if (len == 0)
    return 0;

  // No permitir números negativos para unsigned
  if (len > 0 && str[0] == '-') {
    throw std::invalid_argument("Número negativo en literal _u128");
  }

  unsigned __int128 result = 0;
  size_t start = 0;

  if (str[0] == '+') {
    start = 1;
  }

  for (size_t i = start; i < len; ++i) {
    if (str[i] < '0' || str[i] > '9') {
      throw std::invalid_argument("Carácter inválido en literal _u128");
    }
    result = result * 10 + (str[i] - '0');
  }

  return result;
}

/**
 * @brief Literal para crear valores unsigned __int128 desde enteros
 * @param value Valor entero literal
 * @return Valor unsigned __int128
 *
 * Uso: auto big = 123456789_u128;
 */
constexpr unsigned __int128 operator"" _u128(unsigned long long value) {
  return static_cast<unsigned __int128>(value);
}

//==============================================================================
// LITERALES HEXADECIMALES
//==============================================================================

/**
 * @brief Literal para crear valores __int128 desde strings hexadecimales
 * @param str String literal hexadecimal (sin 0x)
 * @param len Longitud del string
 * @return Valor __int128
 *
 * Uso: auto big = "DEADBEEFCAFEBABE"_i128_hex;
 */
constexpr __int128 operator"" _i128_hex(const char *str, size_t len) {
  if (len == 0)
    return 0;

  __int128 result = 0;
  for (size_t i = 0; i < len; ++i) {
    char c = str[i];
    int digit;

    if (c >= '0' && c <= '9') {
      digit = c - '0';
    } else if (c >= 'A' && c <= 'F') {
      digit = c - 'A' + 10;
    } else if (c >= 'a' && c <= 'f') {
      digit = c - 'a' + 10;
    } else {
      throw std::invalid_argument("Carácter inválido en literal hexadecimal");
    }

    result = result * 16 + digit;
  }

  return result;
}

/**
 * @brief Literal para crear valores unsigned __int128 desde strings
 * hexadecimales
 * @param str String literal hexadecimal (sin 0x)
 * @param len Longitud del string
 * @return Valor unsigned __int128
 *
 * Uso: auto big = "DEADBEEFCAFEBABE"_u128_hex;
 */
constexpr unsigned __int128 operator"" _u128_hex(const char *str, size_t len) {
  if (len == 0)
    return 0;

  unsigned __int128 result = 0;
  for (size_t i = 0; i < len; ++i) {
    char c = str[i];
    int digit;

    if (c >= '0' && c <= '9') {
      digit = c - '0';
    } else if (c >= 'A' && c <= 'F') {
      digit = c - 'A' + 10;
    } else if (c >= 'a' && c <= 'f') {
      digit = c - 'a' + 10;
    } else {
      throw std::invalid_argument("Carácter inválido en literal hexadecimal");
    }

    result = result * 16 + digit;
  }

  return result;
}

//==============================================================================
// LITERALES BINARIOS
//==============================================================================

/**
 * @brief Literal para crear valores __int128 desde strings binarios
 * @param str String literal binario (sin 0b)
 * @param len Longitud del string
 * @return Valor __int128
 *
 * Uso: auto big = "1010101010101010"_i128_bin;
 */
constexpr __int128 operator"" _i128_bin(const char *str, size_t len) {
  if (len == 0)
    return 0;

  __int128 result = 0;
  for (size_t i = 0; i < len; ++i) {
    if (str[i] != '0' && str[i] != '1') {
      throw std::invalid_argument("Carácter inválido en literal binario");
    }
    result = result * 2 + (str[i] - '0');
  }

  return result;
}

/**
 * @brief Literal para crear valores unsigned __int128 desde strings binarios
 * @param str String literal binario (sin 0b)
 * @param len Longitud del string
 * @return Valor unsigned __int128
 *
 * Uso: auto big = "1010101010101010"_u128_bin;
 */
constexpr unsigned __int128 operator"" _u128_bin(const char *str, size_t len) {
  if (len == 0)
    return 0;

  unsigned __int128 result = 0;
  for (size_t i = 0; i < len; ++i) {
    if (str[i] != '0' && str[i] != '1') {
      throw std::invalid_argument("Carácter inválido en literal binario");
    }
    result = result * 2 + (str[i] - '0');
  }

  return result;
}

#endif // __GNUC__ || __clang__

} // namespace integral_literals
} // namespace literals

//==============================================================================
// ALIAS PÚBLICOS PARA USO CONVENIENTE
//==============================================================================

#if defined(__GNUC__) || defined(__clang__)

// Namespace inline para que los literales estén disponibles automáticamente
namespace literals {
inline namespace integral_literals {
// Los literales se importan automáticamente
}
} // namespace literals

// Para compatibilidad con código existente, también en namespace global
using namespace literals::integral_literals;

#endif