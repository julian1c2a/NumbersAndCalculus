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
    std::cout << " [OK]\n";
  }

  // Verificar que exponente 7 esta fuera de rango (overflow)
  std::cout << "   2^7 causaria overflow (max int8_t = 127)\n";

  // 2. Pruebas de tablas constexpr para uint8_t
  std::cout << "\n2. Pruebas de tabla constexpr uint8_t (2^0 hasta 2^7):\n";
  for (int exp = 0; exp <= 7; ++exp) {
    uint8_t result = get_power_of_2_uint8(exp);
    uint8_t expected = static_cast<uint8_t>(1 << exp);
    std::cout << "   2^" << exp << " = " << static_cast<int>(result);
    assert(result == expected);
    std::cout << " [OK]\n";
  }

  // 3. Pruebas de tablas constexpr para int16_t
  std::cout << "\n3. Pruebas de tabla constexpr int16_t (muestra parcial):\n";
  int16_t test_exps_int16[] = {0, 1, 5, 10, 14};
  for (int exp : test_exps_int16) {
    int16_t result = get_power_of_2_int16(exp);
    int16_t expected = static_cast<int16_t>(1 << exp);
    std::cout << "   2^" << exp << " = " << result;
    assert(result == expected);
    std::cout << " [OK]\n";
  }

  // 4. Pruebas de tablas constexpr para uint16_t
  std::cout << "\n4. Pruebas de tabla constexpr uint16_t (muestra parcial):\n";
  int test_exps_uint16[] = {0, 1, 8, 12, 15};
  for (int exp : test_exps_uint16) {
    uint16_t result = get_power_of_2_uint16(exp);
    uint16_t expected = static_cast<uint16_t>(1 << exp);
    std::cout << "   2^" << exp << " = " << result;
    assert(result == expected);
    std::cout << " [OK]\n";
  }

  // 5. Pruebas de funcion template generica
  std::cout << "\n5. Pruebas de funcion template generica:\n";

  // int8_t
  int8_t result_i8 = get_power_of_2_from_table<int8_t>(4);
  std::cout << "   get_power_of_2_from_table<int8_t>(4) = "
            << static_cast<int>(result_i8);
  assert(result_i8 == 16);
  std::cout << " [OK]\n";

  // uint8_t
  uint8_t result_u8 = get_power_of_2_from_table<uint8_t>(6);
  std::cout << "   get_power_of_2_from_table<uint8_t>(6) = "
            << static_cast<int>(result_u8);
  assert(result_u8 == 64);
  std::cout << " [OK]\n";

  // int16_t
  int16_t result_i16 = get_power_of_2_from_table<int16_t>(10);
  std::cout << "   get_power_of_2_from_table<int16_t>(10) = " << result_i16;
  assert(result_i16 == 1024);
  std::cout << " [OK]\n";

  // uint16_t
  uint16_t result_u16 = get_power_of_2_from_table<uint16_t>(12);
  std::cout << "   get_power_of_2_from_table<uint16_t>(12) = " << result_u16;
  assert(result_u16 == 4096);
  std::cout << " [OK]\n";

  // 6. Pruebas de verificacion de rangos
  std::cout << "\n6. Pruebas de verificacion de rangos:\n";

  std::cout << "   int8_t: rango valido [0, "
            << get_max_power_of_2_exponent<int8_t>() << "] [OK]\n";
  std::cout << "   uint8_t: rango valido [0, "
            << get_max_power_of_2_exponent<uint8_t>() << "] [OK]\n";
  std::cout << "   int16_t: rango valido [0, "
            << get_max_power_of_2_exponent<int16_t>() << "] [OK]\n";
  std::cout << "   uint16_t: rango valido [0, "
            << get_max_power_of_2_exponent<uint16_t>() << "] [OK]\n";

  // Verificar limites
  assert(is_valid_power_of_2_exponent<int8_t>(6));   // Valido
  assert(!is_valid_power_of_2_exponent<int8_t>(7));  // Invalido (overflow)
  assert(is_valid_power_of_2_exponent<uint8_t>(7));  // Valido
  assert(!is_valid_power_of_2_exponent<uint8_t>(8)); // Invalido (overflow)

  std::cout << "   Verificaciones de limites correctas [OK]\n";

  // 7. Demostracion de rendimiento en tiempo de compilacion
  std::cout << "\n7. Demostracion de constexpr (tiempo de compilacion):\n";

  // Estos valores se calculan en tiempo de compilacion
  constexpr int8_t compile_time_result1 = get_power_of_2_int8(5);
  constexpr uint16_t compile_time_result2 = get_power_of_2_uint16(12);

  std::cout << "   constexpr int8_t(2^5) = "
            << static_cast<int>(compile_time_result1) << " [OK]\n";
  std::cout << "   constexpr uint16_t(2^12) = " << compile_time_result2
            << " [OK]\n";

  static_assert(compile_time_result1 == 32, "Error en calculo constexpr");
  static_assert(compile_time_result2 == 4096, "Error en calculo constexpr");

  std::cout << "   static_assert verificaciones pasaron [OK]\n";

  // 8. Comparacion con calculo tradicional
  std::cout << "\n8. Comparacion tablas vs calculo tradicional:\n";

  // Simular tiempo (en compilacion real, las tablas son instantaneas)
  auto start = std::chrono::high_resolution_clock::now();

  // Usando tablas
  for (int i = 0; i < 1000; ++i) {
    volatile uint16_t result = get_power_of_2_uint16(10);
    (void)result; // Evitar optimizacion del compilador
  }

  auto mid = std::chrono::high_resolution_clock::now();

  // Usando calculo tradicional
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
  std::cout << "   Tiempo calculo: " << calc_time.count() << " ns\n";
  std::cout << "   (En contexto constexpr, las tablas son instantaneas)\n";

  std::cout << "\n=== TODAS LAS PRUEBAS PASARON EXITOSAMENTE ===\n";
  std::cout << "Las tablas constexpr para tipos pequenos funcionan "
               "perfectamente!\n\n";

  std::cout << "[ROCKET] BENEFICIOS LOGRADOS:\n";
  std::cout << "   [CHECK] Acceso instantaneo en tiempo de compilacion\n";
  std::cout << "   [CHECK] Sin calculos en runtime para potencias de 2\n";
  std::cout << "   [CHECK] Verificacion automatica de overflow\n";
  std::cout << "   [CHECK] Especializacion automatica por tipo\n";
  std::cout << "   [CHECK] API unificada con dispatch inteligente\n";

  return 0;
}