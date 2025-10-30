/**
 * @file custom_literals.hpp
 * @brief Literales definidos por el usuario para tipos de precisión arbitraria
 * @author AlgoritmiaCombinatoria Project
 * @date 2024
 * @version 1.0
 *
 * Este archivo define literales definidos por el usuario (UDL) para tipos
 * de Boost.Multiprecision y tipos no estándar como __int128.
 *
 * @note Los UDL están disponibles desde C++11
 * @warning Los nombres de sufijos deben comenzar con _ para UDL personalizados
 */

#pragma once

#include <boost/multiprecision/cpp_int.hpp>
#include <cstring>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>

#ifdef BOOST_HAS_GMP
#include <boost/multiprecision/gmp.hpp>
#endif

#ifdef BOOST_HAS_TOMMATH
#include <boost/multiprecision/tommath.hpp>
#endif

//==============================================================================
// NAMESPACE PARA LITERALES PERSONALIZADOS
//==============================================================================

namespace custom_literals {

using mp = boost::multiprecision;

//------------------------------------------------------------------------------
// LITERALES PARA BOOST.MULTIPRECISION
//------------------------------------------------------------------------------

/**
 * @brief Literal para cpp_int desde string
 * @param str String que representa el número
 * @return cpp_int construido desde la string
 *
 * Uso: auto big_num = "123456789012345678901234567890"_cpp_int;
 */
inline mp::cpp_int operator"" _cpp_int(const char *str) {
  return mp::cpp_int(str);
}

/**
 * @brief Literal para cpp_int desde entero
 * @param value Valor entero
 * @return cpp_int construido desde el entero
 *
 * Uso: auto big_num = 123456789_cpp_int;
 * @note constexpr permite evaluación en tiempo de compilación
 */
constexpr inline mp::cpp_int operator"" _cpp_int(unsigned long long value) {
  return mp::cpp_int(value);
}

#ifdef BOOST_HAS_GMP
/**
 * @brief Literal para mpz_int desde string
 * @param str String que representa el número
 * @return mpz_int construido desde la string
 *
 * Uso: auto big_num = "123456789012345678901234567890"_mpz_int;
 */
inline mp::mpz_int operator"" _mpz_int(const char *str) {
  return mp::mpz_int(str);
}

/**
 * @brief Literal para mpz_int desde entero
 * @param value Valor entero
 * @return mpz_int construido desde el entero
 */
inline mp::mpz_int operator"" _mpz_int(unsigned long long value) {
  return mp::mpz_int(value);
}
#endif // BOOST_HAS_GMP

#ifdef BOOST_HAS_TOMMATH
/**
 * @brief Literal para tom_int desde string
 * @param str String que representa el número
 * @return tom_int construido desde la string
 */
inline mp::tom_int operator"" _tom_int(const char *str) {
  return mp::tom_int(str);
}

/**
 * @brief Literal para tom_int desde entero
 * @param value Valor entero
 * @return tom_int construido desde el entero
 */
inline mp::tom_int operator"" _tom_int(unsigned long long value) {
  return mp::tom_int(value);
}
#endif // BOOST_HAS_TOMMATH

//------------------------------------------------------------------------------
// LITERALES PARA TIPOS DE TAMAÑO FIJO DE BOOST
//------------------------------------------------------------------------------

/**
 * @brief Literal para int128_t de Boost
 * @param value Valor entero
 * @return int128_t de Boost
 *
 * Uso: auto num = 123456789_int128;
 */
inline mp::int128_t operator"" _int128(unsigned long long value) {
  return mp::int128_t(value);
}

/**
 * @brief Literal para uint128_t de Boost
 * @param value Valor entero
 * @return uint128_t de Boost
 */
inline mp::uint128_t operator"" _uint128(unsigned long long value) {
  return mp::uint128_t(value);
}

//------------------------------------------------------------------------------
// LITERALES PARA TIPOS NO ESTÁNDAR (__int128)
//------------------------------------------------------------------------------

#if defined(__GNUC__) || defined(__clang__)

/**
 * @brief Literal para __int128 desde entero
 * @param value Valor entero
 * @return __int128
 *
 * Uso: auto num = 123456789_i128;
 *
 * @note __int128 no tiene sufijos nativos en C++
 * @note Este literal personalizado facilita la construcción
 * @note constexpr permite evaluación en tiempo de compilación
 */
constexpr inline __int128 operator"" _i128(unsigned long long value) noexcept {
  return static_cast<__int128>(value);
}

/**
 * @brief Literal para unsigned __int128 desde entero
 * @param value Valor entero
 * @return unsigned __int128
 * @note constexpr permite evaluación en tiempo de compilación
 */
constexpr inline unsigned __int128
operator"" _ui128(unsigned long long value) noexcept {
  return static_cast<unsigned __int128>(value);
}

/**
 * @brief Literal para __int128 desde string (hexadecimal)
 * @param str String en formato hexadecimal
 * @return __int128
 *
 * Uso: auto num = "DEADBEEFCAFEBABE"_hex128;
 *
 * @note Útil para crear valores grandes que no caben en long long
 */
__int128 operator"" _hex128(const char *str);

/**
 * @brief Literal para crear potencias de 2 con __int128
 * @param exponent Exponente (0-127)
 * @return 2^exponent como __int128
 *
 * Uso: auto power = 100_pow2_i128;  // 2^100
 * @note constexpr permite calcular potencias en tiempo de compilación
 * @note Ideal para optimizaciones de factoriales y combinatoria
 */
constexpr inline __int128
operator"" _pow2_i128(unsigned long long exponent) noexcept {
  if (exponent >= 127) {
    // Retornar el valor máximo para evitar overflow
    return static_cast<__int128>(1) << 126;
  }
  return static_cast<__int128>(1) << exponent;
}

#endif // defined(__GNUC__) || defined(__clang__)

//------------------------------------------------------------------------------
// LITERALES HELPER PARA BASES NUMÉRICAS
//------------------------------------------------------------------------------

/**
 * @brief Literal para crear cpp_int desde string binaria
 * @param str String en formato binario (sin prefijo 0b)
 * @return cpp_int
 *
 * Uso: auto num = "11110000111100001111000011110000"_bin;
 */
boost::multiprecision::cpp_int operator"" _bin(const char *str);

/**
 * @brief Literal para crear cpp_int desde string hexadecimal
 * @param str String en formato hexadecimal (sin prefijo 0x)
 * @return cpp_int
 *
 * Uso: auto num = "DEADBEEFCAFEBABE123456789ABCDEF0"_hex;
 */
boost::multiprecision::cpp_int operator"" _hex(const char *str);

} // namespace custom_literals

//==============================================================================
// IMPLEMENTACIONES
//==============================================================================

namespace custom_literals {

#if defined(__GNUC__) || defined(__clang__)
__int128 operator"" _hex128(const char *str) {
  __int128 result = 0;
  for (const char *p = str; *p; ++p) {
    result <<= 4;
    if (*p >= '0' && *p <= '9') {
      result |= (*p - '0');
    } else if (*p >= 'A' && *p <= 'F') {
      result |= (*p - 'A' + 10);
    } else if (*p >= 'a' && *p <= 'f') {
      result |= (*p - 'a' + 10);
    }
    // Ignorar caracteres inválidos
  }
  return result;
}
#endif

using mp = boost::multiprecision;

mp::cpp_int operator"" _bin(const char *str) {
  mp::cpp_int result = 0;
  for (const char *p = str; *p; ++p) {
    result <<= 1;
    if (*p == '1') {
      result |= 1;
    }
    // Ignorar '0' y caracteres inválidos
  }
  return result;
}

mp::cpp_int operator"" _hex(const char *str) {
  mp::cpp_int result = 0;
  for (const char *p = str; *p; ++p) {
    result <<= 4;
    if (*p >= '0' && *p <= '9') {
      result |= (*p - '0');
    } else if (*p >= 'A' && *p <= 'F') {
      result |= (*p - 'A' + 10);
    } else if (*p >= 'a' && *p <= 'f') {
      result |= (*p - 'a' + 10);
    }
  }
  return result;
}

//==============================================================================
// OPERADORES DE ENTRADA/SALIDA PARA STREAMS
//==============================================================================

//------------------------------------------------------------------------------
// DECLARACIONES ADELANTADAS
//------------------------------------------------------------------------------

#if defined(__GNUC__) || defined(__clang__)
__int128 from_string_i128(const std::string &str);
unsigned __int128 from_string_ui128(const std::string &str);
#endif

//------------------------------------------------------------------------------
// OPERADORES DE SALIDA (operator<<)
//------------------------------------------------------------------------------

#if defined(__GNUC__) || defined(__clang__)

/**
 * @brief Operador de salida para __int128
 * @param os Stream de salida
 * @param value Valor __int128 a imprimir
 * @return Referencia al stream
 *
 * @note __int128 no tiene soporte nativo en std::ostream
 */
std::ostream &operator<<(std::ostream &os, __int128 value) {
  if (value == 0) {
    return os << "0";
  }

  bool negative = false;
  if (value < 0) {
    negative = true;
    // Evitar overflow en caso de valor mínimo
    if (value == std::numeric_limits<__int128>::min()) {
      return os << "-170141183460469231731687303715884105728";
    }
    value = -value;
  }

  std::string result;
  while (value > 0) {
    result = char('0' + (value % 10)) + result;
    value /= 10;
  }

  if (negative) {
    result = "-" + result;
  }

  return os << result;
}

/**
 * @brief Operador de salida para unsigned __int128
 * @param os Stream de salida
 * @param value Valor unsigned __int128 a imprimir
 * @return Referencia al stream
 */
std::ostream &operator<<(std::ostream &os, unsigned __int128 value) {
  if (value == 0) {
    return os << "0";
  }

  std::string result;
  while (value > 0) {
    result = char('0' + (value % 10)) + result;
    value /= 10;
  }

  return os << result;
}

#endif // defined(__GNUC__) || defined(__clang__)

//------------------------------------------------------------------------------
// OPERADORES DE ENTRADA (operator>>)
//------------------------------------------------------------------------------

#if defined(__GNUC__) || defined(__clang__)

/**
 * @brief Operador de entrada para __int128
 * @param is Stream de entrada
 * @param value Referencia donde almacenar el valor leído
 * @return Referencia al stream
 *
 * @throws std::invalid_argument Si la string no es un número válido
 * @throws std::out_of_range Si el número está fuera del rango de __int128
 */
std::istream &operator>>(std::istream &is, __int128 &value) {
  std::string str;
  is >> str;

  if (str.empty()) {
    is.setstate(std::ios::failbit);
    return is;
  }

  try {
    value = from_string_i128(str);
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
std::istream &operator>>(std::istream &is, unsigned __int128 &value) {
  std::string str;
  is >> str;

  if (str.empty()) {
    is.setstate(std::ios::failbit);
    return is;
  }

  try {
    value = from_string_ui128(str);
  } catch (const std::exception &) {
    is.setstate(std::ios::failbit);
  }

  return is;
}

#endif // defined(__GNUC__) || defined(__clang__)

//------------------------------------------------------------------------------
// FUNCIONES DE CONVERSIÓN A STRING
//------------------------------------------------------------------------------

#if defined(__GNUC__) || defined(__clang__)

/**
 * @brief Convierte __int128 a std::string
 * @param value Valor a convertir
 * @return String representando el valor
 */
std::string to_string(__int128 value) {
  std::ostringstream oss;
  oss << value;
  return oss.str();
}

/**
 * @brief Convierte unsigned __int128 a std::string
 * @param value Valor a convertir
 * @return String representando el valor
 */
std::string to_string(unsigned __int128 value) {
  std::ostringstream oss;
  oss << value;
  return oss.str();
}

/**
 * @brief Convierte __int128 a C-string (char*)
 * @param value Valor a convertir
 * @return Puntero a C-string (debe ser liberado con delete[])
 *
 * @warning El puntero retornado debe ser liberado con delete[]
 * @deprecated Considerar usar to_string() en su lugar
 */
[[deprecated("Use to_string() instead")]]
char *to_cstr(__int128 value) {
  std::string str = to_string(value);
  char *result = new char[str.length() + 1];
  std::strcpy(result, str.c_str());
  return result;
}

/**
 * @brief Convierte unsigned __int128 a C-string (char*)
 * @param value Valor a convertir
 * @return Puntero a C-string (debe ser liberado con delete[])
 *
 * @warning El puntero retornado debe ser liberado con delete[]
 * @deprecated Considerar usar to_string() en su lugar
 */
[[deprecated("Use to_string() instead")]]
char *to_cstr(unsigned __int128 value) {
  std::string str = to_string(value);
  char *result = new char[str.length() + 1];
  std::strcpy(result, str.c_str());
  return result;
}

#endif // defined(__GNUC__) || defined(__clang__)

//------------------------------------------------------------------------------
// FUNCIONES DE CONVERSIÓN DESDE STRING
//------------------------------------------------------------------------------

#if defined(__GNUC__) || defined(__clang__)

/**
 * @brief Convierte std::string a __int128
 * @param str String a convertir
 * @return Valor __int128
 *
 * @throws std::invalid_argument Si la string no es un número válido
 * @throws std::out_of_range Si el número está fuera del rango de __int128
 */
__int128 from_string_i128(const std::string &str) {
  if (str.empty()) {
    throw std::invalid_argument("Empty string cannot be converted to __int128");
  }

  __int128 result = 0;
  bool negative = false;
  size_t start = 0;

  // Verificar signo
  if (str[0] == '-') {
    negative = true;
    start = 1;
  } else if (str[0] == '+') {
    start = 1;
  }

  if (start >= str.length()) {
    throw std::invalid_argument("Invalid number format");
  }

  // Convertir dígito por dígito
  for (size_t i = start; i < str.length(); ++i) {
    if (str[i] < '0' || str[i] > '9') {
      throw std::invalid_argument("Invalid character in number string");
    }

    __int128 digit = str[i] - '0';

    // Verificar overflow antes de multiplicar
    __int128 max_div_10 = std::numeric_limits<__int128>::max() / 10;
    if (!negative && result > max_div_10) {
      throw std::out_of_range("Number too large for __int128");
    }

    result *= 10;

    // Verificar overflow después de sumar
    if (!negative && result > std::numeric_limits<__int128>::max() - digit) {
      throw std::out_of_range("Number too large for __int128");
    }

    result += digit;
  }

  if (negative) {
    // Verificar underflow
    if (result >
        static_cast<unsigned __int128>(std::numeric_limits<__int128>::max()) +
            1) {
      throw std::out_of_range("Number too small for __int128");
    }
    result = -result;
  }

  return result;
}

/**
 * @brief Convierte std::string a unsigned __int128
 * @param str String a convertir
 * @return Valor unsigned __int128
 *
 * @throws std::invalid_argument Si la string no es un número válido
 * @throws std::out_of_range Si el número está fuera del rango de unsigned
 * __int128
 */
unsigned __int128 from_string_ui128(const std::string &str) {
  if (str.empty()) {
    throw std::invalid_argument(
        "Empty string cannot be converted to unsigned __int128");
  }

  unsigned __int128 result = 0;
  size_t start = 0;

  // Verificar signo (no permitir negativos para unsigned)
  if (str[0] == '-') {
    throw std::invalid_argument(
        "Negative numbers not allowed for unsigned __int128");
  } else if (str[0] == '+') {
    start = 1;
  }

  if (start >= str.length()) {
    throw std::invalid_argument("Invalid number format");
  }

  // Convertir dígito por dígito
  for (size_t i = start; i < str.length(); ++i) {
    if (str[i] < '0' || str[i] > '9') {
      throw std::invalid_argument("Invalid character in number string");
    }

    unsigned __int128 digit = str[i] - '0';

    // Verificar overflow antes de multiplicar
    unsigned __int128 max_div_10 =
        std::numeric_limits<unsigned __int128>::max() / 10;
    if (result > max_div_10) {
      throw std::out_of_range("Number too large for unsigned __int128");
    }

    result *= 10;

    // Verificar overflow después de sumar
    if (result > std::numeric_limits<unsigned __int128>::max() - digit) {
      throw std::out_of_range("Number too large for unsigned __int128");
    }

    result += digit;
  }

  return result;
}

/**
 * @brief Convierte C-string a __int128
 * @param cstr C-string a convertir
 * @return Valor __int128
 *
 * @throws std::invalid_argument Si cstr es nullptr o no es un número válido
 * @throws std::out_of_range Si el número está fuera del rango de __int128
 */
__int128 from_cstr_i128(const char *cstr) {
  if (cstr == nullptr) {
    throw std::invalid_argument("Null pointer cannot be converted to __int128");
  }
  return from_string_i128(std::string(cstr));
}

/**
 * @brief Convierte C-string a unsigned __int128
 * @param cstr C-string a convertir
 * @return Valor unsigned __int128
 *
 * @throws std::invalid_argument Si cstr es nullptr o no es un número válido
 * @throws std::out_of_range Si el número está fuera del rango de unsigned
 * __int128
 */
unsigned __int128 from_cstr_ui128(const char *cstr) {
  if (cstr == nullptr) {
    throw std::invalid_argument(
        "Null pointer cannot be converted to unsigned __int128");
  }
  return from_string_ui128(std::string(cstr));
}

#endif // defined(__GNUC__) || defined(__clang__)

//------------------------------------------------------------------------------
// FUNCIONES DE CONVERSIÓN PARA BOOST.MULTIPRECISION
//------------------------------------------------------------------------------

/**
 * @brief Convierte cpp_int a std::string con base especificada
 * @param value Valor a convertir
 * @param base Base numérica (2-36, por defecto 10)
 * @return String representando el valor
 */
std::string to_string(const mp::cpp_int &value, int base = 10) {
  return value.str(base);
}

/**
 * @brief Convierte std::string a cpp_int
 * @param str String a convertir
 * @param base Base numérica (0 para autodetección, 2-36)
 * @return Valor cpp_int
 *
 * @throws std::invalid_argument Si la string no es un número válido en la base
 * especificada
 */
mp::cpp_int from_string_cpp_int(const std::string &str, int base = 10) {
  try {
    return mp::cpp_int(str, base);
  } catch (const std::exception &e) {
    throw std::invalid_argument("Invalid string for cpp_int conversion: " +
                                std::string(e.what()));
  }
}

/**
 * @brief Convierte C-string a cpp_int
 * @param cstr C-string a convertir
 * @param base Base numérica (0 para autodetección, 2-36)
 * @return Valor cpp_int
 *
 * @throws std::invalid_argument Si cstr es nullptr o no es un número válido
 */
mp::cpp_int from_cstr_cpp_int(const char *cstr, int base = 10) {
  if (cstr == nullptr) {
    throw std::invalid_argument("Null pointer cannot be converted to cpp_int");
  }
  return from_string_cpp_int(std::string(cstr), base);
}

} // namespace custom_literals

//==============================================================================
// EJEMPLO DE USO
//==============================================================================

/*
#include "custom_literals.hpp"

int main() {
    using namespace custom_literals;

    // Literales para Boost.Multiprecision
    auto big1 = "123456789012345678901234567890"_cpp_int;
    auto big2 = 987654321_cpp_int;

    // Literales para __int128
    auto i128_val = 123456789_i128;
    auto power_of_2 = 100_pow2_i128;  // 2^100
    auto hex_val = "DEADBEEFCAFEBABE"_hex128;

    // Literales para bases específicas
    auto binary = "11110000111100001111000011110000"_bin;
    auto hexadecimal = "DEADBEEF"_hex;

    return 0;
}
*/

mp::cpp_int big_num("123456789012345678901234567890");
mp::cpp_int from_value = 42;

/**
 * @brief Factorial de 20 (precalculado)
 */
constexpr __int128 factorial_20 = 2432902008176640000; // Precalculado

/**stexpr auto cat = constexpr_catalan(10);          // Catalan(10) en
 * compile-timeconstexpr auto fib = constexpr_fibonacci(50);        //
 * Fibonacci(50) en compile-timeconstexpr auto perm = constexpr_permutation(20,
 * 5);  // P(20,5) en compile-timeconstexpr auto comb =
 * constexpr_combination(50, 25); // C(50,25) en compile-timeconstexpr auto fact
 * = constexpr_factorial(20);       // 20! en compile-time
 * @brief 2^64 (precalculado)
 */
@brief Factorial de 20(precalculado) constexpr __int128 pow2_64 =
    18446744073709551616;                              // 2^64 precalculado
constexpr __int128 factorial_20 = 2432902008176640000; // Precalculado
                                                       /**
                                                        * @brief Fibonacci de 50 (precalculado)
                                                        */ @brief 2 ^ 64(precalculado) constexpr __int128 fibonacci_50 =
    12586269025;                                       // Fib(50) precalculado

constexpr auto ubig =
    999999999999999999_ui128; // Unsigned 128-bitconstexpr auto power =
                              // 64_pow2_i128;                 // 2^64 en
                              // compile-timeconstexpr auto big =
                              // 123456789012345_i128;           //
                              // Compile-timeconstexpr __int128 pow2_64 =
                              // 18446744073709551616;      // 2^64 precalculado

/**
 * @brief Fibonacci de 50 (precalculado)
 */
constexpr __int128 fibonacci_50 = 12586269025; // Fib(50) precalculado