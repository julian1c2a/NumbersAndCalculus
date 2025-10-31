/**
 * @file boost_multiprecision_io.hpp
 * @brief Operadores de I/O y conversión de strings para tipos
 * Boost.Multiprecision
 * @author AlgoritmiaCombinatoria Project
 * @date 2024
 * @version 1.0
 *
 * Este archivo define operadores de entrada/salida y funciones de conversión
 * de strings para tipos de Boost.Multiprecision como cpp_int, mpz_int, tom_int.
 */

#pragma once

#include <boost/multiprecision/cpp_bin_float.hpp>
#include <boost/multiprecision/cpp_dec_float.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <cstring>
#include <iomanip>
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
// NAMESPACE PARA FUNCIONES DE I/O DE BOOST.MULTIPRECISION
//==============================================================================

namespace mp {

using namespace boost::multiprecision;

//------------------------------------------------------------------------------
// OPERADORES DE SALIDA (operator<<) - Ya están implementados en Boost
//------------------------------------------------------------------------------

// Nota: Boost.Multiprecision ya proporciona operadores << nativos
// Aquí agregamos funcionalidad adicional y wrappers convenientes

//------------------------------------------------------------------------------
// OPERADORES DE ENTRADA (operator>>) - Ya están implementados en Boost
//------------------------------------------------------------------------------

// Nota: Boost.Multiprecision ya proporciona operadores >> nativos
// Aquí agregamos funcionalidad adicional con manejo de errores mejorado

/**
 * @brief Operador de entrada mejorado para cpp_int con manejo de errores
 * @param is Stream de entrada
 * @param value Referencia donde almacenar el valor leído
 * @return Referencia al stream
 *
 * @note Wrapper que mejora el manejo de errores del operador nativo
 */
inline std::istream &safe_input(std::istream &is, cpp_int &value) {
  std::string str;
  is >> str;

  if (str.empty()) {
    is.setstate(std::ios::failbit);
    return is;
  }

  try {
    value = cpp_int(str);
  } catch (const std::exception &) {
    is.setstate(std::ios::failbit);
  }

  return is;
}

#ifdef BOOST_HAS_GMP
/**
 * @brief Operador de entrada mejorado para mpz_int
 */
inline std::istream &safe_input(std::istream &is, mpz_int &value) {
  std::string str;
  is >> str;

  if (str.empty()) {
    is.setstate(std::ios::failbit);
    return is;
  }

  try {
    value = mpz_int(str);
  } catch (const std::exception &) {
    is.setstate(std::ios::failbit);
  }

  return is;
}
#endif

#ifdef BOOST_HAS_TOMMATH
/**
 * @brief Operador de entrada mejorado para tom_int
 */
inline std::istream &safe_input(std::istream &is, tom_int &value) {
  std::string str;
  is >> str;

  if (str.empty()) {
    is.setstate(std::ios::failbit);
    return is;
  }

  try {
    value = tom_int(str);
  } catch (const std::exception &) {
    is.setstate(std::ios::failbit);
  }

  return is;
}
#endif

//------------------------------------------------------------------------------
// FUNCIONES DE CONVERSIÓN A STRING (con bases personalizadas)
//------------------------------------------------------------------------------

/**
 * @brief Convierte cpp_int a std::string con base especificada
 * @param value Valor a convertir
 * @param base Base numérica (2-36, por defecto 10)
 * @param uppercase Si usar letras mayúsculas para bases >10
 * @return String representando el valor
 */
inline std::string to_string(const cpp_int &value, int base = 10,
                             bool uppercase = false) {
  return value.str(
      base, std::ios_base::fmtflags(uppercase ? std::ios_base::uppercase : 0));
}

/**
 * @brief Convierte cpp_int a std::string en formato hexadecimal
 * @param value Valor a convertir
 * @param uppercase Si usar letras mayúsculas
 * @param prefix Si incluir prefijo "0x"
 * @return String en formato hexadecimal
 */
inline std::string to_hex_string(const cpp_int &value, bool uppercase = true,
                                 bool prefix = true) {
  std::string result = value.str(
      16, std::ios_base::fmtflags(uppercase ? std::ios_base::uppercase : 0));
  if (prefix && value >= 0) {
    result = (uppercase ? "0X" : "0x") + result;
  }
  return result;
}

/**
 * @brief Convierte cpp_int a std::string en formato binario
 * @param value Valor a convertir
 * @param prefix Si incluir prefijo "0b"
 * @return String en formato binario
 */
inline std::string to_binary_string(const cpp_int &value, bool prefix = true) {
  if (value == 0) {
    return prefix ? "0b0" : "0";
  }

  std::string result = value.str(2);
  if (prefix && value >= 0) {
    result = "0b" + result;
  }
  return result;
}

/**
 * @brief Convierte cpp_int a std::string en formato octal
 * @param value Valor a convertir
 * @param prefix Si incluir prefijo "0"
 * @return String en formato octal
 */
inline std::string to_octal_string(const cpp_int &value, bool prefix = true) {
  if (value == 0) {
    return "0";
  }

  std::string result = value.str(8);
  if (prefix && value > 0) {
    result = "0" + result;
  }
  return result;
}

#ifdef BOOST_HAS_GMP
/**
 * @brief Convierte mpz_int a std::string con base especificada
 */
inline std::string to_string(const mpz_int &value, int base = 10,
                             bool uppercase = false) {
  return value.str(
      base, std::ios_base::fmtflags(uppercase ? std::ios_base::uppercase : 0));
}

/**
 * @brief Convierte mpz_int a formato hexadecimal
 */
inline std::string to_hex_string(const mpz_int &value, bool uppercase = true,
                                 bool prefix = true) {
  std::string result = value.str(
      16, std::ios_base::fmtflags(uppercase ? std::ios_base::uppercase : 0));
  if (prefix && value >= 0) {
    result = (uppercase ? "0X" : "0x") + result;
  }
  return result;
}
#endif

#ifdef BOOST_HAS_TOMMATH
/**
 * @brief Convierte tom_int a std::string con base especificada
 */
inline std::string to_string(const tom_int &value, int base = 10,
                             bool uppercase = false) {
  return value.str(
      base, std::ios_base::fmtflags(uppercase ? std::ios_base::uppercase : 0));
}
#endif

//------------------------------------------------------------------------------
// FUNCIONES DE CONVERSIÓN DESDE STRING (con validación mejorada)
//------------------------------------------------------------------------------

/**
 * @brief Convierte std::string a cpp_int con validación robusta
 * @param str String a convertir
 * @param base Base numérica (0 para autodetección, 2-36)
 * @return Valor cpp_int
 *
 * @throws std::invalid_argument Si la string no es un número válido en la base
 * especificada
 * @throws std::runtime_error Si hay otros errores de conversión
 */
inline cpp_int from_string_cpp_int(const std::string &str, int base = 10) {
  if (str.empty()) {
    throw std::invalid_argument("Empty string cannot be converted to cpp_int");
  }

  try {
    return cpp_int(str, base);
  } catch (const std::invalid_argument &e) {
    throw std::invalid_argument("Invalid string for cpp_int conversion: " +
                                std::string(e.what()));
  } catch (const std::exception &e) {
    throw std::runtime_error("Error converting string to cpp_int: " +
                             std::string(e.what()));
  }
}

/**
 * @brief Convierte string hexadecimal a cpp_int
 * @param hex_str String hexadecimal (con o sin prefijo 0x/0X)
 * @return Valor cpp_int
 */
inline cpp_int from_hex_string_cpp_int(const std::string &hex_str) {
  if (hex_str.empty()) {
    throw std::invalid_argument(
        "Empty hex string cannot be converted to cpp_int");
  }

  std::string clean_str = hex_str;

  // Remover prefijos 0x, 0X
  if (clean_str.length() >= 2) {
    if ((clean_str[0] == '0' && (clean_str[1] == 'x' || clean_str[1] == 'X'))) {
      clean_str = clean_str.substr(2);
    }
  }

  if (clean_str.empty()) {
    throw std::invalid_argument("Invalid hex string format");
  }

  return from_string_cpp_int(clean_str, 16);
}

/**
 * @brief Convierte string binario a cpp_int
 * @param bin_str String binario (con o sin prefijo 0b)
 * @return Valor cpp_int
 */
inline cpp_int from_binary_string_cpp_int(const std::string &bin_str) {
  if (bin_str.empty()) {
    throw std::invalid_argument(
        "Empty binary string cannot be converted to cpp_int");
  }

  std::string clean_str = bin_str;

  // Remover prefijo 0b
  if (clean_str.length() >= 2 && clean_str[0] == '0' && clean_str[1] == 'b') {
    clean_str = clean_str.substr(2);
  }

  if (clean_str.empty()) {
    throw std::invalid_argument("Invalid binary string format");
  }

  return from_string_cpp_int(clean_str, 2);
}

/**
 * @brief Convierte string octal a cpp_int
 * @param oct_str String octal (con o sin prefijo 0)
 * @return Valor cpp_int
 */
inline cpp_int from_octal_string_cpp_int(const std::string &oct_str) {
  if (oct_str.empty()) {
    throw std::invalid_argument(
        "Empty octal string cannot be converted to cpp_int");
  }

  return from_string_cpp_int(oct_str, 8);
}

/**
 * @brief Convierte C-string a cpp_int
 * @param cstr C-string a convertir
 * @param base Base numérica (0 para autodetección, 2-36)
 * @return Valor cpp_int
 *
 * @throws std::invalid_argument Si cstr es nullptr o no es un número válido
 */
inline cpp_int from_cstr_cpp_int(const char *cstr, int base = 10) {
  if (cstr == nullptr) {
    throw std::invalid_argument("Null pointer cannot be converted to cpp_int");
  }
  return from_string_cpp_int(std::string(cstr), base);
}

#ifdef BOOST_HAS_GMP
/**
 * @brief Convierte std::string a mpz_int con validación
 */
inline mpz_int from_string_mpz_int(const std::string &str, int base = 10) {
  if (str.empty()) {
    throw std::invalid_argument("Empty string cannot be converted to mpz_int");
  }

  try {
    return mpz_int(str, base);
  } catch (const std::invalid_argument &e) {
    throw std::invalid_argument("Invalid string for mpz_int conversion: " +
                                std::string(e.what()));
  } catch (const std::exception &e) {
    throw std::runtime_error("Error converting string to mpz_int: " +
                             std::string(e.what()));
  }
}

/**
 * @brief Convierte C-string a mpz_int
 */
inline mpz_int from_cstr_mpz_int(const char *cstr, int base = 10) {
  if (cstr == nullptr) {
    throw std::invalid_argument("Null pointer cannot be converted to mpz_int");
  }
  return from_string_mpz_int(std::string(cstr), base);
}
#endif

#ifdef BOOST_HAS_TOMMATH
/**
 * @brief Convierte std::string a tom_int con validación
 */
inline tom_int from_string_tom_int(const std::string &str, int base = 10) {
  if (str.empty()) {
    throw std::invalid_argument("Empty string cannot be converted to tom_int");
  }

  try {
    return tom_int(str, base);
  } catch (const std::invalid_argument &e) {
    throw std::invalid_argument("Invalid string for tom_int conversion: " +
                                std::string(e.what()));
  } catch (const std::exception &e) {
    throw std::runtime_error("Error converting string to tom_int: " +
                             std::string(e.what()));
  }
}

/**
 * @brief Convierte C-string a tom_int
 */
inline tom_int from_cstr_tom_int(const char *cstr, int base = 10) {
  if (cstr == nullptr) {
    throw std::invalid_argument("Null pointer cannot be converted to tom_int");
  }
  return from_string_tom_int(std::string(cstr), base);
}
#endif

//------------------------------------------------------------------------------
// FUNCIONES DE CONVERSIÓN A C-STRING (deprecated, usar to_string)
//------------------------------------------------------------------------------

/**
 * @brief Convierte cpp_int a C-string (char*)
 * @param value Valor a convertir
 * @param base Base numérica (2-36, por defecto 10)
 * @return Puntero a C-string (debe ser liberado con delete[])
 *
 * @warning El puntero retornado debe ser liberado con delete[]
 * @deprecated Considerar usar to_string() en su lugar
 */
[[deprecated("Use to_string() instead")]]
inline char *to_cstr(const cpp_int &value, int base = 10) {
  std::string str = to_string(value, base);
  char *result = new char[str.length() + 1];
  std::strcpy(result, str.c_str());
  return result;
}

//------------------------------------------------------------------------------
// FUNCIONES AUXILIARES PARA FORMATTING AVANZADO
//------------------------------------------------------------------------------

/**
 * @brief Formatea cpp_int con separadores de miles
 * @param value Valor a formatear
 * @param separator Separador de miles (por defecto ',')
 * @return String formateado con separadores
 */
inline std::string format_with_thousands_separator(const cpp_int &value,
                                                   char separator = ',') {
  std::string str = to_string(value);

  if (str.length() <= 3) {
    return str;
  }

  std::string result;
  bool negative = (str[0] == '-');
  size_t start = negative ? 1 : 0;
  size_t len = str.length() - start;

  if (negative) {
    result = "-";
  }

  // Agregar separadores de derecha a izquierda
  for (size_t i = 0; i < len; ++i) {
    if (i > 0 && (len - i) % 3 == 0) {
      result += separator;
    }
    result += str[start + i];
  }

  return result;
}

/**
 * @brief Formatea cpp_int en notación científica aproximada
 * @param value Valor a formatear
 * @param precision Dígitos de precision (por defecto 6)
 * @return String en notación científica
 */
inline std::string format_scientific(const cpp_int &value, int precision = 6) {
  if (value == 0) {
    return "0.000000e+00";
  }

  std::string str = to_string(abs(value));

  if (str.length() <= precision) {
    return to_string(value) + ".000000e+00";
  }

  // Aproximación simple de notación científica
  std::string mantissa = str.substr(0, 1) + "." + str.substr(1, precision - 1);
  int exponent = static_cast<int>(str.length() - 1);

  std::ostringstream oss;
  if (value < 0)
    oss << "-";
  oss << mantissa << "e+" << std::setfill('0') << std::setw(2) << exponent;

  return oss.str();
}

/**
 * @brief Cuenta el número de dígitos en un cpp_int
 * @param value Valor a contar
 * @return Número de dígitos (sin contar el signo)
 */
inline size_t count_digits(const cpp_int &value) {
  if (value == 0)
    return 1;

  std::string str = to_string(abs(value));
  return str.length();
}

/**
 * @brief Verifica si un cpp_int es una potencia de 2
 * @param value Valor a verificar
 * @return true si es potencia de 2, false en caso contrario
 */
inline bool is_power_of_two(const cpp_int &value) {
  if (value <= 0)
    return false;
  return (value & (value - 1)) == 0;
}

/**
 * @brief Calcula el logaritmo base 2 aproximado de un cpp_int
 * @param value Valor positivo
 * @return Logaritmo base 2 aproximado
 */
inline int log2_approx(const cpp_int &value) {
  if (value <= 0) {
    throw std::invalid_argument("log2 requires positive value");
  }

  if (value == 1)
    return 0;

  // Aproximación basada en número de dígitos binarios
  std::string binary = to_binary_string(value, false);
  return static_cast<int>(binary.length() - 1);
}

//------------------------------------------------------------------------------
// SOBRECARGAS PARA TIPOS DE PRECISIÓN FIJA
//------------------------------------------------------------------------------

// Tipos de enteros de precisión fija comunes
using int128_t =
    number<cpp_int_backend<128, 128, signed_magnitude, unchecked, void>>;
using uint128_t =
    number<cpp_int_backend<128, 128, unsigned_magnitude, unchecked, void>>;
using int256_t =
    number<cpp_int_backend<256, 256, signed_magnitude, unchecked, void>>;
using uint256_t =
    number<cpp_int_backend<256, 256, unsigned_magnitude, unchecked, void>>;
using int512_t =
    number<cpp_int_backend<512, 512, signed_magnitude, unchecked, void>>;
using uint512_t =
    number<cpp_int_backend<512, 512, unsigned_magnitude, unchecked, void>>;
using int1024_t =
    number<cpp_int_backend<1024, 1024, signed_magnitude, unchecked, void>>;
using uint1024_t =
    number<cpp_int_backend<1024, 1024, unsigned_magnitude, unchecked, void>>;

// Tipos de punto flotante de precisión fija
using cpp_dec_float_50 = cpp_dec_float<50>;
using cpp_dec_float_100 = cpp_dec_float<100>;
using cpp_bin_float_quad = cpp_bin_float_quad;
using cpp_bin_float_oct = cpp_bin_float_oct;

/**
 * @brief Template genérico para conversión a string de tipos de precisión fija
 */
template <typename Backend>
inline std::string to_string(const number<Backend> &value, int base = 10,
                             bool uppercase = false) {
  return value.str(
      base, std::ios_base::fmtflags(uppercase ? std::ios_base::uppercase : 0));
}

/**
 * @brief Template genérico para conversión hexadecimal de tipos de precisión
 * fija
 */
template <typename Backend>
inline std::string to_hex_string(const number<Backend> &value,
                                 bool uppercase = true, bool prefix = true) {
  std::string result = value.str(
      16, std::ios_base::fmtflags(uppercase ? std::ios_base::uppercase : 0));
  if (prefix) {
    result = (uppercase ? "0X" : "0x") + result;
  }
  return result;
}

/**
 * @brief Template genérico para conteo de dígitos de tipos de precisión fija
 */
template <typename Backend>
inline size_t count_digits(const number<Backend> &value) {
  std::string str = value.str();
  if (!str.empty() && str[0] == '-') {
    return str.length() - 1; // No contar el signo menos
  }
  return str.length();
}

/**
 * @brief Template genérico para formateo con separadores de miles
 */
template <typename Backend>
inline std::string format_with_thousands_separator(const number<Backend> &value,
                                                   char separator = ',') {
  std::string str = value.str();

  if (str.length() <= 3) {
    return str;
  }

  std::string result;
  bool negative = (!str.empty() && str[0] == '-');
  size_t start = negative ? 1 : 0;
  size_t len = str.length() - start;

  if (negative) {
    result = "-";
  }

  for (size_t i = 0; i < len; ++i) {
    if (i > 0 && (len - i) % 3 == 0) {
      result += separator;
    }
    result += str[start + i];
  }

  return result;
}

/**
 * @brief Template genérico para entrada segura de tipos de precisión fija
 */
template <typename Backend>
inline std::istream &safe_input(std::istream &is, number<Backend> &value) {
  std::string str;
  is >> str;

  if (str.empty()) {
    is.setstate(std::ios::failbit);
    return is;
  }

  try {
    value = number<Backend>(str);
  } catch (const std::exception &) {
    is.setstate(std::ios::failbit);
  }

  return is;
}

//------------------------------------------------------------------------------
// FUNCIONES ESPECÍFICAS PARA TIPOS COMUNES DE PRECISIÓN FIJA
//------------------------------------------------------------------------------

// Funciones específicas para int128_t
inline std::string to_string(const int128_t &value) { return value.str(); }
inline std::string to_hex_string(const int128_t &value, bool uppercase = true,
                                 bool prefix = true) {
  return to_hex_string<
      cpp_int_backend<128, 128, signed_magnitude, unchecked, void>>(
      value, uppercase, prefix);
}
inline size_t count_digits(const int128_t &value) {
  return count_digits<
      cpp_int_backend<128, 128, signed_magnitude, unchecked, void>>(value);
}

// Funciones específicas para uint128_t
inline std::string to_string(const uint128_t &value) { return value.str(); }
inline std::string to_hex_string(const uint128_t &value, bool uppercase = true,
                                 bool prefix = true) {
  return to_hex_string<
      cpp_int_backend<128, 128, unsigned_magnitude, unchecked, void>>(
      value, uppercase, prefix);
}
inline size_t count_digits(const uint128_t &value) {
  return count_digits<
      cpp_int_backend<128, 128, unsigned_magnitude, unchecked, void>>(value);
}

// Funciones específicas para int256_t
inline std::string to_string(const int256_t &value) { return value.str(); }
inline std::string to_hex_string(const int256_t &value, bool uppercase = true,
                                 bool prefix = true) {
  return to_hex_string<
      cpp_int_backend<256, 256, signed_magnitude, unchecked, void>>(
      value, uppercase, prefix);
}

// Funciones específicas para tipos de punto flotante
inline std::string to_string(const cpp_dec_float_50 &value,
                             int precision = -1) {
  if (precision >= 0) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(precision) << value;
    return ss.str();
  }
  return value.str();
}

inline std::string to_string(const cpp_dec_float_100 &value,
                             int precision = -1) {
  if (precision >= 0) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(precision) << value;
    return ss.str();
  }
  return value.str();
}

} // namespace mp

//==============================================================================
// EJEMPLO DE USO
//==============================================================================

#ifdef BOOST_MP_IO_EXAMPLE

#include <iomanip>
#include <iostream>

int main() {
  using namespace boost_mp_io;

  try {
    std::cout << "=== DEMOSTRACIÓN I/O BOOST.MULTIPRECISION ===" << std::endl;

    // Ejemplo 1: Conversiones básicas
    std::cout << "\n--- Conversiones básicas ---" << std::endl;

    auto big1 = from_string_cpp_int("123456789012345678901234567890123456789");
    std::cout << "Número gigantesco: " << to_string(big1) << std::endl;
    std::cout << "Dígitos: " << count_digits(big1) << std::endl;

    // Ejemplo 2: Diferentes bases
    std::cout << "\n--- Conversiones en diferentes bases ---" << std::endl;

    auto hex_num =
        from_hex_string_cpp_int("0xDEADBEEFCAFEBABE123456789ABCDEF0");
    std::cout << "Desde hex: " << to_string(hex_num) << std::endl;
    std::cout << "A hex: " << to_hex_string(hex_num) << std::endl;
    std::cout << "A binario: " << to_binary_string(hex_num) << std::endl;
    std::cout << "A octal: " << to_octal_string(hex_num) << std::endl;

    // Ejemplo 3: Formateo avanzado
    std::cout << "\n--- Formateo avanzado ---" << std::endl;

    auto large_num = from_string_cpp_int("1234567890123456789");
    std::cout << "Con separadores: "
              << format_with_thousands_separator(large_num) << std::endl;
    std::cout << "Notación científica: " << format_scientific(large_num)
              << std::endl;

    // Ejemplo 4: Operaciones matemáticas
    std::cout << "\n--- Operaciones matemáticas ---" << std::endl;

    cpp_int power_of_2 = 1024;
    std::cout << "¿Es " << power_of_2 << " potencia de 2? "
              << (is_power_of_two(power_of_2) ? "Sí" : "No") << std::endl;
    std::cout << "log2(" << power_of_2 << ") ≈ " << log2_approx(power_of_2)
              << std::endl;

    // Ejemplo 5: Conversiones desde binario
    std::cout << "\n--- Conversiones desde binario ---" << std::endl;

    auto from_bin = from_binary_string_cpp_int(
        "0b1111000011110000111100001111000011110000");
    std::cout << "Desde binario: " << to_string(from_bin) << std::endl;
    std::cout << "A hex: " << to_hex_string(from_bin) << std::endl;

    // Ejemplo 6: Entrada segura (simulada)
    std::cout << "\n--- Entrada segura ---" << std::endl;

    std::stringstream test_stream("999999999999999999999999999999999999999");
    cpp_int input_value;
    safe_input(test_stream, input_value);
    std::cout << "Valor leído: " << to_string(input_value) << std::endl;

    std::cout << "\n=== TODAS LAS OPERACIONES COMPLETADAS ===" << std::endl;

  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}

#endif // BOOST_MP_IO_EXAMPLE
