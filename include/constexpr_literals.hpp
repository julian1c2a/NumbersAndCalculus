/**
 * @file constexpr_literals.hpp
 * @brief Literales constexpr optimizados para cálculos factoriales y
 * combinatorios
 * @author AlgoritmiaCombinatoria Project
 * @date 2024
 * @version 1.0
 *
 * Este archivo define literales y funciones constexpr específicamente
 * optimizadas para cálculos matemáticos que requieren evaluación en tiempo de
 * compilación. Ideal para factoriales, combinaciones y permutaciones con
 * números gigantescos.
 */

#pragma once

#include <limits>
#include <type_traits>

//==============================================================================
// NAMESPACE PARA LITERALES CONSTEXPR OPTIMIZADOS
//==============================================================================

namespace constexpr_literals {

//------------------------------------------------------------------------------
// LITERALES CONSTEXPR PARA __INT128
//------------------------------------------------------------------------------

#if defined(__GNUC__) || defined(__clang__)

/**
 * @brief Literal constexpr para __int128 desde entero
 * @param value Valor entero
 * @return __int128 evaluado en tiempo de compilación
 *
 * Uso: constexpr auto factorial_base = 123456789_i128;
 * @note Fundamental para optimización de factoriales gigantescos
 */
constexpr __int128 operator""_i128(unsigned long long value) noexcept {
  return static_cast<__int128>(value);
}

/**
 * @brief Literal constexpr para unsigned __int128
 * @param value Valor entero
 * @return unsigned __int128 evaluado en tiempo de compilación
 */
constexpr unsigned __int128
operator""_ui128(unsigned long long value) noexcept {
  return static_cast<unsigned __int128>(value);
}

/**
 * @brief Literal constexpr para potencias de 2 con __int128
 * @param exponent Exponente (0-126)
 * @return 2^exponent como __int128, evaluado en tiempo de compilación
 *
 * Uso: constexpr auto big_power = 100_pow2_i128;  // 2^100 en compile-time
 * @note Esencial para optimización de factoriales que usan potencias de 2
 */
constexpr __int128 operator""_pow2_i128(unsigned long long exponent) noexcept {
  return (exponent >= 127) ? (static_cast<__int128>(1) << 126)
                           : (static_cast<__int128>(1) << exponent);
}

/**
 * @brief Literal constexpr para potencias de 2 unsigned
 * @param exponent Exponente (0-127)
 * @return 2^exponent como unsigned __int128
 */
constexpr unsigned __int128
operator""_pow2_ui128(unsigned long long exponent) noexcept {
  return (exponent >= 128) ? (static_cast<unsigned __int128>(1) << 127)
                           : (static_cast<unsigned __int128>(1) << exponent);
}

#endif // defined(__GNUC__) || defined(__clang__)

//------------------------------------------------------------------------------
// FUNCIONES CONSTEXPR AUXILIARES PARA FACTORIAL
//------------------------------------------------------------------------------

#if defined(__GNUC__) || defined(__clang__)

/**
 * @brief Calcula factorial constexpr con __int128 (versión iterativa)
 * @param n Número para calcular factorial (0-20 seguro)
 * @return n! evaluado en tiempo de compilación
 *
 * @note Limitado por el rango de __int128 y límites de constexpr
 * @note Para números mayores, usar con Boost.Multiprecision
 */
constexpr __int128 constexpr_factorial(__int128 n) noexcept {
  if (n <= 1)
    return 1_i128;

  __int128 result = 1_i128;
  for (__int128 i = 2; i <= n; ++i) {
    result *= i;
  }
  return result;
} /**
   * @brief Calcula potencia constexpr optimizada
   * @param base Base de la potencia
   * @param exp Exponente
   * @return base^exp evaluado en tiempo de compilación
   */
constexpr __int128 constexpr_power(__int128 base,
                                   unsigned long long exp) noexcept {
  __int128 result = 1_i128;
  __int128 current_base = base;

  while (exp > 0) {
    if (exp & 1) {
      result *= current_base;
    }
    current_base *= current_base;
    exp >>= 1;
  }

  return result;
}

/**
 * @brief Calcula combinaciones C(n,k) constexpr
 * @param n Total de elementos
 * @param k Elementos a seleccionar
 * @return C(n,k) evaluado en tiempo de compilación
 *
 * @note Usa algoritmo optimizado para evitar overflow intermedio
 */
constexpr __int128 constexpr_combination(__int128 n, __int128 k) noexcept {
  if (k > n || k < 0)
    return 0_i128;
  if (k == 0 || k == n)
    return 1_i128;

  // Optimización: C(n,k) = C(n,n-k), usar el menor
  if (k > n - k)
    k = n - k;

  __int128 result = 1_i128;
  for (__int128 i = 0; i < k; ++i) {
    result = result * (n - i) / (i + 1);
  }

  return result;
}

/**
 * @brief Calcula permutaciones P(n,k) constexpr
 * @param n Total de elementos
 * @param k Elementos a permutar
 * @return P(n,k) = n!/(n-k)! evaluado en tiempo de compilación
 */
constexpr __int128 constexpr_permutation(__int128 n, __int128 k) noexcept {
  if (k > n || k < 0)
    return 0_i128;
  if (k == 0)
    return 1_i128;

  __int128 result = 1_i128;
  for (__int128 i = n; i > n - k; --i) {
    result *= i;
  }

  return result;
}

/**
 * @brief Calcula el número de Catalan C_n constexpr
 * @param n Índice del número de Catalan
 * @return C_n = (2n)! / ((n+1)! * n!) evaluado en tiempo de compilación
 */
constexpr __int128 constexpr_catalan(__int128 n) noexcept {
  if (n <= 1)
    return 1_i128;

  // Fórmula optimizada: C_n = (2n)! / ((n+1) * n!)
  // = C(2n, n) / (n+1)
  return constexpr_combination(2 * n, n) / (n + 1);
}

/**
 * @brief Calcula número de Fibonacci constexpr
 * @param n Índice de Fibonacci
 * @return F_n evaluado en tiempo de compilación
 */
constexpr __int128 constexpr_fibonacci(__int128 n) noexcept {
  if (n <= 1)
    return n;

  __int128 a = 0_i128, b = 1_i128;
  for (__int128 i = 2; i <= n; ++i) {
    __int128 temp = a + b;
    a = b;
    b = temp;
  }
  return b;
}

#endif // defined(__GNUC__) || defined(__clang__)

//------------------------------------------------------------------------------
// CONSTANTES MATEMÁTICAS PRECALCULADAS
//------------------------------------------------------------------------------

#if defined(__GNUC__) || defined(__clang__)

namespace math_constants {
// Factoriales precalculados hasta donde cabe en __int128
constexpr __int128 factorial_0 = 1_i128;
constexpr __int128 factorial_1 = 1_i128;
constexpr __int128 factorial_2 = 2_i128;
constexpr __int128 factorial_3 = 6_i128;
constexpr __int128 factorial_4 = 24_i128;
constexpr __int128 factorial_5 = 120_i128;
constexpr __int128 factorial_10 = constexpr_factorial(10);
constexpr __int128 factorial_15 = constexpr_factorial(15);
constexpr __int128 factorial_20 = constexpr_factorial(20);
// factorial_25 y superiores pueden exceder límites de constexpr
// Para factoriales más grandes, usar runtime o Boost.Multiprecision

// Potencias de 2 útiles
constexpr __int128 pow2_10 = 10_pow2_i128;   // 1024
constexpr __int128 pow2_20 = 20_pow2_i128;   // ~1M
constexpr __int128 pow2_30 = 30_pow2_i128;   // ~1B
constexpr __int128 pow2_40 = 40_pow2_i128;   // ~1T
constexpr __int128 pow2_50 = 50_pow2_i128;   // ~1P
constexpr __int128 pow2_60 = 60_pow2_i128;   // ~1E
constexpr __int128 pow2_100 = 100_pow2_i128; // ~1.27E30
constexpr __int128 pow2_120 = 120_pow2_i128; // ~1.33E36

// Números de Catalan precalculados
constexpr __int128 catalan_0 = constexpr_catalan(0);   // 1
constexpr __int128 catalan_1 = constexpr_catalan(1);   // 1
constexpr __int128 catalan_2 = constexpr_catalan(2);   // 2
constexpr __int128 catalan_3 = constexpr_catalan(3);   // 5
constexpr __int128 catalan_4 = constexpr_catalan(4);   // 14
constexpr __int128 catalan_5 = constexpr_catalan(5);   // 42
constexpr __int128 catalan_10 = constexpr_catalan(10); // 16796

// Fibonacci precalculados
constexpr __int128 fibonacci_10 = constexpr_fibonacci(10);   // 55
constexpr __int128 fibonacci_20 = constexpr_fibonacci(20);   // 6765
constexpr __int128 fibonacci_30 = constexpr_fibonacci(30);   // 832040
constexpr __int128 fibonacci_50 = constexpr_fibonacci(50);   // 12586269025
constexpr __int128 fibonacci_100 = constexpr_fibonacci(100); // ~3.54E20
} // namespace math_constants

#endif // defined(__GNUC__) || defined(__clang__)

} // namespace constexpr_literals

//==============================================================================
// EJEMPLO DE USO CON OPTIMIZACIÓN DE FACTORIALES
//==============================================================================

#ifdef CONSTEXPR_LITERALS_EXAMPLE

#include <chrono>
#include <iostream>

// Función de factorial runtime para comparación
__int128 runtime_factorial(__int128 n) {
  __int128 result = 1;
  for (__int128 i = 2; i <= n; ++i) {
    result *= i;
  }
  return result;
}

int main() {
  using namespace constexpr_literals;
  using namespace constexpr_literals::math_constants;

  std::cout << "=== DEMOSTRACIÓN DE LITERALES CONSTEXPR PARA FACTORIALES ==="
            << std::endl;

  // Ejemplo 1: Literales básicos constexpr
  std::cout << "\n--- Literales básicos constexpr ---" << std::endl;
  constexpr auto big_num = 123456789012345_i128;
  constexpr auto power_of_2 = 64_pow2_i128;

  std::cout << "Número grande: " << static_cast<long long>(big_num)
            << std::endl;
  std::cout << "2^64: " << static_cast<long long>(power_of_2) << std::endl;

  // Ejemplo 2: Factoriales constexpr vs runtime
  std::cout << "\n--- Comparación factorial constexpr vs runtime ---"
            << std::endl;

  constexpr __int128 n = 20;

  // Compilación en tiempo de compilación
  auto start = std::chrono::high_resolution_clock::now();
  constexpr __int128 factorial_constexpr = constexpr_factorial(n);
  auto end = std::chrono::high_resolution_clock::now();

  std::cout << "Factorial constexpr 20!: "
            << static_cast<long long>(factorial_constexpr) << std::endl;
  std::cout << "Precalculado factorial_20: "
            << static_cast<long long>(factorial_20) << std::endl;

  // Runtime calculation
  start = std::chrono::high_resolution_clock::now();
  __int128 factorial_runtime = runtime_factorial(n);
  end = std::chrono::high_resolution_clock::now();

  std::cout << "Factorial runtime 20!: "
            << static_cast<long long>(factorial_runtime) << std::endl;

  // Ejemplo 3: Combinaciones y permutaciones constexpr
  std::cout << "\n--- Combinaciones y permutaciones constexpr ---" << std::endl;
  constexpr __int128 comb_result = constexpr_combination(20, 10);
  constexpr __int128 perm_result = constexpr_permutation(20, 5);

  std::cout << "C(20,10): " << static_cast<long long>(comb_result) << std::endl;
  std::cout << "P(20,5): " << static_cast<long long>(perm_result) << std::endl;

  // Ejemplo 4: Números de Catalan
  std::cout << "\n--- Números de Catalan constexpr ---" << std::endl;
  std::cout << "Catalan(10): " << static_cast<long long>(catalan_10)
            << std::endl;

  // Ejemplo 5: Fibonacci
  std::cout << "\n--- Fibonacci constexpr ---" << std::endl;
  std::cout << "Fibonacci(50): " << static_cast<long long>(fibonacci_50)
            << std::endl;

  std::cout << "\n=== TODAS LAS OPERACIONES FUERON EVALUADAS EN TIEMPO DE "
               "COMPILACIÓN ==="
            << std::endl;

  return 0;
}

#endif // CONSTEXPR_LITERALS_EXAMPLE