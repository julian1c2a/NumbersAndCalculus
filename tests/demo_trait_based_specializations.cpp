/**
 * @file demo_trait_based_specializations.cpp
 * @brief Demostración de especializaciones basadas en traits signed/unsigned
 * @author AlgoritmiaCombinatoria Project
 * @date 2024
 *
 * Este archivo demuestra las ventajas de usar especializaciones basadas en
 * traits en lugar de especializaciones por tipo específico.
 *
 * Compilar con:
 * g++ -std=c++20 -I../include demo_trait_based_specializations.cpp -o
 * demo_traits
 */

#include "../include/number_calc.hpp"
#include <chrono>
#include <iostream>
#include <type_traits>


using namespace number_calc_impl;

//==============================================================================
// FUNCIONES DE DEMOSTRACIÓN
//==============================================================================

template <typename T> void demo_type_characteristics() {
  std::cout << "\n--- Características del tipo " << typeid(T).name() << " ---"
            << std::endl;

  std::cout << "Tamaño: " << sizeof(T) << " bytes" << std::endl;
  std::cout << "Es signed: " << std::boolalpha
            << std::is_signed_v<T> << std::endl;
  std::cout << "Es unsigned: " << std::boolalpha
            << std::is_unsigned_v<T> << std::endl;

  // Demostrar optimizaciones específicas
  if constexpr (std::is_signed_v<T>) {
    std::cout << "Usando especialización SIGNED" << std::endl;
    std::cout << "(-1)^2 = " << int_power(T{-1}, T{2}) << std::endl;
    std::cout << "(-1)^3 = " << int_power(T{-1}, T{3}) << std::endl;
    std::cout << "(-2)^4 = " << int_power(T{-2}, T{4}) << std::endl;
  } else {
    std::cout << "Usando especialización UNSIGNED" << std::endl;
    std::cout << "2^10 = " << int_power(T{2}, T{10}) << std::endl;
    std::cout << "3^5 = " << int_power(T{3}, T{5}) << std::endl;
  }

  // Demostrar int_power_2
  std::cout << "int_power_2<" << typeid(T).name()
            << ">(10) = " << int_power_2<T>(10) << std::endl;

  // Verificar que int_power normal detecta potencias de 2
  auto normal_result = int_power(T{2}, T{15});
  auto power2_result = int_power_2<T>(15);
  auto new_result = int_power_new(T{2}, T{15});

  std::cout << "2^15 (int_power): " << normal_result << std::endl;
  std::cout << "2^15 (int_power_2): " << power2_result << std::endl;
  std::cout << "2^15 (int_power_new): " << new_result << std::endl;
  std::cout << "¿Todos iguales? " << std::boolalpha
            << (normal_result == power2_result && power2_result == new_result)
            << std::endl;
}

void demo_all_standard_types() {
  std::cout << "=== DEMOSTRACIÓN ESPECIALIZACIONES POR TRAITS ===" << std::endl;

  // Tipos signed estándar
  demo_type_characteristics<int>();
  demo_type_characteristics<long>();
  demo_type_characteristics<long long>();

  // Tipos unsigned estándar
  demo_type_characteristics<unsigned int>();
  demo_type_characteristics<unsigned long>();
  demo_type_characteristics<unsigned long long>();

#if defined(__GNUC__) || defined(__clang__)
  // Tipos extendidos de compilador
  demo_type_characteristics<__int128>();
  demo_type_characteristics<unsigned __int128>();
#endif

  // Tipos de tamaño específico
  demo_type_characteristics<int8_t>();
  demo_type_characteristics<uint8_t>();
  demo_type_characteristics<int16_t>();
  demo_type_characteristics<uint16_t>();
  demo_type_characteristics<int32_t>();
  demo_type_characteristics<uint32_t>();
  demo_type_characteristics<int64_t>();
  demo_type_characteristics<uint64_t>();
}

void benchmark_trait_vs_specific_specializations() {
  std::cout << "\n=== BENCHMARK: TRAITS VS ESPECIALIZACIONES ESPECÍFICAS ==="
            << std::endl;

  const int iterations = 1000000;
  const int base = 3;
  const int exp = 12;

  // Benchmark con int usando especialización por traits
  auto start = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < iterations; ++i) {
    volatile int result = int_power_new(base, exp);
    (void)result;
  }
  auto end = std::chrono::high_resolution_clock::now();
  auto duration_traits =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start);

  std::cout << "Especialización por traits: " << duration_traits.count()
            << " μs" << std::endl;

  // Benchmark con int usando especialización original
  start = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < iterations; ++i) {
    volatile int result = int_power(base, exp);
    (void)result;
  }
  end = std::chrono::high_resolution_clock::now();
  auto duration_specific =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start);

  std::cout << "Especialización específica: " << duration_specific.count()
            << " μs" << std::endl;

  // Benchmark específico para potencias de 2
  start = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < iterations; ++i) {
    volatile int result = int_power_2<int>(exp);
    (void)result;
  }
  end = std::chrono::high_resolution_clock::now();
  auto duration_power2 =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start);

  std::cout << "int_power_2 especializada: " << duration_power2.count() << " μs"
            << std::endl;

  std::cout << "Mejora traits vs specific: "
            << (duration_specific.count() > duration_traits.count() ? "+" : "")
            << (duration_traits.count() - duration_specific.count()) << " μs"
            << std::endl;

  std::cout << "Mejora power2 vs generic: "
            << (duration_traits.count() > duration_power2.count() ? "+" : "")
            << (duration_power2.count() - duration_traits.count()) << " μs"
            << std::endl;
}

void demo_edge_cases_by_sign() {
  std::cout << "\n=== CASOS LÍMITE POR TIPO DE SIGNO ===" << std::endl;

  std::cout << "\n--- Tipos SIGNED ---" << std::endl;

  // Casos especiales con -1 (solo para signed)
  std::cout << "(-1)^0 = " << int_power(-1, 0) << std::endl;
  std::cout << "(-1)^100 = " << int_power(-1, 100) << " (par)" << std::endl;
  std::cout << "(-1)^101 = " << int_power(-1, 101) << " (impar)" << std::endl;

  // Casos con otros números negativos
  std::cout << "(-2)^8 = " << int_power(-2, 8) << std::endl;
  std::cout << "(-3)^5 = " << int_power(-3, 5) << std::endl;

  std::cout << "\n--- Tipos UNSIGNED ---" << std::endl;

  // Para unsigned, no hay casos especiales de signo
  std::cout << "Máximas potencias seguras:" << std::endl;
  std::cout << "uint8_t: 2^7 = "
            << int_power(static_cast<uint8_t>(2), static_cast<uint8_t>(7))
            << std::endl;
  std::cout << "uint16_t: 2^15 = "
            << int_power(static_cast<uint16_t>(2), static_cast<uint16_t>(15))
            << std::endl;
  std::cout << "uint32_t: 2^31 = " << int_power(2U, 31U) << std::endl;
  std::cout << "uint64_t: 2^63 = " << int_power(2ULL, 63ULL) << std::endl;
}

void demo_constexpr_evaluation() {
  std::cout << "\n=== EVALUACIÓN CONSTEXPR ===" << std::endl;

  // Estas se evalúan en tiempo de compilación
  constexpr int power2_10 = int_power_2<int>(10);
  constexpr int power_normal = int_power(3, 5);
  constexpr int power_new = int_power_new(2, 8);

  std::cout << "constexpr int_power_2<int>(10) = " << power2_10 << std::endl;
  std::cout << "constexpr int_power(3, 5) = " << power_normal << std::endl;
  std::cout << "constexpr int_power_new(2, 8) = " << power_new << std::endl;

  // Verificar que se pueden usar en contextos constexpr
  static_assert(int_power_2<int>(10) == 1024);
  static_assert(int_power(3, 5) == 243);
  static_assert(int_power_new(2, 8) == 256);

  std::cout << "✅ Todas las funciones son correctamente constexpr"
            << std::endl;
}

//==============================================================================
// FUNCIÓN PRINCIPAL
//==============================================================================

int main() {
  try {
    std::cout << "=== DEMOSTRACIÓN ESPECIALIZACIONES BASADAS EN TRAITS ==="
              << std::endl;

    demo_all_standard_types();
    benchmark_trait_vs_specific_specializations();
    demo_edge_cases_by_sign();
    demo_constexpr_evaluation();

    std::cout << "\n=== VENTAJAS DE LAS ESPECIALIZACIONES POR TRAITS ==="
              << std::endl;
    std::cout << "✅ Una sola implementación para todos los tipos signed"
              << std::endl;
    std::cout << "✅ Una sola implementación para todos los tipos unsigned"
              << std::endl;
    std::cout << "✅ Optimizaciones específicas por características del tipo"
              << std::endl;
    std::cout << "✅ Función int_power_2 altamente optimizada para base=2"
              << std::endl;
    std::cout << "✅ Código más mantenible y extensible" << std::endl;
    std::cout << "✅ Detección automática de optimizaciones (potencias de 2)"
              << std::endl;
    std::cout << "✅ Compatibilidad total con evaluación constexpr"
              << std::endl;

    std::cout << "\n🎉 DEMOSTRACIÓN COMPLETADA EXITOSAMENTE" << std::endl;

    return 0;

  } catch (const std::exception &e) {
    std::cerr << "❌ Error durante la demostración: " << e.what() << std::endl;
    return 1;
  } catch (...) {
    std::cerr << "❌ Error desconocido durante la demostración" << std::endl;
    return 1;
  }
}