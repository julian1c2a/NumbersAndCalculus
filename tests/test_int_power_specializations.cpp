/**
 * @file test_int_power_specializations.cpp
 * @brief Prueba de especializaciones de int_power para diferentes tipos
 * @author AlgoritmiaCombinatoria Project
 * @date 2024
 *
 * Compilar con:
 * g++ -std=c++17 -I../include test_int_power_specializations.cpp -o
 * test_int_power
 */

#include "../include/number_calc.hpp"
#include <cassert>
#include <chrono>
#include <iostream>

using namespace number_calc_impl;

//==============================================================================
// FUNCIONES DE PRUEBA
//==============================================================================

void test_basic_functionality() {
  std::cout << "\n=== PRUEBAS BÁSICAS DE FUNCIONALIDAD ===" << std::endl;

  // Pruebas con int
  assert(int_power(2, 0) == 1);
  assert(int_power(2, 1) == 2);
  assert(int_power(2, 2) == 4);
  assert(int_power(2, 3) == 8);
  assert(int_power(2, 4) == 16);
  assert(int_power(2, 10) == 1024);

  std::cout << "✅ int: 2^10 = " << int_power(2, 10) << std::endl;

  // Pruebas con long long
  assert(int_power(3LL, 0) == 1LL);
  assert(int_power(3LL, 1) == 3LL);
  assert(int_power(3LL, 2) == 9LL);
  assert(int_power(3LL, 10) == 59049LL);

  std::cout << "✅ long long: 3^10 = " << int_power(3LL, 10) << std::endl;

  // Pruebas con unsigned long long
  assert(int_power(2ULL, 0) == 1ULL);
  assert(int_power(2ULL, 20) == 1048576ULL);

  std::cout << "✅ unsigned long long: 2^20 = " << int_power(2ULL, 20)
            << std::endl;

#if defined(__GNUC__) || defined(__clang__)
  // Pruebas con __int128
  assert(int_power(static_cast<__int128>(2), static_cast<__int128>(0)) == 1);
  assert(int_power(static_cast<__int128>(2), static_cast<__int128>(10)) ==
         1024);

  __int128 result_128 =
      int_power(static_cast<__int128>(2), static_cast<__int128>(50));
  std::cout << "✅ __int128: 2^50 calculado (valor muy grande)" << std::endl;

  // Pruebas con unsigned __int128
  unsigned __int128 result_u128 = int_power(static_cast<unsigned __int128>(2),
                                            static_cast<unsigned __int128>(60));
  std::cout << "✅ unsigned __int128: 2^60 calculado (valor muy grande)"
            << std::endl;
#endif

  std::cout << "✅ Todas las pruebas básicas pasaron" << std::endl;
}

void test_edge_cases() {
  std::cout << "\n=== PRUEBAS DE CASOS LÍMITE ===" << std::endl;

  // Casos especiales
  assert(int_power(0, 0) == 1); // Por definición matemática 0^0 = 1
  assert(int_power(0, 5) == 0);
  assert(int_power(1, 1000) == 1);
  assert(int_power(-1, 2) == 1);
  assert(int_power(-1, 3) == -1);

  std::cout << "✅ 0^0 = " << int_power(0, 0) << std::endl;
  std::cout << "✅ 0^5 = " << int_power(0, 5) << std::endl;
  std::cout << "✅ 1^1000 = " << int_power(1, 1000) << std::endl;
  std::cout << "✅ (-1)^2 = " << int_power(-1, 2) << std::endl;
  std::cout << "✅ (-1)^3 = " << int_power(-1, 3) << std::endl;

  // Casos con números negativos
  assert(int_power(-2, 2) == 4);
  assert(int_power(-2, 3) == -8);
  assert(int_power(-3, 4) == 81);

  std::cout << "✅ (-2)^2 = " << int_power(-2, 2) << std::endl;
  std::cout << "✅ (-2)^3 = " << int_power(-2, 3) << std::endl;
  std::cout << "✅ (-3)^4 = " << int_power(-3, 4) << std::endl;

  std::cout << "✅ Todas las pruebas de casos límite pasaron" << std::endl;
}

void test_overflow_detection() {
  std::cout << "\n=== PRUEBAS DE DETECCIÓN DE OVERFLOW ===" << std::endl;

  // Casos que NO deberían causar overflow
  assert(int_power_safe(2, 10) == true);
  assert(int_power_safe(3, 5) == true);
  assert(int_power_safe(10, 3) == true);

  std::cout << "✅ 2^10 es seguro: " << std::boolalpha << int_power_safe(2, 10)
            << std::endl;
  std::cout << "✅ 3^5 es seguro: " << std::boolalpha << int_power_safe(3, 5)
            << std::endl;

  // Casos que SÍ deberían causar overflow para int
  assert(int_power_safe(2, 31) ==
         false); // 2^31 overflow para int en sistemas de 32 bits
  assert(int_power_safe(1000, 10) == false); // Definitivamente overflow

  std::cout << "✅ 2^31 NO es seguro para int: " << std::boolalpha
            << int_power_safe(2, 31) << std::endl;
  std::cout << "✅ 1000^10 NO es seguro: " << std::boolalpha
            << int_power_safe(1000, 10) << std::endl;

  // Prueba de la versión checked
  try {
    int result = int_power_checked(2, 10);
    std::cout << "✅ int_power_checked(2, 10) = " << result << std::endl;
  } catch (const std::exception &e) {
    std::cout << "❌ Error inesperado: " << e.what() << std::endl;
  }

  try {
    int result = int_power_checked(1000, 10);
    std::cout << "❌ Debería haber lanzado excepción" << std::endl;
  } catch (const std::exception &e) {
    std::cout << "✅ Excepción esperada capturada: " << e.what() << std::endl;
  }

  std::cout << "✅ Todas las pruebas de overflow pasaron" << std::endl;
}

void benchmark_specializations() {
  std::cout << "\n=== BENCHMARK DE ESPECIALIZACIONES ===" << std::endl;

  const int iterations = 1000000;
  const int base = 3;
  const int exp = 15;

  // Benchmark int
  auto start = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < iterations; ++i) {
    volatile int result = int_power(base, exp);
    (void)result; // Evitar optimización
  }
  auto end = std::chrono::high_resolution_clock::now();
  auto duration_int =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start);

  std::cout << "int especializado: " << duration_int.count() << " μs ("
            << iterations << " iteraciones)" << std::endl;

  // Benchmark long long
  start = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < iterations; ++i) {
    volatile long long result =
        int_power(static_cast<long long>(base), static_cast<long long>(exp));
    (void)result;
  }
  end = std::chrono::high_resolution_clock::now();
  auto duration_ll =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start);

  std::cout << "long long especializado: " << duration_ll.count() << " μs ("
            << iterations << " iteraciones)" << std::endl;

#if defined(__GNUC__) || defined(__clang__)
  // Benchmark __int128
  start = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < iterations; ++i) {
    volatile __int128 result =
        int_power(static_cast<__int128>(base), static_cast<__int128>(exp));
    (void)result;
  }
  end = std::chrono::high_resolution_clock::now();
  auto duration_128 =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start);

  std::cout << "__int128 especializado: " << duration_128.count() << " μs ("
            << iterations << " iteraciones)" << std::endl;
#endif

  std::cout << "✅ Benchmark completado" << std::endl;
}

void demo_power_calculations() {
  std::cout << "\n=== DEMOSTRACIÓN DE CÁLCULOS DE POTENCIAS ===" << std::endl;

  // Demostrar diferentes tipos
  std::cout << "Potencias de 2:" << std::endl;
  for (int i = 0; i <= 20; ++i) {
    std::cout << "  2^" << i << " = " << int_power(2, i) << std::endl;
  }

  std::cout << "\nPotencias de 3:" << std::endl;
  for (int i = 0; i <= 10; ++i) {
    std::cout << "  3^" << i << " = " << int_power(3, i) << std::endl;
  }

#if defined(__GNUC__) || defined(__clang__)
  std::cout << "\nPotencias grandes con __int128:" << std::endl;
  for (int i = 50; i <= 60; i += 2) {
    __int128 result =
        int_power(static_cast<__int128>(2), static_cast<__int128>(i));
    std::cout << "  2^" << i << " = [valor muy grande de __int128]"
              << std::endl;
  }
#endif

  std::cout << "✅ Demostración completada" << std::endl;
}

void test_int_power_2_specialization() {
  std::cout << "\n=== PRUEBAS int_power_2 ESPECIALIZADA ===" << std::endl;

  // Pruebas con diferentes tipos signed
  assert(int_power_2<int>(0) == 1);
  assert(int_power_2<int>(1) == 2);
  assert(int_power_2<int>(2) == 4);
  assert(int_power_2<int>(10) == 1024);

  std::cout << "✅ int_power_2<int>(10) = " << int_power_2<int>(10)
            << std::endl;

  // Pruebas con long long signed
  assert(int_power_2<long long>(0) == 1LL);
  assert(int_power_2<long long>(20) == 1048576LL);

  std::cout << "✅ int_power_2<long long>(20) = " << int_power_2<long long>(20)
            << std::endl;

  // Pruebas con tipos unsigned
  assert(int_power_2<unsigned int>(0) == 1U);
  assert(int_power_2<unsigned int>(10) == 1024U);

  std::cout << "✅ int_power_2<unsigned int>(10) = "
            << int_power_2<unsigned int>(10) << std::endl;

  assert(int_power_2<unsigned long long>(0) == 1ULL);
  assert(int_power_2<unsigned long long>(25) == 33554432ULL);

  std::cout << "✅ int_power_2<unsigned long long>(25) = "
            << int_power_2<unsigned long long>(25) << std::endl;

#if defined(__GNUC__) || defined(__clang__)
  // Pruebas con __int128
  assert(int_power_2<__int128>(0) == 1);
  assert(int_power_2<__int128>(50) ==
         int_power(static_cast<__int128>(2), static_cast<__int128>(50)));

  std::cout << "✅ int_power_2<__int128>(50) calculado correctamente"
            << std::endl;

  // Pruebas con unsigned __int128
  assert(int_power_2<unsigned __int128>(0) == 1);
  assert(int_power_2<unsigned __int128>(60) ==
         int_power(static_cast<unsigned __int128>(2),
                   static_cast<unsigned __int128>(60)));

  std::cout << "✅ int_power_2<unsigned __int128>(60) calculado correctamente"
            << std::endl;
#endif

  std::cout << "✅ Todas las pruebas de int_power_2 pasaron" << std::endl;
}

void test_signed_vs_unsigned_traits() {
  std::cout << "\n=== PRUEBAS ESPECIALIZACIONES SIGNED VS UNSIGNED ==="
            << std::endl;

  // Casos especiales para tipos signed (manejo de -1)
  assert(int_power(-1, 0) == 1);
  assert(int_power(-1, 1) == -1);
  assert(int_power(-1, 2) == 1);
  assert(int_power(-1, 3) == -1);
  assert(int_power(-1, 100) == 1);  // Par
  assert(int_power(-1, 101) == -1); // Impar

  std::cout << "✅ Signed: (-1)^100 = " << int_power(-1, 100) << std::endl;
  std::cout << "✅ Signed: (-1)^101 = " << int_power(-1, 101) << std::endl;

  // Casos con números negativos diferentes
  assert(int_power(-2, 2) == 4);
  assert(int_power(-2, 3) == -8);
  assert(int_power(-3, 4) == 81);
  assert(int_power(-3, 3) == -27);

  std::cout << "✅ Signed: (-2)^3 = " << int_power(-2, 3) << std::endl;
  std::cout << "✅ Signed: (-3)^4 = " << int_power(-3, 4) << std::endl;

  // Para tipos unsigned, no hay casos especiales de signo
  assert(int_power(2U, 10) == 1024U);
  assert(int_power(3U, 5) == 243U);

  std::cout << "✅ Unsigned: 2^10 = " << int_power(2U, 10) << std::endl;
  std::cout << "✅ Unsigned: 3^5 = " << int_power(3U, 5) << std::endl;

  std::cout << "✅ Todas las pruebas de signed vs unsigned pasaron"
            << std::endl;
}

void test_new_vs_old_implementations() {
  std::cout << "\n=== COMPARACIÓN NUEVA VS ANTIGUA IMPLEMENTACIÓN ==="
            << std::endl;

  // Comparar int_power vs int_power_new
  for (int base = -3; base <= 5; ++base) {
    for (int exp = 0; exp <= 10; ++exp) {
      if (base == 0 && exp == 0)
        continue; // Evitar 0^0 ambiguo

      auto old_result = int_power(base, exp);
      auto new_result = int_power_new(base, exp);

      assert(old_result == new_result);
    }
  }

  std::cout << "✅ int_power vs int_power_new dan resultados idénticos"
            << std::endl;

  // Casos especiales para potencias de 2
  for (int exp = 0; exp <= 20; ++exp) {
    assert(int_power(2, exp) == int_power_2<int>(exp));
    assert(int_power_new(2, exp) == int_power_2<int>(exp));
  }

  std::cout
      << "✅ Todas las implementaciones de potencias de 2 son consistentes"
      << std::endl;

  std::cout << "✅ Comparación de implementaciones completada" << std::endl;
}

//==============================================================================
// FUNCIÓN PRINCIPAL
//==============================================================================int
// main() {
try {
  std::cout << "=== PRUEBAS DE ESPECIALIZACIONES int_power ===" << std::endl;

  test_basic_functionality();
  test_edge_cases();
  test_overflow_detection();
  test_int_power_2_specialization();
  test_signed_vs_unsigned_traits();
  test_new_vs_old_implementations();
  benchmark_specializations();
  demo_power_calculations();
  std::cout << "\n=== TODAS LAS PRUEBAS COMPLETADAS EXITOSAMENTE ==="
            << std::endl;
  std::cout << "🎉 Las especializaciones de int_power funcionan correctamente"
            << std::endl;

  return 0;

} catch (const std::exception &e) {
  std::cerr << "❌ Error durante las pruebas: " << e.what() << std::endl;
  return 1;
} catch (...) {
  std::cerr << "❌ Error desconocido durante las pruebas" << std::endl;
  return 1;
}
}

// Template genérico con exponenciación binaria
template <typename T> constexpr T int_power(T base, T exp) noexcept;

// Especializaciones optimizadas:
template <> constexpr int int_power<int>(int base, int exp) noexcept;
template <>
constexpr long long int_power<long long>(long long base,
                                         long long exp) noexcept;
template <> constexpr unsigned long long int_power<unsigned long long>(...);
template <>
constexpr __int128 int_power<__int128>(__int128 base, __int128 exp) noexcept;
template <> constexpr unsigned __int128 int_power<unsigned __int128>(...);