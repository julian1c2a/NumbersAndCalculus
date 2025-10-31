#include "number_calc.hpp"
#include <cassert>
#include <iostream>

/**
 * @file test_modular_architecture.cpp
 * @brief Programa de prueba para la nueva arquitectura modular
 */

int main() {
  std::cout << "=== PRUEBAS DE LA ARQUITECTURA MODULAR ===\n\n";

  using namespace number_calc;

  // 1. Prueba de la función inteligente con detección automática
  std::cout << "1. Función int_power_smart (detección automática):\n";

  // Potencia de 2 (debería usar optimización bit-shifting)
  int result_2_10 = int_power_smart(2, 10);
  std::cout << "   2^10 = " << result_2_10 << " (optimización 2^n)\n";
  assert(result_2_10 == 1024);

  // Base no potencia de 2 (debería usar dispatch por traits)
  int result_3_5 = int_power_smart(3, 5);
  std::cout << "   3^5 = " << result_3_5 << " (dispatch por traits)\n";
  assert(result_3_5 == 243);

  // 2. Prueba de optimización específica para potencias de 2
  std::cout << "\n2. Función int_power_2 (especializada):\n";
  auto result_2_20 = int_power_2<int>(20);
  std::cout << "   2^20 = " << result_2_20 << "\n";
  assert(result_2_20 == 1048576);

  // 3. Prueba de funciones básicas
  std::cout << "\n3. Funciones básicas:\n";
  int result_basic = int_power_basic(5, 3);
  std::cout << "   int_power_basic(5, 3) = " << result_basic << "\n";
  assert(result_basic == 125);

  // 4. Prueba de detección de potencias de 2
  std::cout << "\n4. Análisis de optimización:\n";

  int test_values[] = {1, 2, 4, 8, 16, 17, 32, 33};
  for (int val : test_values) {
    bool is_pow2 = is_power_of_2(val);
    int exp = find_power_of_2_exponent(val);
    std::cout << "   " << val
              << " es potencia de 2: " << (is_pow2 ? "SÍ" : "NO");
    if (is_pow2) {
      std::cout << " (2^" << exp << ")";
    }
    std::cout << "\n";
  }

  // 5. Prueba de tipos signed vs unsigned
  std::cout << "\n5. Dispatch signed/unsigned:\n";

  // Signed
  int signed_result = int_power_smart(-2, 3);
  std::cout << "   (-2)^3 = " << signed_result << " (signed)\n";
  assert(signed_result == -8);

  // Unsigned
  unsigned int unsigned_result = int_power_smart(2u, 5);
  std::cout << "   2u^5 = " << unsigned_result << " (unsigned)\n";
  assert(unsigned_result == 32u);

  // 6. Prueba con función de verificación
  std::cout << "\n6. Verificación de overflow:\n";

  auto [result_safe, success] = int_power_safe_pair(2, 30);
  if (success) {
    std::cout << "   2^30 = " << result_safe << " (operación segura)\n";
  } else {
    std::cout << "   2^30 causaría overflow\n";
  }
  std::cout << "\n=== TODAS LAS PRUEBAS PASARON EXITOSAMENTE ===\n";
  std::cout << "La nueva arquitectura modular funciona correctamente!\n\n";

  // 7. Demostración de optimización automática
  std::cout << "7. Demostración de optimización automática:\n";
  int bases[] = {2, 3, 4, 5, 8, 9, 16, 17};
  for (int base : bases) {
    int result = int_power_smart(base, 10);
    bool optimized = is_power_of_2(base);
    std::cout << "   " << base << "^10 = " << result;
    if (optimized) {
      std::cout << " (optimizada con bit-shifting)";
    } else {
      std::cout << " (dispatch por traits)";
    }
    std::cout << "\n";
  }

  return 0;
}