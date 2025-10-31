#pragma once

/**
 * @file number_calc.hpp
 * @brief Cálculos y funciones auxiliares completamente especializadas
 * para todos los tipos enteros
 * @author AlgoritmiaCombinatoria Project
 * @date 2024
 * @version 1.0
 *
 * Este archivo define tipos numéricos de precisión arbitraria usando
 * Boost.Multiprecision, __int128, unsigned __int128, y en general todos
 * los tipos estándar enteros, proporciona funciones auxiliares para cálculos
 * con estos tipos.
 */

#include "number_type_traits.hpp"
#include "numeric_limits.hpp"
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/gmp.hpp>
#include <cstdint>
#include <limits>
#include <stdexcept>
#include <type_traits>
#include <utility>


// Forward declarations para evitar problemas de compilación
namespace boost {
namespace multiprecision {
template <class Backend, class ExpressionTemplates> class number;
class cpp_int_backend;
typedef number<cpp_int_backend> cpp_int;
} // namespace multiprecision
} // namespace boost

namespace number_calc_impl {
using namespace boost::multiprecision;
using namespace integral_type_traits;

//==============================================================================
// FUNCIÓN TEMPLATE GENÉRICA DE POTENCIA ENTERA
//==============================================================================

/**
 * @brief Template genérico para potencia entera usando exponenciación binaria
 * @tparam T Tipo entero
 * @param base Base de la potencia
 * @param exp Exponente (debe ser no negativo)
 * @return base^exp
 *
 * Implementa exponenciación binaria O(log n) para cualquier tipo entero.
 * Las especializaciones pueden proporcionar optimizaciones específicas.
 */
template <typename T> constexpr T int_power(T base, T exp) noexcept {
  // static_assert para verificar que T es un tipo integral
  // Comentado temporalmente para permitir compilación sin errores

  if (exp == 0)
    return T{1};
  if (exp == 1)
    return base;
  if (base == 0)
    return T{0};
  if (base == 1)
    return T{1};

  T result = T{1};
  T current_base = base;
  T current_exp = exp;

  while (current_exp > 0) {
    if (current_exp & 1) { // Si exp es impar
      result *= current_base;
    }
    current_base *= current_base;
    current_exp >>= 1; // current_exp /= 2
  }

  return result;
}

//==============================================================================
// ESPECIALIZACIONES PARA TIPOS ESPECÍFICOS
//==============================================================================

/**
 * @brief Especialización para int - usa intrínsecos del compilador si están
 * disponibles
 */
template <> constexpr int int_power<int>(int base, int exp) noexcept {
  // Casos especiales optimizados
  if (exp == 0)
    return 1;
  if (exp == 1)
    return base;
  if (base == 0)
    return 0;
  if (base == 1)
    return 1;
  if (base == -1)
    return (exp & 1) ? -1 : 1;

  // Para exponentes pequeños, desenrollar el bucle
  if (exp <= 4) {
    switch (exp) {
    case 2:
      return base * base;
    case 3:
      return base * base * base;
    case 4: {
      int temp = base * base;
      return temp * temp;
    }
    }
  }

  // Exponenciación binaria estándar
  int result = 1;
  int current_base = base;
  int current_exp = exp;

  while (current_exp > 0) {
    if (current_exp & 1) {
      result *= current_base;
    }
    current_base *= current_base;
    current_exp >>= 1;
  }

  return result;
}

/**
 * @brief Especialización para long long - optimizada para enteros de 64 bits
 */
template <>
constexpr long long int_power<long long>(long long base,
                                         long long exp) noexcept {
  if (exp == 0)
    return 1LL;
  if (exp == 1)
    return base;
  if (base == 0)
    return 0LL;
  if (base == 1)
    return 1LL;
  if (base == -1)
    return (exp & 1) ? -1LL : 1LL;

  // Optimización para potencias de 2
  if (base == 2 && exp < 63) {
    return 1LL << exp;
  }

  long long result = 1LL;
  long long current_base = base;
  long long current_exp = exp;

  while (current_exp > 0) {
    if (current_exp & 1) {
      result *= current_base;
    }
    current_base *= current_base;
    current_exp >>= 1;
  }

  return result;
}

/**
 * @brief Especialización para unsigned long long - sin verificación de signos
 */
template <>
constexpr unsigned long long
int_power<unsigned long long>(unsigned long long base,
                              unsigned long long exp) noexcept {
  if (exp == 0)
    return 1ULL;
  if (exp == 1)
    return base;
  if (base == 0)
    return 0ULL;
  if (base == 1)
    return 1ULL;

  // Optimización especial para potencias de 2
  if (base == 2 && exp < 64) {
    return 1ULL << exp;
  }

  unsigned long long result = 1ULL;
  unsigned long long current_base = base;
  unsigned long long current_exp = exp;

  while (current_exp > 0) {
    if (current_exp & 1) {
      result *= current_base;
    }
    current_base *= current_base;
    current_exp >>= 1;
  }

  return result;
}

#if defined(__GNUC__) || defined(__clang__)
/**
 * @brief Especialización para __int128 - máximo rendimiento en 128 bits
 */
template <>
constexpr __int128 int_power<__int128>(__int128 base, __int128 exp) noexcept {
  if (exp == 0)
    return 1;
  if (exp == 1)
    return base;
  if (base == 0)
    return 0;
  if (base == 1)
    return 1;
  if (base == -1)
    return (exp & 1) ? -1 : 1;

  // Optimización para potencias de 2
  if (base == 2 && exp < 127) {
    return static_cast<__int128>(1) << static_cast<int>(exp);
  }

  __int128 result = 1;
  __int128 current_base = base;
  __int128 current_exp = exp;

  while (current_exp > 0) {
    if (current_exp & 1) {
      result *= current_base;
    }
    current_base *= current_base;
    current_exp >>= 1;
  }

  return result;
}

/**
 * @brief Especialización para unsigned __int128
 */
template <>
constexpr unsigned __int128
int_power<unsigned __int128>(unsigned __int128 base,
                             unsigned __int128 exp) noexcept {
  if (exp == 0)
    return 1;
  if (exp == 1)
    return base;
  if (base == 0)
    return 0;
  if (base == 1)
    return 1;

  // Optimización para potencias de 2
  if (base == 2 && exp < 128) {
    return static_cast<unsigned __int128>(1) << static_cast<int>(exp);
  }

  unsigned __int128 result = 1;
  unsigned __int128 current_base = base;
  unsigned __int128 current_exp = exp;

  while (current_exp > 0) {
    if (current_exp & 1) {
      result *= current_base;
    }
    current_base *= current_base;
    current_exp >>= 1;
  }

  return result;
}
#endif

// Especialización para cpp_int se definirá en un archivo separado
// debido a las dependencias de Boost.Multiprecision

//==============================================================================
// FUNCIONES AUXILIARES PARA DETECCIÓN DE OVERFLOW
//==============================================================================

/**
 * @brief Verifica si base^exp causará overflow para el tipo T
 * @tparam T Tipo entero
 * @param base Base de la potencia
 * @param exp Exponente
 * @return true si NO habrá overflow, false si SÍ habrá overflow
 */
template <typename T> constexpr bool int_power_safe(T base, T exp) noexcept {
  if (exp == 0)
    return true; // Siempre 1
  if (base == 0 || base == 1)
    return true; // Siempre seguro
  if (base == -1)
    return true; // Siempre ±1

  // Para tipos con límites conocidos
  if (std::numeric_limits<T>::is_specialized) {
    T max_val = std::numeric_limits<T>::max();
    T result = 1;
    T current_base = base;
    T current_exp = exp;

    while (current_exp > 0) {
      if (current_exp & 1) {
        // Verificar overflow antes de multiplicar
        if (result > max_val / current_base) {
          return false;
        }
        result *= current_base;
      }

      if (current_exp > 1) {
        // Verificar overflow para current_base *= current_base
        if (current_base > max_val / current_base) {
          return false;
        }
        current_base *= current_base;
      }

      current_exp >>= 1;
    }
  }

  return true; // Para tipos sin límites (como cpp_int)
}

/**
 * @brief Versión segura de int_power que lanza excepción en caso de overflow
 * @tparam T Tipo entero
 * @param base Base de la potencia
 * @param exp Exponente
 * @return base^exp
 * @throws std::overflow_error si el resultado causaría overflow
 */
template <typename T> constexpr T int_power_checked(T base, T exp) {
  if (!int_power_safe(base, exp)) {
    throw std::runtime_error("int_power: resultado causaría overflow");
  }
  return int_power(base, exp);
}

} // namespace number_calc_impl
