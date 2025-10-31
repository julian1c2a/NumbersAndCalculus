/**
 * @file test_int128_support.cpp
 * @brief Test específico para verificar soporte de __int128 con traits
 * @author AlgoritmiaCombinatoria Project
 * @date 2024
 */

#include "../include/number_calc.hpp"
#include <cassert>
#include <iostream>


using namespace number_calc_impl;

void test_int128_basic() {
  std::cout << "\n=== TEST __int128 BÁSICO ===" << std::endl;

  // Tests básicos con __int128
  __int128 result1 =
      int_power(static_cast<__int128>(2), static_cast<__int128>(10));
  assert(result1 == 1024);

  __int128 result2 =
      int_power(static_cast<__int128>(3), static_cast<__int128>(5));
  assert(result2 == 243);

  std::cout << "✅ __int128 basic tests completed" << std::endl;
}

void test_uint128_basic() {
  std::cout << "\n=== TEST unsigned __int128 BÁSICO ===" << std::endl;

  // Tests básicos con unsigned __int128
  unsigned __int128 result1 = int_power(static_cast<unsigned __int128>(2),
                                        static_cast<unsigned __int128>(10));
  assert(result1 == 1024);

  unsigned __int128 result2 = int_power(static_cast<unsigned __int128>(3),
                                        static_cast<unsigned __int128>(5));
  assert(result2 == 243);

  std::cout << "✅ unsigned __int128 basic tests completed" << std::endl;
}

void test_int128_signed_specialization() {
  std::cout << "\n=== TEST __int128 ESPECIALIZACIÓN SIGNED ===" << std::endl;

  // Test de especialización signed con __int128
  __int128 result1 = int_power_signed_impl(static_cast<__int128>(-1), 0);
  assert(result1 == 1);

  __int128 result2 = int_power_signed_impl(static_cast<__int128>(-1), 2);
  assert(result2 == 1); // Par

  __int128 result3 = int_power_signed_impl(static_cast<__int128>(-1), 3);
  assert(result3 == -1); // Impar

  std::cout << "✅ __int128 signed specialization working" << std::endl;
}

void test_uint128_unsigned_specialization() {
  std::cout << "\n=== TEST unsigned __int128 ESPECIALIZACIÓN UNSIGNED ==="
            << std::endl;

  // Test de especialización unsigned con unsigned __int128
  unsigned __int128 result1 =
      int_power_unsigned_impl(static_cast<unsigned __int128>(2), 10);
  assert(result1 == 1024);

  unsigned __int128 result2 =
      int_power_unsigned_impl(static_cast<unsigned __int128>(5), 3);
  assert(result2 == 125);

  std::cout << "✅ unsigned __int128 unsigned specialization working"
            << std::endl;
}

void test_int128_dispatch() {
  std::cout << "\n=== TEST __int128 DISPATCH AUTOMÁTICO ===" << std::endl;

  // Test de dispatch automático
  __int128 signed_result = int_power_dispatch(static_cast<__int128>(-2), 4);
  assert(signed_result == 16);

  unsigned __int128 unsigned_result =
      int_power_dispatch(static_cast<unsigned __int128>(2), 20);
  assert(unsigned_result == 1048576);

  std::cout << "✅ __int128 automatic dispatch working" << std::endl;
}

void test_int128_power_of_2() {
  std::cout << "\n=== TEST __int128 POTENCIAS DE 2 ===" << std::endl;

  // Test de optimización para potencias de 2
  __int128 result1 = int_power_2<__int128>(10);
  assert(result1 == 1024);

  unsigned __int128 result2 = int_power_2<unsigned __int128>(20);
  assert(result2 == 1048576);

  std::cout << "✅ __int128 power of 2 optimization working" << std::endl;
}

void test_int128_new_function() {
  std::cout << "\n=== TEST __int128 FUNCIÓN NUEVA ===" << std::endl;

  // Test de función nueva con detección automática
  __int128 result1 = int_power_new(static_cast<__int128>(2), 15);
  assert(result1 == 32768);

  __int128 result2 = int_power_new(static_cast<__int128>(3), 7);
  assert(result2 == 2187);

  std::cout << "✅ __int128 new function with auto-detection working"
            << std::endl;
}

void test_trait_detection() {
  std::cout << "\n=== TEST DETECCIÓN DE TRAITS ===" << std::endl;

  // Verificar que los traits funcionan correctamente
  static_assert(is_integral_extended_v<__int128>,
                "__int128 should be detected as integral");
  static_assert(is_integral_extended_v<unsigned __int128>,
                "unsigned __int128 should be detected as integral");
  static_assert(is_signed_extended_v<__int128>,
                "__int128 should be detected as signed");
  static_assert(is_unsigned_extended_v<unsigned __int128>,
                "unsigned __int128 should be detected as unsigned");
  static_assert(!is_signed_extended_v<unsigned __int128>,
                "unsigned __int128 should NOT be detected as signed");
  static_assert(!is_unsigned_extended_v<__int128>,
                "__int128 should NOT be detected as unsigned");

  std::cout << "✅ Trait detection working correctly" << std::endl;
}

int main() {
  try {
    std::cout << "=== TEST SOPORTE COMPLETO __int128 CON TRAITS ==="
              << std::endl;

    test_trait_detection();
    test_int128_basic();
    test_uint128_basic();
    test_int128_signed_specialization();
    test_uint128_unsigned_specialization();
    test_int128_dispatch();
    test_int128_power_of_2();
    test_int128_new_function();

    std::cout << "\n=== RESUMEN DE FUNCIONALIDADES ===" << std::endl;
    std::cout << "✅ Soporte completo para __int128 y unsigned __int128"
              << std::endl;
    std::cout << "✅ Traits extendidos funcionando correctamente" << std::endl;
    std::cout << "✅ Especializaciones automáticas por signed/unsigned"
              << std::endl;
    std::cout << "✅ Optimizaciones para potencias de 2" << std::endl;
    std::cout << "✅ Sistema de dispatch automático" << std::endl;
    std::cout
        << "✅ Compatibilidad con sintaxis template<typename T, typename U>"
        << std::endl;

    std::cout << "\n🎉 TODOS LOS TESTS __int128 COMPLETADOS EXITOSAMENTE"
              << std::endl;

    return 0;

  } catch (const std::exception &e) {
    std::cerr << "❌ Error durante los tests: " << e.what() << std::endl;
    return 1;
  } catch (...) {
    std::cerr << "❌ Error desconocido durante los tests" << std::endl;
    return 1;
  }
}