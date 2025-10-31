#pragma once

/**
 * @file int128_io.hpp
 * @brief Operadores de entrada/salida optimizados para tipos __int128
 * @author AlgoritmiaCombinatoria Project
 * @date 2024
 * @version 2.0
 *
 * Este archivo proporciona operadores de E/S y utilidades de conversión
 * optimizadas específicamente para __int128 y unsigned __int128.
 *
 * Parte del módulo io/integral_types/ en la nueva arquitectura modular.
 */

#include "../../type_traits/extended_integral_traits.hpp"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>


namespace io {
namespace integral_types {

using namespace number_calc::type_traits;

//==============================================================================
// VERIFICACIÓN DE SOPORTE __INT128
//==============================================================================

#if defined(__GNUC__) || defined(__clang__)
#define IO_INT128_SUPPORTED 1
#else
#define IO_INT128_SUPPORTED 0
#endif

#if IO_INT128_SUPPORTED

//==============================================================================
// CONVERSIONES DE __INT128 A STRING
//==============================================================================

/**
 * @brief Convierte __int128 a string en base decimal
 * @param value Valor __int128 a convertir
 * @return String representando el valor
 *
 * Implementación optimizada para __int128 que evita usar
 * bibliotecas externas como Boost.
 */
inline std::string int128_to_string(__int128 value) {
  if (value == 0)
    return "0";

  std::string result;
  bool negative = value < 0;

  if (negative) {
    value = -value;
  }

  while (value > 0) {
    result = char('0' + (value % 10)) + result;
    value /= 10;
  }

  if (negative) {
    result = '-' + result;
  }

  return result;
}

/**
 * @brief Convierte unsigned __int128 a string en base decimal
 * @param value Valor unsigned __int128 a convertir
 * @return String representando el valor
 */
inline std::string uint128_to_string(unsigned __int128 value) {
  if (value == 0)
    return "0";

  std::string result;
  while (value > 0) {
    result = char('0' + (value % 10)) + result;
    value /= 10;
  }

  return result;
}

//==============================================================================
// CONVERSIONES DE STRING A __INT128
//==============================================================================

/**
 * @brief Convierte string a __int128
 * @param str String a convertir
 * @param base Base numérica (2, 8, 10, 16)
 * @return Valor __int128
 * @throws std::invalid_argument si el string no es válido
 * @throws std::out_of_range si el valor no cabe en __int128
 */
inline __int128 string_to_int128(const std::string &str, int base = 10) {
  if (str.empty()) {
    throw std::invalid_argument("string_to_int128: string vacío");
  }

  if (base < 2 || base > 36) {
    throw std::invalid_argument(
        "string_to_int128: base debe estar entre 2 y 36");
  }

  __int128 result = 0;
  bool negative = false;
  size_t start = 0;

  if (str[0] == '-') {
    negative = true;
    start = 1;
  } else if (str[0] == '+') {
    start = 1;
  }

  for (size_t i = start; i < str.length(); ++i) {
    char c = str[i];
    int digit;

    if (c >= '0' && c <= '9') {
      digit = c - '0';
    } else if (c >= 'A' && c <= 'Z') {
      digit = c - 'A' + 10;
    } else if (c >= 'a' && c <= 'z') {
      digit = c - 'a' + 10;
    } else {
      throw std::invalid_argument("string_to_int128: carácter inválido");
    }

    if (digit >= base) {
      throw std::invalid_argument("string_to_int128: dígito fuera de base");
    }

    // Verificar overflow antes de multiplicar
    __int128 max_div = std::numeric_limits<__int128>::max() / base;
    if (result > max_div) {
      throw std::out_of_range("string_to_int128: overflow");
    }

    result = result * base + digit;
  }

  return negative ? -result : result;
}

//==============================================================================
// OPERADORES DE SALIDA (operator<<)
//==============================================================================

/**
 * @brief Operador de salida para __int128
 * @param os Stream de salida
 * @param value Valor __int128 a imprimir
 * @return Referencia al stream
 */
inline std::ostream &operator<<(std::ostream &os, __int128 value) {
  os << int128_to_string(value);
  return os;
}

/**
 * @brief Operador de salida para unsigned __int128
 * @param os Stream de salida
 * @param value Valor unsigned __int128 a imprimir
 * @return Referencia al stream
 */
inline std::ostream &operator<<(std::ostream &os, unsigned __int128 value) {
  os << uint128_to_string(value);
  return os;
}

//==============================================================================
// OPERADORES DE ENTRADA (operator>>)
//==============================================================================

/**
 * @brief Operador de entrada para __int128
 * @param is Stream de entrada
 * @param value Referencia donde almacenar el valor leído
 * @return Referencia al stream
 */
inline std::istream &operator>>(std::istream &is, __int128 &value) {
  std::string str;
  is >> str;

  if (is.fail()) {
    return is;
  }

  try {
    value = string_to_int128(str);
  } catch (const std::exception &) {
    is.setstate(std::ios::failbit);
  }

  return is;
}

/**
 * @brief Operador de entrada para unsigned __int128
 * @param is Stream de entrada
 * @param value Referencia donde almacenar el valor leído
 * @return Referencia al stream
 */
inline std::istream &operator>>(std::istream &is, unsigned __int128 &value) {
  std::string str;
  is >> str;

  if (is.fail()) {
    return is;
  }

  try {
    // Para unsigned, no permitir números negativos
    if (!str.empty() && str[0] == '-') {
      is.setstate(std::ios::failbit);
      return is;
    }

    __int128 temp = string_to_int128(str);
    if (temp < 0) {
      is.setstate(std::ios::failbit);
      return is;
    }

    value = static_cast<unsigned __int128>(temp);
  } catch (const std::exception &) {
    is.setstate(std::ios::failbit);
  }

  return is;
}

#endif // IO_INT128_SUPPORTED

} // namespace integral_types
} // namespace io

//==============================================================================
// ALIAS PÚBLICOS PARA COMPATIBILIDAD
//==============================================================================

#if IO_INT128_SUPPORTED
// Exportar al namespace global para compatibilidad con código existente
using io::integral_types::operator<<;
using io::integral_types::operator>>;
using io::integral_types::int128_to_string;
using io::integral_types::string_to_int128;
using io::integral_types::uint128_to_string;
#endif