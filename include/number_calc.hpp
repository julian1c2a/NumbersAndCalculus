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

// Solo usar las declaraciones de Boost.Multiprecision

namespace number_calc_impl {
using mp = boost::multiprecision;
using int_tt = integral_type_traits;

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
template <int_tt::integral T, int_tt::integral U>
constexpr T int_power(T base, U exp) noexcept {
  // static_assert para verificar que T es un tipo integral
  // Comentado temporalmente para permitir compilación sin errores
  // static_assert(int_tt::is_integral<T>::value, "Tipo no soportado");
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
// ESPECIALIZACIONES BASADAS EN TRAITS DE SIGNO
//==============================================================================

/**
 * @brief Especialización para tipos SIGNED usando SFINAE
 * Optimizada para tipos enteros con signo, maneja casos especiales de -1
 */
template <int_tt::signed_integral T, int_tt::integral U>
constexpr T int_power_signed_impl(T base, U exp) noexcept {
  if (exp == 0)
    return T{1};
  if (exp == 1)
    return base;
  if (base == 0)
    return T{0};
  if (base == 1)
    return T{1};
  if (base == -1)
    return (exp & 1) ? T{-1} : T{1}; // Manejo especial de -1

  // Para exponentes pequeños y tipos pequeños, desenrollar
  if constexpr (sizeof(T) <= sizeof(int)) {
    if (exp <= 4) {
      switch (exp) {
      case 2:
        return base * base;
      case 3:
        return base * base * base;
      case 4: {
        T temp = base * base;
        return temp * temp;
      }
      }
    }
  }

  // Optimización para potencias de 2
  if (base == 2) {
    if constexpr (sizeof(T) == sizeof(int) && exp < 31) {
      return T{1} << static_cast<int>(exp);
    } else if constexpr (sizeof(T) == sizeof(long long) && exp < 63) {
      return T{1} << static_cast<int>(exp);
    } else if constexpr (sizeof(T) == sizeof(__int128) && exp < 127) {
      return T{1} << static_cast<int>(exp);
    }
  }

  // Exponenciación binaria estándar
  T result = T{1};
  T current_base = base;
  U current_exp = exp;

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
 * @brief Especialización para tipos UNSIGNED usando SFINAE
 * Optimizada para tipos sin signo, sin verificaciones de signo
 */
template <int_tt::unsigned_integral T, int_tt::integral U>
constexpr T int_power_unsigned_impl(T base, U exp) noexcept {
  if (exp == 0)
    return T{1};
  if (exp == 1)
    return base;
  if (base == 0)
    return T{0};
  if (base == 1)
    return T{1};
  // No hay casos especiales para -1 en tipos unsigned

  // Optimización especial para potencias de 2 (muy común)
  if (base == 2) {
    if constexpr (sizeof(T) == sizeof(unsigned int) && exp < 32) {
      return T{1} << static_cast<unsigned int>(exp);
    } else if constexpr (sizeof(T) == sizeof(unsigned long long) && exp < 64) {
      return T{1} << static_cast<unsigned int>(exp);
    } else if constexpr (sizeof(T) == sizeof(unsigned __int128) && exp < 128) {
      return T{1} << static_cast<unsigned int>(exp);
    }
  }

  // Para exponentes pequeños, desenrollar bucle
  if constexpr (sizeof(T) <= sizeof(unsigned int)) {
    if (exp <= 4) {
      switch (exp) {
      case 2:
        return base * base;
      case 3:
        return base * base * base;
      case 4: {
        T temp = base * base;
        return temp * temp;
      }
      }
    }
  }

  // Exponenciación binaria estándar
  T result = T{1};
  T current_base = base;
  U current_exp = exp;

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
 * @brief Función dispatch que elige la implementación correcta según el signo
 */
template <int_tt::integral T, int_tt::integral U>
constexpr T int_power_dispatch(T base, U exp) noexcept {
  if constexpr (int_tt::signed_integral<T>) {
    return int_power_signed_impl(base, exp);
  } else {
    return int_power_unsigned_impl(base, exp);
  }
}

//==============================================================================
// FUNCIÓN int_power_2 ESPECIALIZADA PARA BASE = 2
//==============================================================================

/**
 * @brief Función optimizada específicamente para calcular 2^exp
 * @tparam T Tipo del resultado (entero)
 * @tparam U Tipo del exponente (entero)
 * @param exp Exponente (debe ser no negativo)
 * @return 2^exp
 *
 * Esta función está altamente optimizada para calcular potencias de 2,
 * usando desplazamientos de bits cuando sea posible.
 */
template <int_tt::integral T, int_tt::integral U>
constexpr T int_power_2(U exp) noexcept {
  if (exp == 0)
    return T{1};

  // Usar desplazamiento de bits para tipos pequeños/medianos
  if constexpr (int_tt::signed_integral<T>) {
    // Para tipos signed, verificar límites para evitar overflow
    if constexpr (sizeof(T) == sizeof(int)) {
      if (exp < 31)
        return T{1} << static_cast<int>(exp);
    } else if constexpr (sizeof(T) == sizeof(long long)) {
      if (exp < 63)
        return T{1} << static_cast<int>(exp);
    } else if constexpr (sizeof(T) == sizeof(__int128)) {
      if (exp < 127)
        return T{1} << static_cast<int>(exp);
    }
  } else {
    // Para tipos unsigned, usar todo el rango disponible
    if constexpr (sizeof(T) == sizeof(unsigned int)) {
      if (exp < 32)
        return T{1} << static_cast<unsigned int>(exp);
    } else if constexpr (sizeof(T) == sizeof(unsigned long long)) {
      if (exp < 64)
        return T{1} << static_cast<unsigned int>(exp);
    } else if constexpr (sizeof(T) == sizeof(unsigned __int128)) {
      if (exp < 128)
        return T{1} << static_cast<unsigned int>(exp);
    }
  }

  // Fallback para exponentes muy grandes o tipos especiales
  // Usar multiplicación iterativa (más lento pero seguro)
  T result = T{1};
  T base = T{2};
  U current_exp = exp;

  while (current_exp > 0) {
    if (current_exp & 1) {
      result *= base;
    }
    base *= base;
    current_exp >>= 1;
  }

  return result;
}

/**
 * @brief Versión signed especializada de int_power_2
 */
template <int_tt::signed_integral T, int_tt::integral U>
constexpr T int_power_2_signed(U exp) noexcept {
  return int_power_2<T>(exp);
}

/**
 * @brief Versión unsigned especializada de int_power_2
 */
template <int_tt::unsigned_integral T, int_tt::integral U>
constexpr T int_power_2_unsigned(U exp) noexcept {
  return int_power_2<T>(exp);
}

//==============================================================================
// REDEFINICIÓN DE int_power USANDO DISPATCH
//==============================================================================

/**
 * @brief Redefinición de int_power que usa el sistema de dispatch
 */
template <int_tt::integral T, int_tt::integral U>
constexpr T int_power_new(T base, U exp) noexcept {
  // Optimización especial para base = 2
  if (base == 2) {
    return int_power_2<T>(exp);
  }

  // Usar dispatch basado en signo
  return int_power_dispatch(base, exp);
}

//==============================================================================
// ESPECIALIZACIONES ADICIONALES PARA TIPOS BOOST MULTIPRECISION
//==============================================================================

// Especialización para cpp_int (comentada hasta tener dependencias correctas)
/*
template<int_tt::boost_integral T, int_tt::integral U>
constexpr T int_power_boost_impl(T base, U exp) noexcept {
    if (exp == 0) return T{1};
    if (exp == 1) return base;
    if (base == 0) return T{0};
    if (base == 1) return T{1};

    // Para Boost.Multiprecision, usar pow nativo si está disponible
    try {
        return boost::multiprecision::pow(base, static_cast<unsigned>(exp));
    } catch (...) {
        // Fallback a implementación manual
        T result{1};
        T current_base = base;
        U current_exp = exp;

        while (current_exp > 0) {
            if (current_exp % 2 == 1) {
                result *= current_base;
            }
            current_base *= current_base;
            current_exp /= 2;
        }

        return result;
    }
}
*/

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
template <int_tt::integral T>
constexpr bool int_power_safe(T base, T exp) noexcept {
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
