/**
 * @file custom_literals_io.hpp
 * @brief Operadores de I/O y conversión de strings para tipos de precisión
 * arbitraria
 * @author AlgoritmiaCombinatoria Project
 * @date 2024
 * @version 1.0
 *
 * Este archivo define operadores de entrada/salida y funciones de conversión
 * de strings para tipos de Boost.Multiprecision y tipos no estándar como
 * __int128.
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
// NAMESPACE PARA FUNCIONES DE I/O
//==============================================================================

namespace custom_literals_io {

namespace mp = boost::multiprecision;

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

#ifdef BOOST_HAS_GMP
/**
 * @brief Convierte mpz_int a std::string con base especificada
 * @param value Valor a convertir
 * @param base Base numérica (2-36, por defecto 10)
 * @return String representando el valor
 */
std::string to_string(const mp::mpz_int &value, int base = 10) {
  return value.str(base);
}

/**
 * @brief Convierte std::string a mpz_int
 * @param str String a convertir
 * @param base Base numérica (0 para autodetección, 2-36)
 * @return Valor mpz_int
 */
mp::mpz_int from_string_mpz_int(const std::string &str, int base = 10) {
  try {
    return mp::mpz_int(str, base);
  } catch (const std::exception &e) {
    throw std::invalid_argument("Invalid string for mpz_int conversion: " +
                                std::string(e.what()));
  }
}
#endif

#ifdef BOOST_HAS_TOMMATH
/**
 * @brief Convierte tom_int a std::string con base especificada
 * @param value Valor a convertir
 * @param base Base numérica (2-36, por defecto 10)
 * @return String representando el valor
 */
std::string to_string(const mp::tom_int &value, int base = 10) {
  return value.str(base);
}

/**
 * @brief Convierte std::string a tom_int
 * @param str String a convertir
 * @param base Base numérica (0 para autodetección, 2-36)
 * @return Valor tom_int
 */
mp::tom_int from_string_tom_int(const std::string &str, int base = 10) {
  try {
    return mp::tom_int(str, base);
  } catch (const std::exception &e) {
    throw std::invalid_argument("Invalid string for tom_int conversion: " +
                                std::string(e.what()));
  }
}
#endif

} // namespace custom_literals_io

//==============================================================================
// EJEMPLO DE USO
//==============================================================================

#ifdef CUSTOM_LITERALS_IO_EXAMPLE

#include <iostream>
#include <sstream>

int main() {
  using namespace custom_literals_io;

  try {
// Ejemplo con __int128
#if defined(__GNUC__) || defined(__clang__)
    std::cout << "=== Pruebas con __int128 ===" << std::endl;

    __int128 big_number = from_string_i128("123456789012345678901234567890");
    std::cout << "Número desde string: " << big_number << std::endl;

    std::string str_number = to_string(big_number);
    std::cout << "Convertido a string: " << str_number << std::endl;

    // Prueba con stream
    std::stringstream ss;
    ss << big_number;

    __int128 recovered;
    ss >> recovered;
    std::cout << "Leído desde stream: " << recovered << std::endl;

    // Prueba con unsigned
    unsigned __int128 ubig =
        from_string_ui128("340282366920938463463374607431768211455");
    std::cout << "Unsigned máximo: " << ubig << std::endl;
#endif

    // Ejemplo con cpp_int
    std::cout << "\n=== Pruebas con cpp_int ===" << std::endl;

    mp::cpp_int cpp_big = from_string_cpp_int(
        "12345678901234567890123456789012345678901234567890");
    std::cout << "cpp_int desde string: " << to_string(cpp_big) << std::endl;

    // Diferentes bases
    mp::cpp_int hex_num = from_string_cpp_int("DEADBEEF", 16);
    std::cout << "Desde hexadecimal: " << to_string(hex_num) << std::endl;
    std::cout << "En hexadecimal: " << to_string(hex_num, 16) << std::endl;
    std::cout << "En binario: " << to_string(hex_num, 2) << std::endl;

  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}

#endif // CUSTOM_LITERALS_IO_EXAMPLE