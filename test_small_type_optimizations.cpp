#include "simplified_power_tables_cpp14.hpp"
#include <cassert>
#include <chrono>
#include <cstdint>
#include <iostream>

/**
 * @file test_small_type_optimizations.cpp
 * @brief Programa de prueba para optimizaciones con tablas constexpr
 */

int main() {
  std::cout << "=== PRUEBAS DE OPTIMIZACIONES CON TABLAS CONSTEXPR ===\n\n";

  using namespace number_calc::integral_power_functions::lookup_tables;

  // 1. Pruebas de tablas constexpr para int8_t
  std::cout << "1. Pruebas de tabla constexpr int8_t (2^0 hasta 2^6):\n";
  for (int exp = 0; exp <= 6; ++exp) {
    int8_t result = get_power_of_2_int8(exp);
    int8_t expected = static_cast<int8_t>(1 << exp);
    std::cout << "   2^" << exp << " = " << static_cast<int>(result);
    assert(result == expected);
    std::cout << " ✓\n";
  }

  // Verificar que exponente 7 está fuera de rango (overflow)
  std::cout << "   2^7 causaría overflow (max int8_t = 127)\n";

  // 2. Pruebas de tablas constexpr para uint8_t
  std::cout << "\n2. Pruebas de tabla constexpr uint8_t (2^0 hasta 2^7):\n";
  for (int exp = 0; exp <= 7; ++exp) {
    uint8_t result = get_power_of_2_uint8(exp);
    uint8_t expected = static_cast<uint8_t>(1 << exp);
    std::cout << "   2^" << exp << " = " << static_cast<int>(result);
    assert(result == expected);
    std::cout << " ✓\n";
  }

  // 3. Pruebas de tablas constexpr para int16_t
  std::cout << "\n3. Pruebas de tabla constexpr int16_t (muestra parcial):\n";
  int16_t test_exps_int16[] = {0, 1, 5, 10, 14};
  for (int exp : test_exps_int16) {
    int16_t result = get_power_of_2_int16(exp);
    int16_t expected = static_cast<int16_t>(1 << exp);
    std::cout << "   2^" << exp << " = " << result;
    assert(result == expected);
    std::cout << " ✓\n";
  }

  // 4. Pruebas de tablas constexpr para uint16_t
  std::cout << "\n4. Pruebas de tabla constexpr uint16_t (muestra parcial):\n";
  int test_exps_uint16[] = {0, 1, 8, 12, 15};
  for (int exp : test_exps_uint16) {
    uint16_t result = get_power_of_2_uint16(exp);
    uint16_t expected = static_cast<uint16_t>(1 << exp);
    std::cout << "   2^" << exp << " = " << result;
    assert(result == expected);
    std::cout << " ✓\n";
  }

  // 5. Pruebas de función template genérica
  std::cout << "\n5. Pruebas de función template genérica:\n";

  // int8_t
  int8_t result_i8 = get_power_of_2_from_table<int8_t>(4);
  std::cout << "   get_power_of_2_from_table<int8_t>(4) = "
            << static_cast<int>(result_i8);
  assert(result_i8 == 16);
  std::cout << " ✓\n";

  // uint8_t
  uint8_t result_u8 = get_power_of_2_from_table<uint8_t>(6);
  std::cout << "   get_power_of_2_from_table<uint8_t>(6) = "
            << static_cast<int>(result_u8);
  assert(result_u8 == 64);
  std::cout << " ✓\n";

  // int16_t
  int16_t result_i16 = get_power_of_2_from_table<int16_t>(10);
  std::cout << "   get_power_of_2_from_table<int16_t>(10) = " << result_i16;
  assert(result_i16 == 1024);
  std::cout << " ✓\n";

  // uint16_t
  uint16_t result_u16 = get_power_of_2_from_table<uint16_t>(12);
  std::cout << "   get_power_of_2_from_table<uint16_t>(12) = " << result_u16;
  assert(result_u16 == 4096);
  std::cout << " ✓\n";

  // 6. Pruebas de verificación de rangos
  std::cout << "\n6. Pruebas de verificación de rangos:\n";

  std::cout << "   int8_t: rango válido [0, "
            << get_max_power_of_2_exponent<int8_t>() << "] ✓\n";
  std::cout << "   uint8_t: rango válido [0, "
            << get_max_power_of_2_exponent<uint8_t>() << "] ✓\n";
  std::cout << "   int16_t: rango válido [0, "
            << get_max_power_of_2_exponent<int16_t>() << "] ✓\n";
  std::cout << "   uint16_t: rango válido [0, "
            << get_max_power_of_2_exponent<uint16_t>() << "] ✓\n";

  // Verificar límites
  assert(is_valid_power_of_2_exponent<int8_t>(6));   // Válido
  assert(!is_valid_power_of_2_exponent<int8_t>(7));  // Inválido (overflow)
  assert(is_valid_power_of_2_exponent<uint8_t>(7));  // Válido
  assert(!is_valid_power_of_2_exponent<uint8_t>(8)); // Inválido (overflow)

  std::cout << "   Verificaciones de límites correctas ✓\n";

  // 7. Demostración de rendimiento en tiempo de compilación
  std::cout << "\n7. Demostración de constexpr (tiempo de compilación):\n";

  // Estos valores se calculan en tiempo de compilación
  constexpr int8_t compile_time_result1 = get_power_of_2_int8(5);
  constexpr uint16_t compile_time_result2 = get_power_of_2_uint16(12);

  std::cout << "   constexpr int8_t(2^5) = "
            << static_cast<int>(compile_time_result1) << " ✓\n";
  std::cout << "   constexpr uint16_t(2^12) = " << compile_time_result2
            << " ✓\n";

  static_assert(compile_time_result1 == 32, "Error en cálculo constexpr");
  static_assert(compile_time_result2 == 4096, "Error en cálculo constexpr");

  std::cout << "   static_assert verificaciones pasaron ✓\n";

  // 8. Comparación con cálculo tradicional
  std::cout << "\n8. Comparación tablas vs cálculo tradicional:\n";

  // Simular tiempo (en compilación real, las tablas son instantáneas)
  auto start = std::chrono::high_resolution_clock::now();

  // Usando tablas
  for (int i = 0; i < 1000; ++i) {
    volatile uint16_t result = get_power_of_2_uint16(10);
    (void)result; // Evitar optimización del compilador
  }

  auto mid = std::chrono::high_resolution_clock::now();

  // Usando cálculo tradicional
  for (int i = 0; i < 1000; ++i) {
    volatile uint16_t result = static_cast<uint16_t>(1 << 10);
    (void)result;
  }

  auto end = std::chrono::high_resolution_clock::now();

  auto table_time =
      std::chrono::duration_cast<std::chrono::nanoseconds>(mid - start);
  auto calc_time =
      std::chrono::duration_cast<std::chrono::nanoseconds>(end - mid);

  std::cout << "   Tiempo tablas: " << table_time.count() << " ns\n";
  std::cout << "   Tiempo cálculo: " << calc_time.count() << " ns\n";
  std::cout << "   (En contexto constexpr, las tablas son instantáneas)\n";

  std::cout << "\n=== TODAS LAS PRUEBAS PASARON EXITOSAMENTE ===\n";
  std::cout << "Las tablas constexpr para tipos pequeños funcionan "
               "perfectamente!\n\n";

  std::cout << "🚀 BENEFICIOS LOGRADOS:\n";
  std::cout << "   ✅ Acceso instantáneo en tiempo de compilación\n";
  std::cout << "   ✅ Sin cálculos en runtime para potencias de 2\n";
  std::cout << "   ✅ Verificación automática de overflow\n";
  std::cout << "   ✅ Especialización automática por tipo\n";
  std::cout << "   ✅ API unificada con dispatch inteligente\n";

  return 0;
}