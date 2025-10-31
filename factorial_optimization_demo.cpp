#include "include/constexpr_literals.hpp"
#include "include/int128_io.hpp"
#include <chrono>
#include <iostream>

#if defined(__SIZEOF_INT128__)

using namespace constexpr_literals;
using namespace constexpr_literals::math_constants;
using namespace int128_io;

// Ejemplo de función que usa constexpr para optimización
template <__int128 N> constexpr __int128 optimized_factorial_power() {
  // Todo esto se calcula en tiempo de compilación
  constexpr __int128 base_factorial = constexpr_factorial(N);
  constexpr __int128 power_factor = 10_pow2_i128; // 2^10 = 1024
  return base_factorial * power_factor;
}

// Función para demostrar uso en algoritmos combinatorios
template <__int128 N, __int128 K>
constexpr __int128 optimized_combination_with_factorial() {
  // Combinación usando factoriales precalculados en compile-time
  if constexpr (N <= 20 && K <= 20) {
    return constexpr_combination(N, K);
  } else {
    // Para números más grandes, usar fórmula optimizada
    return constexpr_permutation(N, K) / constexpr_factorial(K);
  }
}

int main() {
  std::cout << "=== OPTIMIZACIÓN DE FACTORIALES CON CONSTEXPR LITERALS ==="
            << std::endl;

  // Ejemplo 1: Literales básicos optimizados
  std::cout << "\n--- Literales constexpr para números gigantescos ---"
            << std::endl;

  constexpr auto giant_base = 999999999999999999_i128;
  constexpr auto power_128 = 64_pow2_i128; // 2^64
  constexpr auto fibonacci_big = constexpr_fibonacci(30);

  std::cout << "Base gigantesca: " << giant_base << std::endl;
  std::cout << "2^64: " << power_128 << std::endl;
  std::cout << "Fibonacci(30): " << fibonacci_big << std::endl;

  // Ejemplo 2: Optimización de factoriales en tiempo de compilación
  std::cout << "\n--- Factoriales optimizados en compile-time ---" << std::endl;

  // Estos cálculos NO se ejecutan en runtime, se precalculan
  constexpr auto optimized_result_15 = optimized_factorial_power<15>();
  constexpr auto optimized_result_20 = optimized_factorial_power<20>();

  std::cout << "Factorial(15) * 2^10: " << optimized_result_15 << std::endl;
  std::cout << "Factorial(20) * 2^10: " << optimized_result_20 << std::endl;

  // Ejemplo 3: Combinaciones optimizadas
  std::cout << "\n--- Combinaciones optimizadas constexpr ---" << std::endl;

  constexpr auto comb_10_5 = optimized_combination_with_factorial<10, 5>();
  constexpr auto comb_20_10 = optimized_combination_with_factorial<20, 10>();

  std::cout << "C(10,5) optimizado: " << comb_10_5 << std::endl;
  std::cout << "C(20,10) optimizado: " << comb_20_10 << std::endl;

  // Ejemplo 4: Comparación de performance
  std::cout << "\n--- Comparación de performance: constexpr vs runtime ---"
            << std::endl;

  auto start_time = std::chrono::high_resolution_clock::now();

  // Runtime calculation
  __int128 runtime_result = 1_i128;
  for (int i = 1; i <= 20; ++i) {
    runtime_result *= i;
  }
  runtime_result *= (1_i128 << 10); // * 2^10

  auto end_time = std::chrono::high_resolution_clock::now();
  auto runtime_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(
      end_time - start_time);

  std::cout << "Runtime result: " << runtime_result << std::endl;
  std::cout << "Runtime time: " << runtime_duration.count() << " nanoseconds"
            << std::endl;
  std::cout << "Constexpr result: " << optimized_result_20
            << " (0 nanoseconds - calculado en compile-time)" << std::endl;

  // Ejemplo 5: Uso en algoritmos que requieren números gigantescos
  std::cout << "\n--- Aplicación en algoritmos combinatorios ---" << std::endl;

  // Simulación de cálculo de combinaciones para problemas grandes
  constexpr __int128 total_elements = 50_i128;
  constexpr __int128 selected_elements = 25_i128;

  // En un problema real, podrías tener:
  // - Número de formas de organizar un torneo
  // - Combinaciones genéticas posibles
  // - Configuraciones de un sistema

  auto large_combination =
      constexpr_combination(total_elements, selected_elements);
  std::cout << "C(50,25) = " << large_combination << std::endl;

  // Ejemplo 6: Potencias de 2 para algoritmos bit-manipulation
  std::cout << "\n--- Potencias de 2 para optimización de algoritmos ---"
            << std::endl;

  constexpr auto mask_32 = 32_pow2_i128 - 1_i128; // 2^32 - 1
  constexpr auto mask_64 = 64_pow2_i128 - 1_i128; // 2^64 - 1

  std::cout << "Máscara 32 bits: " << mask_32 << std::endl;
  std::cout << "Máscara 64 bits: " << mask_64 << std::endl;

  // Ejemplo de uso en hash table sizing
  constexpr auto hash_table_size = 20_pow2_i128; // 2^20 = ~1M entries
  std::cout << "Tamaño optimal hash table: " << hash_table_size << " entries"
            << std::endl;

  std::cout << "\n=== TODOS LOS CÁLCULOS PRINCIPALES FUERON OPTIMIZADOS EN "
               "COMPILE-TIME ==="
            << std::endl;
  std::cout << "Esto significa CERO overhead de runtime para estos cálculos!"
            << std::endl;

  return 0;
}

#else // !defined(__SIZEOF_INT128__)

#include <iostream>

int main() {
  std::cout
      << "Esta demo requiere soporte para __int128 (disponible en GCC/Clang)"
      << std::endl;
  std::cout
      << "MSVC no soporta __int128, use tipos de 64 bits o Boost.Multiprecision"
      << std::endl;
  return 0;
}

#endif // defined(__SIZEOF_INT128__)