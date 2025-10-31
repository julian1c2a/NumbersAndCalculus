/**
 * @file multiprecision_io.hpp
 * @brief I/O unificado para tipos de precisión arbitraria
 * @author AlgoritmiaCombinatoria Project
 * @date 2024
 * @version 1.0
 *
 * Este archivo unifica las funciones de I/O para tanto __int128 como
 * tipos de Boost.Multiprecision, proporcionando una interfaz consistente.
 */

#pragma once

// Incluir ambas librerías de I/O
#include "boost_multiprecision_io.hpp"
#include "int128_io.hpp"


//==============================================================================
// NAMESPACE UNIFICADO PARA I/O DE MULTIPRECISIÓN
//==============================================================================

namespace multiprecision_io {

// Importar todos los namespaces
using namespace int128_io;
using namespace boost_mp_io;

//------------------------------------------------------------------------------
// FUNCIONES TEMPLATE GENÉRICAS PARA CUALQUIER TIPO
//------------------------------------------------------------------------------

/**
 * @brief Convierte cualquier tipo de precisión arbitraria a string
 * @tparam T Tipo de número (puede ser __int128, cpp_int, etc.)
 * @param value Valor a convertir
 * @param base Base numérica (por defecto 10)
 * @return String representando el valor
 *
 * @note Esta función funciona tanto con __int128 como con tipos Boost
 */
template <typename T>
inline std::string universal_to_string(const T &value, int base = 10) {
  // Para tipos que tienen método .str()
  if constexpr (requires { value.str(base); }) {
    return value.str(base);
  }
  // Para __int128 y otros tipos básicos
  else {
    return to_string(value);
  }
}

/**
 * @brief Convierte cualquier tipo a formato hexadecimal
 * @tparam T Tipo de número
 * @param value Valor a convertir
 * @param uppercase Si usar letras mayúsculas
 * @param prefix Si incluir prefijo "0x"
 * @return String en formato hexadecimal
 */
template <typename T>
inline std::string universal_to_hex(const T &value, bool uppercase = true,
                                    bool prefix = true) {
  // Para tipos Boost.Multiprecision
  if constexpr (requires { value.str(16); }) {
    return boost_mp_io::to_hex_string(value, uppercase, prefix);
  }
  // Para __int128 - implementación básica
  else {
    std::stringstream ss;
    if (prefix)
      ss << (uppercase ? "0X" : "0x");
    ss << std::hex;
    if (uppercase)
      ss << std::uppercase;
    ss << value;
    return ss.str();
  }
}

/**
 * @brief Cuenta dígitos de cualquier tipo de número
 * @tparam T Tipo de número
 * @param value Valor a contar
 * @return Número de dígitos
 */
template <typename T> inline size_t universal_count_digits(const T &value) {
  // Para tipos Boost.Multiprecision
  if constexpr (requires { value.str(); }) {
    return boost_mp_io::count_digits(value);
  }
  // Para __int128
  else {
    std::string str = to_string(value < 0 ? -value : value);
    return str.length();
  }
}

/**
 * @brief Formatea cualquier número con separadores de miles
 * @tparam T Tipo de número
 * @param value Valor a formatear
 * @param separator Separador (por defecto ',')
 * @return String formateado
 */
template <typename T>
inline std::string universal_format_thousands(const T &value,
                                              char separator = ',') {
  // Para tipos Boost.Multiprecision
  if constexpr (requires { value.str(); }) {
    return boost_mp_io::format_with_thousands_separator(value, separator);
  }
  // Para __int128 - implementación simple
  else {
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

    for (size_t i = 0; i < len; ++i) {
      if (i > 0 && (len - i) % 3 == 0) {
        result += separator;
      }
      result += str[start + i];
    }

    return result;
  }
}

//------------------------------------------------------------------------------
// FUNCIONES DE COMPARACIÓN DE RENDIMIENTO
//------------------------------------------------------------------------------

/**
 * @brief Compara el rendimiento de diferentes tipos de precisión arbitraria
 */
inline void benchmark_comparison() {
  std::cout << "=== COMPARACIÓN DE RENDIMIENTO TIPOS MULTIPRECISIÓN ==="
            << std::endl;

  // Benchmark básico (solo para demostración)
  auto start = std::chrono::high_resolution_clock::now();

#if defined(__GNUC__) || defined(__clang__)
  // Test con __int128
  __int128 i128_val = 0;
  for (int i = 0; i < 1000; ++i) {
    i128_val += 123456789;
  }
  auto mid = std::chrono::high_resolution_clock::now();

  std::cout << "Resultado __int128: " << to_string(i128_val) << std::endl;
  std::cout << "Tiempo __int128: "
            << std::chrono::duration_cast<std::chrono::microseconds>(mid -
                                                                     start)
                   .count()
            << " microsegundos" << std::endl;
#endif

  // Test con Boost.Multiprecision requeriría compilación con Boost
  std::cout
      << "Nota: Test con Boost.Multiprecision requiere compilación con Boost"
      << std::endl;
}

//------------------------------------------------------------------------------
// INFORMACIÓN DEL SISTEMA
//------------------------------------------------------------------------------

/**
 * @brief Muestra información sobre los tipos disponibles
 */
inline void show_system_info() {
  std::cout << "=== INFORMACIÓN DEL SISTEMA MULTIPRECISIÓN ===" << std::endl;

#if defined(__GNUC__) || defined(__clang__)
  std::cout << "✅ __int128 disponible" << std::endl;
  std::cout << "   Rango: " << std::numeric_limits<__int128>::min() << " a "
            << std::numeric_limits<__int128>::max() << std::endl;
#else
  std::cout << "❌ __int128 no disponible" << std::endl;
#endif

#ifdef BOOST_MULTIPRECISION_CPP_INT_HPP
  std::cout << "✅ Boost.Multiprecision cpp_int disponible" << std::endl;
#else
  std::cout << "❓ Boost.Multiprecision estado desconocido" << std::endl;
#endif

#ifdef BOOST_HAS_GMP
  std::cout << "✅ Backend GMP disponible" << std::endl;
#else
  std::cout << "❌ Backend GMP no disponible" << std::endl;
#endif

#ifdef BOOST_HAS_TOMMATH
  std::cout << "✅ Backend TomMath disponible" << std::endl;
#else
  std::cout << "❌ Backend TomMath no disponible" << std::endl;
#endif
}

} // namespace multiprecision_io

//==============================================================================
// EJEMPLO DE USO UNIFICADO
//==============================================================================

#ifdef MULTIPRECISION_IO_EXAMPLE

#include <chrono>
#include <iostream>


int main() {
  using namespace multiprecision_io;

  try {
    std::cout << "=== DEMOSTRACIÓN I/O UNIFICADO MULTIPRECISIÓN ==="
              << std::endl;

    // Mostrar información del sistema
    show_system_info();

#if defined(__GNUC__) || defined(__clang__)
    std::cout << "\n--- Ejemplo con __int128 ---" << std::endl;

    __int128 big_int128 =
        static_cast<__int128>(123456789012345LL) * 1000000000LL;

    std::cout << "Valor: " << universal_to_string(big_int128) << std::endl;
    std::cout << "Hex: " << universal_to_hex(big_int128) << std::endl;
    std::cout << "Dígitos: " << universal_count_digits(big_int128) << std::endl;
    std::cout << "Con separadores: " << universal_format_thousands(big_int128)
              << std::endl;

    // Ejemplos con diferentes bases
    std::cout << "\nConversiones de formato:" << std::endl;
    std::cout << "  Decimal: " << to_string(big_int128) << std::endl;
    std::cout << "  Hex: " << universal_to_hex(big_int128, true, true)
              << std::endl;
    std::cout << "  Con separadores: "
              << universal_format_thousands(big_int128, '.') << std::endl;

    // Prueba de conversión desde string
    __int128 from_str = from_string_i128("999999999999999999999999999999");
    std::cout << "\nDesde string: " << to_string(from_str) << std::endl;

    // Benchmark simple
    std::cout << "\n--- Benchmark ---" << std::endl;
    benchmark_comparison();
#endif

    std::cout << "\n=== DEMOSTRACIÓN COMPLETADA ===" << std::endl;
    std::cout << "Para funcionalidad completa de Boost.Multiprecision,"
              << std::endl;
    std::cout << "compilar con las librerías Boost instaladas." << std::endl;

  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}

#endif // MULTIPRECISION_IO_EXAMPLE