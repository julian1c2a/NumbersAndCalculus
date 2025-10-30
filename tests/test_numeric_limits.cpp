/**
 * @file test_numeric_limits.cpp
 * @brief Tests para las especializaciones de std::numeric_limits
 * @author AlgoritmiaCombinatoria Project
 * @date 2024
 */

#include "../include/number_type_traits.hpp"
#include "../include/numeric_limits.hpp"
#include <boost/multiprecision/cpp_int.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <type_traits>

using namespace boost::multiprecision;
using namespace numeric_limits_extensions;

//==============================================================================
// TESTS PARA ESPECIALIZACIONES DE NUMERIC_LIMITS
//==============================================================================

TEST_CASE("numeric_limits especializaciones básicas", "[numeric_limits]") {

  SECTION("cpp_int traits básicos") {
    using BigInt = cpp_int;

    REQUIRE(std::numeric_limits<BigInt>::is_specialized == true);
    REQUIRE(std::numeric_limits<BigInt>::is_signed == true);
    REQUIRE(std::numeric_limits<BigInt>::is_integer == true);
    REQUIRE(std::numeric_limits<BigInt>::is_exact == true);
    REQUIRE(std::numeric_limits<BigInt>::is_bounded ==
            false); // Precisión arbitraria
    REQUIRE(std::numeric_limits<BigInt>::is_modulo == false);
    REQUIRE(std::numeric_limits<BigInt>::has_infinity == false);
    REQUIRE(std::numeric_limits<BigInt>::has_quiet_NaN == false);
    REQUIRE(std::numeric_limits<BigInt>::has_signaling_NaN == false);
  }

  SECTION("cpp_int propiedades numéricas") {
    using BigInt = cpp_int;

    REQUIRE(std::numeric_limits<BigInt>::digits == 0);       // No aplicable
    REQUIRE(std::numeric_limits<BigInt>::digits10 == 0);     // No aplicable
    REQUIRE(std::numeric_limits<BigInt>::max_digits10 == 0); // No aplicable
    REQUIRE(std::numeric_limits<BigInt>::radix == 2);
    REQUIRE(std::numeric_limits<BigInt>::min_exponent == 0);
    REQUIRE(std::numeric_limits<BigInt>::max_exponent == 0);
  }

  SECTION("cpp_int valores especiales") {
    using BigInt = cpp_int;

    // Para tipos de precisión arbitraria, estos métodos retornan 0
    // ya que no tienen límites fijos
    REQUIRE(std::numeric_limits<BigInt>::min() == 0);
    REQUIRE(std::numeric_limits<BigInt>::max() == 0);
    REQUIRE(std::numeric_limits<BigInt>::lowest() == 0);
    REQUIRE(std::numeric_limits<BigInt>::epsilon() == 0);
    REQUIRE(std::numeric_limits<BigInt>::round_error() == 0);
    REQUIRE(std::numeric_limits<BigInt>::infinity() == 0);
    REQUIRE(std::numeric_limits<BigInt>::quiet_NaN() == 0);
    REQUIRE(std::numeric_limits<BigInt>::signaling_NaN() == 0);
    REQUIRE(std::numeric_limits<BigInt>::denorm_min() == 0);
  }
}

TEST_CASE("numeric_limits para tipos de tamaño fijo",
          "[numeric_limits][fixed_size]") {

  SECTION("int128_t traits básicos") {
    using Int128 = int128_t;

    REQUIRE(std::numeric_limits<Int128>::is_specialized == true);
    REQUIRE(std::numeric_limits<Int128>::is_signed == true);
    REQUIRE(std::numeric_limits<Int128>::is_integer == true);
    REQUIRE(std::numeric_limits<Int128>::is_exact == true);
    REQUIRE(std::numeric_limits<Int128>::is_bounded == true); // Tamaño fijo
    REQUIRE(std::numeric_limits<Int128>::is_modulo == true);
  }

  SECTION("int128_t propiedades numéricas") {
    using Int128 = int128_t;

    REQUIRE(std::numeric_limits<Int128>::digits == 127); // 127 bits + signo
    REQUIRE(std::numeric_limits<Int128>::digits10 ==
            38); // ~38 dígitos decimales
    REQUIRE(std::numeric_limits<Int128>::max_digits10 == 40);
    REQUIRE(std::numeric_limits<Int128>::radix == 2);
  }

  SECTION("uint128_t traits básicos") {
    using UInt128 = uint128_t;

    REQUIRE(std::numeric_limits<UInt128>::is_specialized == true);
    REQUIRE(std::numeric_limits<UInt128>::is_signed == false);
    REQUIRE(std::numeric_limits<UInt128>::is_integer == true);
    REQUIRE(std::numeric_limits<UInt128>::is_exact == true);
    REQUIRE(std::numeric_limits<UInt128>::is_bounded == true);
    REQUIRE(std::numeric_limits<UInt128>::is_modulo == true);
  }

  SECTION("uint128_t propiedades numéricas") {
    using UInt128 = uint128_t;

    REQUIRE(std::numeric_limits<UInt128>::digits == 128); // 128 bits sin signo
    REQUIRE(std::numeric_limits<UInt128>::digits10 == 38);
    REQUIRE(std::numeric_limits<UInt128>::max_digits10 == 40);
    REQUIRE(std::numeric_limits<UInt128>::radix == 2);

    // Para unsigned, min() siempre es 0
    REQUIRE(std::numeric_limits<UInt128>::min() == 0);
  }
}

//==============================================================================
// TESTS PARA TRAITS PERSONALIZADOS
//==============================================================================

TEST_CASE("traits personalizados de precisión arbitraria",
          "[numeric_limits][arbitrary_precision]") {

  SECTION("is_arbitrary_precision trait") {
    REQUIRE(is_arbitrary_precision<cpp_int>::value == true);
    REQUIRE(is_arbitrary_precision<int>::value == false);
    REQUIRE(is_arbitrary_precision<long long>::value == false);
    REQUIRE(is_arbitrary_precision<int128_t>::value == false);  // Tamaño fijo
    REQUIRE(is_arbitrary_precision<uint128_t>::value == false); // Tamaño fijo

#ifdef BOOST_HAS_TOMMATH
    REQUIRE(is_arbitrary_precision<tom_int>::value == true);
#endif

#ifdef BOOST_HAS_GMP
    REQUIRE(is_arbitrary_precision<mpz_int>::value == true);
#endif
  }

  SECTION("is_arbitrary_precision_v variable template") {
    REQUIRE(is_arbitrary_precision_v<cpp_int> == true);
    REQUIRE(is_arbitrary_precision_v<int> == false);
    REQUIRE(is_arbitrary_precision_v<int128_t> == false);
  }

  SECTION("has_arbitrary_precision function") {
    REQUIRE(has_arbitrary_precision<cpp_int>() == true);
    REQUIRE(has_arbitrary_precision<int>() == false);
    REQUIRE(has_arbitrary_precision<int128_t>() == false);
  }
}

//==============================================================================
// TESTS DE INTEGRACIÓN CON NUMBER_TYPE_TRAITS
//==============================================================================

TEST_CASE("integración con number_type_traits",
          "[numeric_limits][integration]") {

  SECTION("tipos de precisión arbitraria") {
    using namespace integral_type_traits;

    // cpp_int debe satisfacer ambos sistemas de traits
    REQUIRE(integral<cpp_int> == true);
    REQUIRE(infty_integral<cpp_int> == true);
    REQUIRE(is_arbitrary_precision_v<cpp_int> == true);
    REQUIRE(std::numeric_limits<cpp_int>::is_bounded == false);
  }

  SECTION("tipos de tamaño fijo") {
    using namespace integral_type_traits;

    // int128_t debe ser integral pero finito
    REQUIRE(integral<int128_t> == true);
    REQUIRE(finite_integral<int128_t> == true);
    REQUIRE(infty_integral<int128_t> == false);
    REQUIRE(is_arbitrary_precision_v<int128_t> == false);
    REQUIRE(std::numeric_limits<int128_t>::is_bounded == true);
  }
}

//==============================================================================
// TESTS DE COMPORTAMIENTO EN TIEMPO DE COMPILACIÓN
//==============================================================================

TEST_CASE("evaluación en tiempo de compilación",
          "[numeric_limits][constexpr]") {

  SECTION("valores constexpr") {
    // Verificar que los valores son evaluables en tiempo de compilación
    static_assert(std::numeric_limits<cpp_int>::is_specialized);
    static_assert(std::numeric_limits<cpp_int>::is_signed);
    static_assert(std::numeric_limits<cpp_int>::is_integer);
    static_assert(!std::numeric_limits<cpp_int>::is_bounded);

    static_assert(std::numeric_limits<int128_t>::is_bounded);
    static_assert(std::numeric_limits<int128_t>::digits == 127);
    static_assert(std::numeric_limits<uint128_t>::digits == 128);

    static_assert(is_arbitrary_precision_v<cpp_int>);
    static_assert(!is_arbitrary_precision_v<int128_t>);
  }

  SECTION("concepts en tiempo de compilación") {
    using namespace numeric_limits_extensions;

    // Verificar que los concepts funcionan en tiempo de compilación
    static_assert(arbitrary_precision<cpp_int>);
    static_assert(!arbitrary_precision<int>);
    static_assert(!arbitrary_precision<int128_t>);
  }
}

//==============================================================================
// TESTS DE DEMOSTRACIÓN Y EJEMPLOS
//==============================================================================

TEST_CASE("ejemplos de uso práctico", "[numeric_limits][examples]") {

  SECTION("detección de capacidades en templates") {
    auto test_type_properties = []<typename T>() {
      if constexpr (std::numeric_limits<T>::is_bounded) {
        return "bounded";
      } else if constexpr (is_arbitrary_precision_v<T>) {
        return "arbitrary_precision";
      } else {
        return "unknown";
      }
    };

    REQUIRE(std::string(test_type_properties.template operator()<cpp_int>()) ==
            "arbitrary_precision");
    REQUIRE(std::string(test_type_properties.template operator()<int128_t>()) ==
            "bounded");
    REQUIRE(std::string(test_type_properties.template operator()<int>()) ==
            "bounded");
  }

  SECTION("uso con concepts personalizados") {
    using namespace numeric_limits_extensions;

    auto process_arbitrary_precision = []<arbitrary_precision T>(T value) {
      // Esta función solo acepta tipos de precisión arbitraria
      return value * T(2);
    };

    cpp_int big_num = 12345;
    auto result = process_arbitrary_precision(big_num);
    REQUIRE(result == 24690);

    // Esto no compila: process_arbitrary_precision(123); // int no es
    // arbitrary_precision
  }
}

//==============================================================================
// TESTS DE BACKEND ESPECÍFICOS (CONDICIONALES)
//==============================================================================

#ifdef BOOST_HAS_TOMMATH
TEST_CASE("numeric_limits para tom_int", "[numeric_limits][tommath]") {
  SECTION("tom_int traits básicos") {
    using TomInt = tom_int;

    REQUIRE(std::numeric_limits<TomInt>::is_specialized == true);
    REQUIRE(std::numeric_limits<TomInt>::is_signed == true);
    REQUIRE(std::numeric_limits<TomInt>::is_integer == true);
    REQUIRE(std::numeric_limits<TomInt>::is_bounded == false);
    REQUIRE(is_arbitrary_precision_v<TomInt> == true);
  }
}
#endif

#ifdef BOOST_HAS_GMP
TEST_CASE("numeric_limits para mpz_int", "[numeric_limits][gmp]") {
  SECTION("mpz_int traits básicos") {
    using MpzInt = mpz_int;

    REQUIRE(std::numeric_limits<MpzInt>::is_specialized == true);
    REQUIRE(std::numeric_limits<MpzInt>::is_signed == true);
    REQUIRE(std::numeric_limits<MpzInt>::is_integer == true);
    REQUIRE(std::numeric_limits<MpzInt>::is_bounded == false);
    REQUIRE(is_arbitrary_precision_v<MpzInt> == true);
  }
}
#endif

//==============================================================================
// NOTA: TIPOS NO ESTÁNDAR (__int128) - YA SOPORTADOS NATIVAMENTE
//==============================================================================

// Los tipos __int128 y unsigned __int128 ya están soportados nativamente
// en versiones modernas de GCC (15.2.0+) y Clang a través de
// std::numeric_limits No necesitamos tests personalizados ya que el soporte
// está en la biblioteca estándar.
//
// Si necesitas usar estos tipos, simplemente incluye <limits> y usa:
//   std::numeric_limits<__int128>::max()
//   std::numeric_limits<unsigned __int128>::max()
// etc.

#if defined(__GNUC__) || defined(__clang__)
TEST_CASE("verificacion de soporte para __int128",
          "[numeric_limits][int128][support]") {

  SECTION("__int128 disponible con especializaciones de numeric_limits") {
    // Verificar que __int128 está disponible
    REQUIRE(sizeof(__int128) == 16); // 128 bits = 16 bytes

    // Verificar que std::numeric_limits<__int128> está especializado
    // Ya sea nativamente (con gnu++XX) o por nuestras especializaciones (con
    // c++XX)
    REQUIRE(std::numeric_limits<__int128>::is_specialized == true);
    REQUIRE(std::numeric_limits<__int128>::is_signed == true);
    REQUIRE(std::numeric_limits<__int128>::is_integer == true);
    REQUIRE(std::numeric_limits<__int128>::is_bounded == true);
    REQUIRE(std::numeric_limits<__int128>::digits == 127);
    REQUIRE(std::numeric_limits<__int128>::digits10 == 38);

    // Los traits personalizados siguen funcionando
    REQUIRE(is_arbitrary_precision_v<__int128> == false);
    REQUIRE(has_arbitrary_precision<__int128>() == false);
  }

  SECTION("unsigned __int128 disponible con especializaciones") {
    REQUIRE(sizeof(unsigned __int128) == 16); // 128 bits = 16 bytes

    REQUIRE(std::numeric_limits<unsigned __int128>::is_specialized == true);
    REQUIRE(std::numeric_limits<unsigned __int128>::is_signed == false);
    REQUIRE(std::numeric_limits<unsigned __int128>::is_integer == true);
    REQUIRE(std::numeric_limits<unsigned __int128>::is_bounded == true);
    REQUIRE(std::numeric_limits<unsigned __int128>::digits == 128);
    REQUIRE(std::numeric_limits<unsigned __int128>::digits10 == 38);

    REQUIRE(is_arbitrary_precision_v<unsigned __int128> == false);
    REQUIRE(has_arbitrary_precision<unsigned __int128>() == false);
  }

  SECTION("operaciones basicas con __int128") {
    __int128 a = 1;
    a <<= 100; // 2^100

    unsigned __int128 b = 1;
    b <<= 100; // 2^100

    // Verificar que las operaciones funcionan
    REQUIRE(a > 0);
    REQUIRE(b > 0);
    REQUIRE(static_cast<unsigned __int128>(a) == b);
  }

  SECTION("verificacion de rangos min/max") {
    // __int128 con signo
    __int128 min_val = std::numeric_limits<__int128>::min();
    __int128 max_val = std::numeric_limits<__int128>::max();

    REQUIRE(min_val < 0); // Valor mínimo es negativo
    REQUIRE(max_val > 0); // Valor máximo es positivo
    REQUIRE(max_val > min_val);

    // unsigned __int128
    unsigned __int128 u_min = std::numeric_limits<unsigned __int128>::min();
    unsigned __int128 u_max = std::numeric_limits<unsigned __int128>::max();

    REQUIRE(u_min == 0); // Mínimo es 0 para unsigned
    REQUIRE(u_max > 0);  // Máximo es positivo
    REQUIRE(u_max > u_min);
  }
}
#endif // defined(__GNUC__) || defined(__clang__)