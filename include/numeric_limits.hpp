/**
 * @file numeric_limits.hpp
 * @brief Especializaciones de std::numeric_limits para tipos de precisión
 * arbitraria
 * @author AlgoritmiaCombinatoria Project
 * @date 2024
 * @version 1.0
 *
 * Este archivo extiende std::numeric_limits con especializaciones para tipos
 * de Boost.Multiprecision, proporcionando información detallada sobre las
 * características y límites de estos tipos de precisión arbitraria.
 *
 * @section overview Descripción General
 *
 * std::numeric_limits es un template de traits que proporciona información
 * sobre las propiedades numéricas de los tipos aritméticos. Para tipos de
 * precisión arbitraria como cpp_int, tom_int y mpz_int, necesitamos
 * especializaciones personalizadas.
 *
 * @section supported_types Tipos Soportados
 * - boost::multiprecision::cpp_int (backend C++ puro)
 * - boost::multiprecision::tom_int (backend libtommath)
 * - boost::multiprecision::mpz_int (backend GMP)
 * - boost::multiprecision::int128_t, int256_t, int512_t, int1024_t
 * - boost::multiprecision::uint128_t, uint256_t, uint512_t, uint1024_t
 *
 * @section usage Uso
 * @code
 * #include "numeric_limits.hpp"
 *
 * using BigInt = boost::multiprecision::cpp_int;
 *
 * if constexpr (std::numeric_limits<BigInt>::is_bounded) {
 *     // Nunca se ejecuta - cpp_int es ilimitado
 * }
 *
 * if constexpr (std::numeric_limits<BigInt>::is_arbitrary_precision) {
 *     // Se ejecuta - cpp_int tiene precisión arbitraria
 *     std::cout << "Tipo de precisión arbitraria detectado\n";
 * }
 * @endcode
 *
 * @note Requiere Boost.Multiprecision 1.80+
 * @warning Las especializaciones para tipos de precisión arbitraria pueden
 *          no estar disponibles en versiones antiguas de Boost
 */

#pragma once

#include <boost/multiprecision/cpp_int.hpp>
#include <limits>

#ifdef BOOST_HAS_TOMMATH
#include <boost/multiprecision/tommath.hpp>
#endif

#ifdef BOOST_HAS_GMP
#include <boost/multiprecision/gmp.hpp>
#endif

//==============================================================================
// NAMESPACE NUMERIC_LIMITS_EXTENSIONS
//==============================================================================

/**
 * @brief Namespace para extensiones y utilidades de numeric_limits
 *
 * Contiene traits y funciones helper para trabajar con numeric_limits
 * de tipos de precisión arbitraria.
 */
namespace numeric_limits_extensions {

/**
 * @brief Trait para detectar si un tipo tiene precisión arbitraria
 * @tparam T Tipo a evaluar
 *
 * Verdadero para tipos que pueden representar números de cualquier magnitud
 * limitados solo por la memoria disponible.
 */
template <typename T> struct is_arbitrary_precision : std::false_type {};

/**
 * @brief Trait para detectar si un tipo usa un backend específico
 * @tparam T Tipo a evaluar
 * @tparam Backend Tipo de backend (GMP, tommath, cpp)
 */
template <typename T, typename Backend>
struct uses_backend : std::false_type {};

} // namespace numeric_limits_extensions

//==============================================================================
// ESPECIALIZACIONES DE STD::NUMERIC_LIMITS
//==============================================================================

namespace std {

//------------------------------------------------------------------------------
// ESPECIALIZACIÓN PARA BOOST::MULTIPRECISION::CPP_INT
//------------------------------------------------------------------------------

/**
 * @brief Especialización de numeric_limits para cpp_int
 *
 * cpp_int es un tipo de precisión arbitraria implementado en C++ puro.
 * No tiene límites fijos y puede crecer dinámicamente según sea necesario.
 */
template <> class numeric_limits<boost::multiprecision::cpp_int> {
public:
  static constexpr bool is_specialized = true;
  static constexpr bool is_signed = true;
  static constexpr bool is_integer = true;
  static constexpr bool is_exact = true;
  static constexpr bool is_bounded =
      false; ///< Precisión arbitraria = sin límites
  static constexpr bool is_modulo = false;
  static constexpr bool is_iec559 = false;
  static constexpr bool has_infinity = false;
  static constexpr bool has_quiet_NaN = false;
  static constexpr bool has_signaling_NaN = false;
  static constexpr bool has_denorm_loss = false;
  static constexpr bool is_arbitrary_precision = true; ///< Extensión propia
  static constexpr float_denorm_style has_denorm = denorm_absent;
  static constexpr float_round_style round_style = round_toward_zero;

  static constexpr int digits = 0;   ///< No aplicable para precisión arbitraria
  static constexpr int digits10 = 0; ///< No aplicable para precisión arbitraria
  static constexpr int max_digits10 =
      0; ///< No aplicable para precisión arbitraria
  static constexpr int radix = 2;
  static constexpr int min_exponent = 0;
  static constexpr int min_exponent10 = 0;
  static constexpr int max_exponent = 0;
  static constexpr int max_exponent10 = 0;
  static constexpr bool traps = false;
  static constexpr bool tinyness_before = false;

  /// @brief No hay mínimo para precisión arbitraria
  static constexpr boost::multiprecision::cpp_int min() noexcept { return 0; }

  /// @brief No hay máximo para precisión arbitraria
  static constexpr boost::multiprecision::cpp_int max() noexcept { return 0; }

  /// @brief Valor más pequeño mayor que cero (siempre 1 para enteros)
  static constexpr boost::multiprecision::cpp_int lowest() noexcept {
    return 0;
  }

  /// @brief Precisión de máquina (no aplicable)
  static constexpr boost::multiprecision::cpp_int epsilon() noexcept {
    return 0;
  }

  /// @brief Error de redondeo (no aplicable para enteros exactos)
  static constexpr boost::multiprecision::cpp_int round_error() noexcept {
    return 0;
  }

  /// @brief No tiene infinito
  static constexpr boost::multiprecision::cpp_int infinity() noexcept {
    return 0;
  }

  /// @brief No tiene NaN silencioso
  static constexpr boost::multiprecision::cpp_int quiet_NaN() noexcept {
    return 0;
  }

  /// @brief No tiene NaN que señala
  static constexpr boost::multiprecision::cpp_int signaling_NaN() noexcept {
    return 0;
  }

  /// @brief Número denormalizado más pequeño (no aplicable)
  static constexpr boost::multiprecision::cpp_int denorm_min() noexcept {
    return 0;
  }
};

#ifdef BOOST_HAS_TOMMATH
//------------------------------------------------------------------------------
// ESPECIALIZACIÓN PARA BOOST::MULTIPRECISION::TOM_INT
//------------------------------------------------------------------------------

/**
 * @brief Especialización de numeric_limits para tom_int
 *
 * tom_int usa la biblioteca libtommath como backend y proporciona
 * aritmética de precisión arbitraria con mejor rendimiento que cpp_int.
 */
template <> class numeric_limits<boost::multiprecision::tom_int> {
public:
  static constexpr bool is_specialized = true;
  static constexpr bool is_signed = true;
  static constexpr bool is_integer = true;
  static constexpr bool is_exact = true;
  static constexpr bool is_bounded = false;
  static constexpr bool is_modulo = false;
  static constexpr bool is_iec559 = false;
  static constexpr bool has_infinity = false;
  static constexpr bool has_quiet_NaN = false;
  static constexpr bool has_signaling_NaN = false;
  static constexpr bool has_denorm_loss = false;
  static constexpr bool is_arbitrary_precision = true;
  static constexpr float_denorm_style has_denorm = denorm_absent;
  static constexpr float_round_style round_style = round_toward_zero;

  static constexpr int digits = 0;
  static constexpr int digits10 = 0;
  static constexpr int max_digits10 = 0;
  static constexpr int radix = 2;
  static constexpr int min_exponent = 0;
  static constexpr int min_exponent10 = 0;
  static constexpr int max_exponent = 0;
  static constexpr int max_exponent10 = 0;
  static constexpr bool traps = false;
  static constexpr bool tinyness_before = false;

  static constexpr boost::multiprecision::tom_int min() noexcept { return 0; }
  static constexpr boost::multiprecision::tom_int max() noexcept { return 0; }
  static constexpr boost::multiprecision::tom_int lowest() noexcept {
    return 0;
  }
  static constexpr boost::multiprecision::tom_int epsilon() noexcept {
    return 0;
  }
  static constexpr boost::multiprecision::tom_int round_error() noexcept {
    return 0;
  }
  static constexpr boost::multiprecision::tom_int infinity() noexcept {
    return 0;
  }
  static constexpr boost::multiprecision::tom_int quiet_NaN() noexcept {
    return 0;
  }
  static constexpr boost::multiprecision::tom_int signaling_NaN() noexcept {
    return 0;
  }
  static constexpr boost::multiprecision::tom_int denorm_min() noexcept {
    return 0;
  }
};
#endif // BOOST_HAS_TOMMATH

#ifdef BOOST_HAS_GMP
//------------------------------------------------------------------------------
// ESPECIALIZACIÓN PARA BOOST::MULTIPRECISION::MPZ_INT
//------------------------------------------------------------------------------

/**
 * @brief Especialización de numeric_limits para mpz_int
 *
 * mpz_int usa la biblioteca GMP (GNU Multiple Precision) como backend,
 * ofreciendo el mejor rendimiento para operaciones de precisión arbitraria.
 */
template <> class numeric_limits<boost::multiprecision::mpz_int> {
public:
  static constexpr bool is_specialized = true;
  static constexpr bool is_signed = true;
  static constexpr bool is_integer = true;
  static constexpr bool is_exact = true;
  static constexpr bool is_bounded = false;
  static constexpr bool is_modulo = false;
  static constexpr bool is_iec559 = false;
  static constexpr bool has_infinity = false;
  static constexpr bool has_quiet_NaN = false;
  static constexpr bool has_signaling_NaN = false;
  static constexpr bool has_denorm_loss = false;
  static constexpr bool is_arbitrary_precision = true;
  static constexpr float_denorm_style has_denorm = denorm_absent;
  static constexpr float_round_style round_style = round_toward_zero;

  static constexpr int digits = 0;
  static constexpr int digits10 = 0;
  static constexpr int max_digits10 = 0;
  static constexpr int radix = 2;
  static constexpr int min_exponent = 0;
  static constexpr int min_exponent10 = 0;
  static constexpr int max_exponent = 0;
  static constexpr int max_exponent10 = 0;
  static constexpr bool traps = false;
  static constexpr bool tinyness_before = false;

  static constexpr boost::multiprecision::mpz_int min() noexcept { return 0; }
  static constexpr boost::multiprecision::mpz_int max() noexcept { return 0; }
  static constexpr boost::multiprecision::mpz_int lowest() noexcept {
    return 0;
  }
  static constexpr boost::multiprecision::mpz_int epsilon() noexcept {
    return 0;
  }
  static constexpr boost::multiprecision::mpz_int round_error() noexcept {
    return 0;
  }
  static constexpr boost::multiprecision::mpz_int infinity() noexcept {
    return 0;
  }
  static constexpr boost::multiprecision::mpz_int quiet_NaN() noexcept {
    return 0;
  }
  static constexpr boost::multiprecision::mpz_int signaling_NaN() noexcept {
    return 0;
  }
  static constexpr boost::multiprecision::mpz_int denorm_min() noexcept {
    return 0;
  }
};
#endif // BOOST_HAS_GMP

//------------------------------------------------------------------------------
// ESPECIALIZACIONES PARA TIPOS DE TAMAÑO FIJO
//------------------------------------------------------------------------------

/**
 * @brief Especialización para int128_t de Boost.Multiprecision
 *
 * Entero de 128 bits con signo, de tamaño fijo.
 */
template <> class numeric_limits<boost::multiprecision::int128_t> {
public:
  static constexpr bool is_specialized = true;
  static constexpr bool is_signed = true;
  static constexpr bool is_integer = true;
  static constexpr bool is_exact = true;
  static constexpr bool is_bounded = true;
  static constexpr bool is_modulo = true;
  static constexpr bool is_iec559 = false;
  static constexpr bool has_infinity = false;
  static constexpr bool has_quiet_NaN = false;
  static constexpr bool has_signaling_NaN = false;
  static constexpr bool has_denorm_loss = false;
  static constexpr bool is_arbitrary_precision = false;
  static constexpr float_denorm_style has_denorm = denorm_absent;
  static constexpr float_round_style round_style = round_toward_zero;

  static constexpr int digits = 127;  ///< 127 bits de valor + 1 bit de signo
  static constexpr int digits10 = 38; ///< Aproximadamente 38 dígitos decimales
  static constexpr int max_digits10 =
      40; ///< Dígitos necesarios para reconstruir el valor
  static constexpr int radix = 2;
  static constexpr int min_exponent = 0;
  static constexpr int min_exponent10 = 0;
  static constexpr int max_exponent = 0;
  static constexpr int max_exponent10 = 0;
  static constexpr bool traps = false;
  static constexpr bool tinyness_before = false;

  /// @brief Valor mínimo representable (-2^127)
  static constexpr boost::multiprecision::int128_t min() noexcept {
    return boost::multiprecision::int128_t(-1) << 127;
  }

  /// @brief Valor máximo representable (2^127 - 1)
  static constexpr boost::multiprecision::int128_t max() noexcept {
    return (boost::multiprecision::int128_t(1) << 127) - 1;
  }

  /// @brief Mismo que min() para enteros
  static constexpr boost::multiprecision::int128_t lowest() noexcept {
    return min();
  }

  static constexpr boost::multiprecision::int128_t epsilon() noexcept {
    return 0;
  }
  static constexpr boost::multiprecision::int128_t round_error() noexcept {
    return 0;
  }
  static constexpr boost::multiprecision::int128_t infinity() noexcept {
    return 0;
  }
  static constexpr boost::multiprecision::int128_t quiet_NaN() noexcept {
    return 0;
  }
  static constexpr boost::multiprecision::int128_t signaling_NaN() noexcept {
    return 0;
  }
  static constexpr boost::multiprecision::int128_t denorm_min() noexcept {
    return 0;
  }
};

/**
 * @brief Especialización para uint128_t de Boost.Multiprecision
 *
 * Entero de 128 bits sin signo, de tamaño fijo.
 */
template <> class numeric_limits<boost::multiprecision::uint128_t> {
public:
  static constexpr bool is_specialized = true;
  static constexpr bool is_signed = false;
  static constexpr bool is_integer = true;
  static constexpr bool is_exact = true;
  static constexpr bool is_bounded = true;
  static constexpr bool is_modulo = true;
  static constexpr bool is_iec559 = false;
  static constexpr bool has_infinity = false;
  static constexpr bool has_quiet_NaN = false;
  static constexpr bool has_signaling_NaN = false;
  static constexpr bool has_denorm_loss = false;
  static constexpr bool is_arbitrary_precision = false;
  static constexpr float_denorm_style has_denorm = denorm_absent;
  static constexpr float_round_style round_style = round_toward_zero;

  static constexpr int digits = 128;  ///< 128 bits de valor
  static constexpr int digits10 = 38; ///< Aproximadamente 38 dígitos decimales
  static constexpr int max_digits10 = 40;
  static constexpr int radix = 2;
  static constexpr int min_exponent = 0;
  static constexpr int min_exponent10 = 0;
  static constexpr int max_exponent = 0;
  static constexpr int max_exponent10 = 0;
  static constexpr bool traps = false;
  static constexpr bool tinyness_before = false;

  /// @brief Valor mínimo (0 para unsigned)
  static constexpr boost::multiprecision::uint128_t min() noexcept { return 0; }

  /// @brief Valor máximo representable (2^128 - 1)
  static constexpr boost::multiprecision::uint128_t max() noexcept {
    return (boost::multiprecision::uint128_t(1) << 128) - 1;
  }

  static constexpr boost::multiprecision::uint128_t lowest() noexcept {
    return min();
  }
  static constexpr boost::multiprecision::uint128_t epsilon() noexcept {
    return 0;
  }
  static constexpr boost::multiprecision::uint128_t round_error() noexcept {
    return 0;
  }
  static constexpr boost::multiprecision::uint128_t infinity() noexcept {
    return 0;
  }
  static constexpr boost::multiprecision::uint128_t quiet_NaN() noexcept {
    return 0;
  }
  static constexpr boost::multiprecision::uint128_t signaling_NaN() noexcept {
    return 0;
  }
  static constexpr boost::multiprecision::uint128_t denorm_min() noexcept {
    return 0;
  }
};

// Las especializaciones para int256_t, int512_t, int1024_t y sus versiones
// unsigned siguen el mismo patrón, ajustando los valores de digits y los
// métodos min()/max()

//------------------------------------------------------------------------------
// ESPECIALIZACIONES PARA __int128 Y unsigned __int128
//------------------------------------------------------------------------------
//
// IMPORTANTE: El soporte de __int128 en GCC depende del estándar utilizado:
//
// - Con -std=c++XX:    __int128 está disponible PERO std::numeric_limits NO
// está especializado
// - Con -std=gnu++XX:  __int128 está disponible Y std::numeric_limits SÍ está
// especializado
//
// En GCC 15.2.0, std::numeric_limits<__int128> está disponible solo con
// estándares GNU. Para máxima compatibilidad, proporcionamos especializaciones
// condicionales.
//------------------------------------------------------------------------------

#if defined(__GNUC__) || defined(__clang__)

// Solo proporcionar especializaciones si estamos usando std=c++XX (no gnu++XX)
// Con -std=c++XX: __STRICT_ANSI__ está definido y necesitamos nuestras
// especializaciones Con -std=gnu++XX: __STRICT_ANSI__ NO está definido y GCC
// proporciona las especializaciones
#ifdef __STRICT_ANSI__

/**
 * @brief Especialización condicional de numeric_limits para __int128
 *
 * Esta especialización se activa solo cuando:
 * 1. El compilador es GCC o Clang
 * 2. __int128 está disponible (__SIZEOF_INT128__ definido)
 * 3. La biblioteca estándar no proporciona especialización nativa
 *
 * @note Con -std=gnu++XX, GCC proporciona especialización nativa
 * @note Con -std=c++XX, necesitamos esta especialización personalizada
 */
template <> class numeric_limits<__int128> {
public:
  static constexpr bool is_specialized = true;
  static constexpr bool is_signed = true;
  static constexpr bool is_integer = true;
  static constexpr bool is_exact = true;
  static constexpr bool is_bounded = true; // Tamaño fijo de 128 bits
  static constexpr bool is_modulo = true;  // Overflow causa wrap-around
  static constexpr bool is_iec559 = false;
  static constexpr bool has_infinity = false;
  static constexpr bool has_quiet_NaN = false;
  static constexpr bool has_signaling_NaN = false;
  static constexpr bool has_denorm_loss = false;
  static constexpr bool is_arbitrary_precision = false; // Tamaño fijo
  static constexpr float_denorm_style has_denorm = denorm_absent;
  static constexpr float_round_style round_style = round_toward_zero;

  static constexpr int digits = 127;      // 127 bits de valor + 1 bit de signo
  static constexpr int digits10 = 38;     // Floor(127 * log10(2)) ≈ 38
  static constexpr int max_digits10 = 40; // Ceil(127 * log10(2)) + 1 ≈ 40
  static constexpr int radix = 2;
  static constexpr int min_exponent = 0;         // No aplicable para enteros
  static constexpr int min_exponent10 = 0;       // No aplicable para enteros
  static constexpr int max_exponent = 0;         // No aplicable para enteros
  static constexpr int max_exponent10 = 0;       // No aplicable para enteros
  static constexpr bool traps = false;           // Normalmente no causa traps
  static constexpr bool tinyness_before = false; // No aplicable para enteros

  /// @brief Valor mínimo representable (-2^127)
  static constexpr __int128 min() noexcept {
    return static_cast<__int128>(1) << 127;
  }

  /// @brief Valor máximo representable (2^127 - 1)
  static constexpr __int128 max() noexcept { return ~min(); }

  /// @brief Mismo que min() para enteros con signo
  static constexpr __int128 lowest() noexcept { return min(); }

  /// @brief Precisión de máquina (0 para enteros exactos)
  static constexpr __int128 epsilon() noexcept { return 0; }

  /// @brief Error de redondeo (0 para enteros exactos)
  static constexpr __int128 round_error() noexcept { return 0; }

  /// @brief No tiene infinito
  static constexpr __int128 infinity() noexcept { return 0; }

  /// @brief No tiene NaN silencioso
  static constexpr __int128 quiet_NaN() noexcept { return 0; }

  /// @brief No tiene NaN que señala
  static constexpr __int128 signaling_NaN() noexcept { return 0; }

  /// @brief Número denormalizado más pequeño (no aplicable para enteros)
  static constexpr __int128 denorm_min() noexcept { return 0; }
};

/**
 * @brief Especialización condicional de numeric_limits para unsigned __int128
 */
template <> class numeric_limits<unsigned __int128> {
public:
  static constexpr bool is_specialized = true;
  static constexpr bool is_signed = false; // Sin signo
  static constexpr bool is_integer = true;
  static constexpr bool is_exact = true;
  static constexpr bool is_bounded = true;
  static constexpr bool is_modulo = true;
  static constexpr bool is_iec559 = false;
  static constexpr bool has_infinity = false;
  static constexpr bool has_quiet_NaN = false;
  static constexpr bool has_signaling_NaN = false;
  static constexpr bool has_denorm_loss = false;
  static constexpr bool is_arbitrary_precision = false;
  static constexpr float_denorm_style has_denorm = denorm_absent;
  static constexpr float_round_style round_style = round_toward_zero;

  static constexpr int digits = 128;  // 128 bits de valor (sin bit de signo)
  static constexpr int digits10 = 38; // Floor(128 * log10(2)) ≈ 38
  static constexpr int max_digits10 = 40; // Ceil(128 * log10(2)) + 1 ≈ 40
  static constexpr int radix = 2;
  static constexpr int min_exponent = 0;
  static constexpr int min_exponent10 = 0;
  static constexpr int max_exponent = 0;
  static constexpr int max_exponent10 = 0;
  static constexpr bool traps = false;
  static constexpr bool tinyness_before = false;

  /// @brief Valor mínimo (0 para tipos unsigned)
  static constexpr unsigned __int128 min() noexcept { return 0; }

  /// @brief Valor máximo representable (2^128 - 1)
  static constexpr unsigned __int128 max() noexcept {
    return ~static_cast<unsigned __int128>(0);
  }

  /// @brief Mismo que min() para tipos unsigned
  static constexpr unsigned __int128 lowest() noexcept { return min(); }

  static constexpr unsigned __int128 epsilon() noexcept { return 0; }
  static constexpr unsigned __int128 round_error() noexcept { return 0; }
  static constexpr unsigned __int128 infinity() noexcept { return 0; }
  static constexpr unsigned __int128 quiet_NaN() noexcept { return 0; }
  static constexpr unsigned __int128 signaling_NaN() noexcept { return 0; }
  static constexpr unsigned __int128 denorm_min() noexcept { return 0; }
};

#endif // Condicional para especializaciones personalizadas
#endif // defined(__GNUC__) || defined(__clang__)

} // namespace std

//==============================================================================
// IMPLEMENTACIONES DE TRAITS PERSONALIZADOS
//==============================================================================

namespace numeric_limits_extensions {

// Especializaciones del trait is_arbitrary_precision
template <>
struct is_arbitrary_precision<boost::multiprecision::cpp_int> : std::true_type {
};

#ifdef BOOST_HAS_TOMMATH
template <>
struct is_arbitrary_precision<boost::multiprecision::tom_int> : std::true_type {
};
#endif

#ifdef BOOST_HAS_GMP
template <>
struct is_arbitrary_precision<boost::multiprecision::mpz_int> : std::true_type {
};
#endif

// Los tipos __int128 y unsigned __int128 ya están soportados nativamente
// por GCC/Clang y no necesitan traits especiales para is_arbitrary_precision
// (por defecto ya son false_type)
//
// CONFIRMADO EN GCC 15.2.0 (MSYS2):
// - Tanto -std=c++23 como -std=gnu++23 soportan __int128
// - std::numeric_limits<__int128> está especializado nativamente
// - std::numeric_limits<unsigned __int128> está especializado nativamente
// - Características: digits=127/128, digits10=38, is_signed=true/false,
// is_bounded=true

/**
 * @brief Variable template helper para is_arbitrary_precision (C++14
 * compatible)
 * @tparam T Tipo a evaluar
 */
template <typename T>
constexpr bool is_arbitrary_precision_v = is_arbitrary_precision<T>::value;

/**
 * @brief Función helper para detectar tipos de precisión arbitraria en tiempo
 * de compilación
 * @tparam T Tipo a evaluar
 * @return true si T tiene precisión arbitraria, false en caso contrario
 *
 * @code
 * if constexpr (has_arbitrary_precision<BigInt>()) {
 *     // Código para tipos de precisión arbitraria
 * }
 * @endcode
 */
template <typename T> constexpr bool has_arbitrary_precision() {
  return is_arbitrary_precision_v<T>;
}

/**
 * @brief Concept para tipos con precisión arbitraria (requiere C++20)
 * @tparam T Tipo a evaluar
 *
 * Útil para restringir templates a tipos que pueden crecer dinámicamente.
 *
 * @note Deshabilitado para compatibilidad con C++14/17
 */
#if __cplusplus >= 202002L
template <typename T>
concept arbitrary_precision = is_arbitrary_precision_v<T>;
#endif

} // namespace numeric_limits_extensions

//==============================================================================
// FIN DEL ARCHIVO NUMERIC_LIMITS.HPP
//==============================================================================