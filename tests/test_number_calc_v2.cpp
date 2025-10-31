/**
 * @file test_number_calc_v2.cpp
 * @brief Test de la versión corregida con especializaciones por traits
 * @author AlgoritmiaCombinatoria Project
 * @date 2024
 */

#include "../include/number_calc_v2.hpp"
#include <cassert>
#include <chrono>
#include <iostream>


using namespace number_calc_impl;

void test_basic_functionality() {
  std::cout << "\n=== TEST FUNCIONALIDAD BÁSICA ===" << std::endl;

  // Tests básicos
  assert(int_power(2, 0) == 1);
  assert(int_power(2, 1) == 2);
  assert(int_power(2, 10) == 1024);
  assert(int_power(3, 5) == 243);
  assert(int_power(5, 3) == 125);

  std::cout << "✅ Tests básicos completados" << std::endl;
}

void test_signed_specialization() {
  std::cout << "\n=== TEST ESPECIALIZACIÓN SIGNED ===" << std::endl;

  // Tests específicos para signed
  assert(int_power_signed_impl(-1, 0) == 1);
  assert(int_power_signed_impl(-1, 2) == 1);  // Par
  assert(int_power_signed_impl(-1, 3) == -1); // Impar
  assert(int_power_signed_impl(-2, 4) == 16);
  assert(int_power_signed_impl(-3, 3) == -27);

  std::cout << "✅ Tests signed completados" << std::endl;
}

void test_unsigned_specialization() {
  std::cout << "\n=== TEST ESPECIALIZACIÓN UNSIGNED ===" << std::endl;

  // Tests específicos para unsigned
  assert(int_power_unsigned_impl(2U, 10U) == 1024U);
  assert(int_power_unsigned_impl(3U, 5U) == 243U);
  assert(int_power_unsigned_impl(5U, 3U) == 125U);

  std::cout << "✅ Tests unsigned completados" << std::endl;
}

void test_dispatch_system() {
  std::cout << "\n=== TEST SISTEMA DE DISPATCH ===" << std::endl;

  // Verificar que dispatch funciona correctamente
  int signed_result = int_power_dispatch(-2, 4);
  unsigned int unsigned_result = int_power_dispatch(2U, 10U);

  assert(signed_result == 16);
  assert(unsigned_result == 1024U);

  std::cout << "✅ Sistema de dispatch funcionando" << std::endl;
}

void test_power_of_2_optimization() {
  std::cout << "\n=== TEST OPTIMIZACIÓN POTENCIAS DE 2 ===" << std::endl;

  // Tests de int_power_2
  assert(int_power_2<int>(0) == 1);
  assert(int_power_2<int>(1) == 2);
  assert(int_power_2<int>(10) == 1024);
  assert(int_power_2<long long>(20) == 1048576LL);

  std::cout << "✅ int_power_2<int>(10) = " << int_power_2<int>(10)
            << std::endl;
  std::cout << "✅ int_power_2<long long>(20) = " << int_power_2<long long>(20)
            << std::endl;

  std::cout << "✅ Optimización potencias de 2 funcionando" << std::endl;
}

void test_new_function() {
  std::cout << "\n=== TEST FUNCIÓN int_power_new ===" << std::endl;

  // Verificar detección automática de potencias de 2
  assert(int_power_new(2, 10) == int_power_2<int>(10));
  assert(int_power_new(3, 5) == int_power_dispatch(3, 5));

  std::cout << "✅ Detección automática funcionando" << std::endl;
}

void test_safety_functions() {
  std::cout << "\n=== TEST FUNCIONES DE SEGURIDAD ===" << std::endl;

  // Tests de seguridad
  assert(int_power_safe(2, 10) == true);
  assert(int_power_safe(3, 5) == true);
  assert(int_power_safe(-1, 100) == true);

  std::cout << "✅ Funciones de seguridad funcionando" << std::endl;
}

void benchmark_implementations() {
  std::cout << "\n=== BENCHMARK IMPLEMENTACIONES ===" << std::endl;

  const int iterations = 1000000;
  const int base = 3, exp = 12;

  // Benchmark implementación básica
  auto start = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < iterations; ++i) {
    volatile int result = int_power(base, exp);
    (void)result;
  }
  auto end = std::chrono::high_resolution_clock::now();
  auto duration_basic =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start);

  // Benchmark dispatch por traits
  start = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < iterations; ++i) {
    volatile int result = int_power_dispatch(base, exp);
    (void)result;
  }
  end = std::chrono::high_resolution_clock::now();
  auto duration_dispatch =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start);

  // Benchmark int_power_2
  start = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < iterations; ++i) {
    volatile int result = int_power_2<int>(exp);
    (void)result;
  }
  end = std::chrono::high_resolution_clock::now();
  auto duration_power2 =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start);

  std::cout << "Implementación básica: " << duration_basic.count() << " μs"
            << std::endl;
  std::cout << "Dispatch por traits: " << duration_dispatch.count() << " μs"
            << std::endl;
  std::cout << "int_power_2 especializada: " << duration_power2.count() << " μs"
            << std::endl;
}

void demo_constexpr_evaluation() {
  std::cout << "\n=== DEMO EVALUACIÓN CONSTEXPR ===" << std::endl;

  // Estas se evalúan en tiempo de compilación
  constexpr int power2_10 = int_power_2<int>(10);
  constexpr int power_dispatch = int_power_dispatch(3, 5);
  constexpr int power_new = int_power_new(2, 8);

  std::cout << "constexpr int_power_2<int>(10) = " << power2_10 << std::endl;
  std::cout << "constexpr int_power_dispatch(3, 5) = " << power_dispatch
            << std::endl;
  std::cout << "constexpr int_power_new(2, 8) = " << power_new << std::endl;

  // Verificar con static_assert
  static_assert(int_power_2<int>(10) == 1024);
  static_assert(int_power_dispatch(3, 5) == 243);
  static_assert(int_power_new(2, 8) == 256);

  std::cout << "✅ Todas las funciones son correctamente constexpr"
            << std::endl;
}

int main() {
  try {
    std::cout << "=== TEST VERSIÓN CORREGIDA CON TRAITS ===" << std::endl;

    test_basic_functionality();
    test_signed_specialization();
    test_unsigned_specialization();
    test_dispatch_system();
    test_power_of_2_optimization();
    test_new_function();
    test_safety_functions();
    benchmark_implementations();
    demo_constexpr_evaluation();

    std::cout << "\n=== VENTAJAS DE LA VERSIÓN CORREGIDA ===" << std::endl;
    std::cout << "✅ Usa sintaxis estándar template<typename T, typename U>"
              << std::endl;
    std::cout << "✅ Especializaciones automáticas con std::is_signed_v"
              << std::endl;
    std::cout << "✅ Sistema de dispatch por traits completamente funcional"
              << std::endl;
    std::cout << "✅ Optimización int_power_2 para potencias de base 2"
              << std::endl;
    std::cout << "✅ Detección automática de optimizaciones disponibles"
              << std::endl;
    std::cout << "✅ Funciones de seguridad para prevenir overflow"
              << std::endl;
    std::cout << "✅ Evaluación constexpr completa" << std::endl;
    std::cout << "✅ Compatibilidad garantizada con C++17/C++20" << std::endl;

    std::cout << "\n🎉 TODOS LOS TESTS COMPLETADOS EXITOSAMENTE" << std::endl;

    return 0;

  } catch (const std::exception &e) {
    std::cerr << "❌ Error durante los tests: " << e.what() << std::endl;
    return 1;
  } catch (...) {
    std::cerr << "❌ Error desconocido durante los tests" << std::endl;
    return 1;
  }
}