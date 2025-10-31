/**
 * @file multiprecision_io_compatible.hpp
 * @brief I/O unificado compatible con C++14/17 para tipos de precisión
 * arbitraria
 * @author AlgoritmiaCombinatoria Project
 * @date 2024
 * @version 1.0
 *
 * Versión compatible que funciona tanto con como sin Boost.Multiprecision
 * y utiliza características de C++14/17 estándar.
 */

#pragma once

#include <algorithm>
#include <cctype>
#include <iostream>
#include <sstream>
#include <string>
#include <type_traits>

// Solo incluir int128_io.hpp que sabemos que funciona
#include "int128_io.hpp"

// Detectar si Boost.Multiprecision está disponible
#ifdef BOOST_MULTIPRECISION_CPP_INT_HPP
#include "boost_multiprecision_io.hpp"
#define HAS_BOOST_MULTIPRECISION 1
#else
#define HAS_BOOST_MULTIPRECISION 0
#endif

//==============================================================================
// NAMESPACE UNIFICADO COMPATIBLE
//==============================================================================

namespace multiprecision_io_compat {

// Importar namespace de int128
using namespace int128_io;

#if HAS_BOOST_MULTIPRECISION
using namespace mp; // Cambio de boost_mp_io a mp
#endif //------------------------------------------------------------------------------
// FUNCIONES GENÉRICAS CON SFINAE (C++14 compatible)
//------------------------------------------------------------------------------

/**
 * @brief Estructura auxiliar para detectar si un tipo tiene método str()
 */
template <typename T> struct has_str_method {
private:
  template <typename U>
  static auto test(int) -> decltype(std::declval<U>().str(), std::true_type{});

  template <typename> static std::false_type test(...);

public:
  static constexpr bool value = decltype(test<T>(0))::value;
};

/**
 * @brief Estructura auxiliar para detectar si un tipo tiene método str(int)
 */
template <typename T> struct has_str_base_method {
private:
  template <typename U>
  static auto test(int)
      -> decltype(std::declval<U>().str(10), std::true_type{});

  template <typename> static std::false_type test(...);

public:
  static constexpr bool value = decltype(test<T>(0))::value;
};

/**
 * @brief Convierte cualquier tipo a string (versión para tipos con método str)
 */
template <typename T>
typename std::enable_if<has_str_method<T>::value, std::string>::type
universal_to_string_impl(const T &value, int /*base*/) {
  return value.str();
}

/**
 * @brief Convierte cualquier tipo a string (versión para tipos sin método str)
 */
template <typename T>
typename std::enable_if<!has_str_method<T>::value, std::string>::type
universal_to_string_impl(const T &value, int /*base*/) {
  return to_string(value);
}

/**
 * @brief Convierte cualquier tipo de precisión arbitraria a string
 */
template <typename T>
std::string universal_to_string(const T &value, int base = 10) {
  return universal_to_string_impl(value, base);
}

/**
 * @brief Versión para hex con tipos que tienen str(base)
 */
template <typename T>
typename std::enable_if<has_str_base_method<T>::value, std::string>::type
universal_to_hex_impl(const T &value, bool uppercase, bool prefix) {
  std::string result = value.str(16);

  if (uppercase) {
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
  }

  if (prefix) {
    result = (uppercase ? "0X" : "0x") + result;
  }

  return result;
}

/**
 * @brief Versión para hex con tipos básicos
 */
template <typename T>
typename std::enable_if<!has_str_base_method<T>::value, std::string>::type
universal_to_hex_impl(const T &value, bool uppercase, bool prefix) {
  std::stringstream ss;
  if (prefix) {
    ss << (uppercase ? "0X" : "0x");
  }
  ss << std::hex;
  if (uppercase) {
    ss << std::uppercase;
  }
  ss << value;
  return ss.str();
}

/**
 * @brief Convierte cualquier tipo a formato hexadecimal
 */
template <typename T>
std::string universal_to_hex(const T &value, bool uppercase = true,
                             bool prefix = true) {
  return universal_to_hex_impl(value, uppercase, prefix);
}

/**
 * @brief Cuenta dígitos para tipos con método str()
 */
template <typename T>
typename std::enable_if<has_str_method<T>::value, size_t>::type
universal_count_digits_impl(const T &value) {
  std::string str = value.str();
  if (!str.empty() && str[0] == '-') {
    return str.length() - 1; // No contar el signo menos
  }
  return str.length();
}

/**
 * @brief Cuenta dígitos para tipos básicos
 */
template <typename T>
typename std::enable_if<!has_str_method<T>::value, size_t>::type
universal_count_digits_impl(const T &value) {
  std::string str = to_string(value < 0 ? -value : value);
  return str.length();
}

/**
 * @brief Cuenta dígitos de cualquier tipo de número
 */
template <typename T> size_t universal_count_digits(const T &value) {
  return universal_count_digits_impl(value);
}

/**
 * @brief Formatea cualquier número con separadores de miles
 */
template <typename T>
std::string universal_format_thousands(const T &value, char separator = ',') {
  std::string str = universal_to_string(value);

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

//------------------------------------------------------------------------------
// INFORMACIÓN DEL SISTEMA
//------------------------------------------------------------------------------

/**
 * @brief Muestra información sobre los tipos disponibles
 */
inline void show_system_info() {
  std::cout << "=== INFORMACIÓN DEL SISTEMA MULTIPRECISIÓN (COMPATIBLE) ==="
            << std::endl;

#if defined(__GNUC__) || defined(__clang__)
  std::cout << "✅ __int128 disponible" << std::endl;
#else
  std::cout << "❌ __int128 no disponible" << std::endl;
#endif

#if HAS_BOOST_MULTIPRECISION
  std::cout << "✅ Boost.Multiprecision disponible" << std::endl;
#else
  std::cout << "❌ Boost.Multiprecision no disponible" << std::endl;
#endif

  std::cout << "Estándar C++: ";
#if __cplusplus >= 202002L
  std::cout << "C++20 o superior" << std::endl;
#elif __cplusplus >= 201703L
  std::cout << "C++17" << std::endl;
#elif __cplusplus >= 201402L
  std::cout << "C++14" << std::endl;
#else
  std::cout << "C++11 o anterior" << std::endl;
#endif
}

/**
 * @brief Función de demostración simple
 */
inline void demo_basic() {
  std::cout << "\n=== DEMOSTRACIÓN BÁSICA ===" << std::endl;

#if defined(__GNUC__) || defined(__clang__)
  // Ejemplo con __int128
  __int128 big_val = static_cast<__int128>(123456789012345LL) * 1000000LL;

  std::cout << "Valor __int128: " << universal_to_string(big_val) << std::endl;
  std::cout << "En hexadecimal: " << universal_to_hex(big_val) << std::endl;
  std::cout << "Número de dígitos: " << universal_count_digits(big_val)
            << std::endl;
  std::cout << "Con separadores: " << universal_format_thousands(big_val, '.')
            << std::endl;

  // Prueba con diferentes valores
  __int128 negative_val = -big_val;
  std::cout << "\nValor negativo: " << universal_to_string(negative_val)
            << std::endl;
  std::cout << "Formateado: " << universal_format_thousands(negative_val, '\'')
            << std::endl;
#endif

#if HAS_BOOST_MULTIPRECISION
  // Este código solo se compila si Boost está disponible
  std::cout << "\n--- Ejemplo con Boost.Multiprecision ---" << std::endl;
// Aquí iría código específico de Boost
#endif
}

} // namespace multiprecision_io_compat

//==============================================================================
// EJEMPLO DE USO COMPATIBLE
//==============================================================================

#ifdef MULTIPRECISION_IO_COMPATIBLE_EXAMPLE

int main() {
  using namespace multiprecision_io_compat;

  try {
    std::cout << "=== DEMOSTRACIÓN I/O UNIFICADO COMPATIBLE ===" << std::endl;

    // Mostrar información del sistema
    show_system_info();

    // Demostración básica
    demo_basic();

    std::cout << "\n=== DEMOSTRACIÓN COMPLETADA ===" << std::endl;

  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}

#endif // MULTIPRECISION_IO_COMPATIBLE_EXAMPLE