#include "simplified_power_tables_cpp14.hpp"
#include <cassert>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <random>
#include <vector>


/**
 * @file benchmark_constexpr_tables.cpp
 * @brief Benchmark completo para comparar tablas constexpr vs cálculos
 * tradicionales
 */

using namespace std::chrono;
using namespace number_calc::integral_power_functions::lookup_tables;

// Funciones de cálculo tradicional para comparación
template <typename T> T traditional_power_of_2(int exponent) {
  if (exponent < 0)
    return T{0};
  return static_cast<T>(T{1} << exponent);
}

// Función de cálculo manual (más segura)
template <typename T> T manual_power_of_2(int exponent) {
  if (exponent < 0)
    return T{0};
  T result = 1;
  for (int i = 0; i < exponent; ++i) {
    result *= 2;
  }
  return result;
}

// Benchmark para un tipo específico
template <typename T>
void benchmark_type(const std::string &type_name, int max_exp,
                    size_t iterations = 10000000) {
  std::cout << "\n=== BENCHMARK " << type_name << " ===\n";
  std::cout << "Rango de exponentes: 0 a " << max_exp << "\n";
  std::cout << "Iteraciones: " << iterations << "\n\n";

  // Generar exponentes aleatorios para el test
  std::vector<int> test_exponents;
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, max_exp);

  for (size_t i = 0; i < iterations; ++i) {
    test_exponents.push_back(dis(gen));
  }

  volatile T sink = 0; // Para evitar optimizaciones del compilador

  // 1. Benchmark tabla constexpr
  auto start = high_resolution_clock::now();
  for (size_t i = 0; i < iterations; ++i) {
    sink += get_power_of_2_from_table<T>(test_exponents[i]);
  }
  auto end = high_resolution_clock::now();
  auto constexpr_time = duration_cast<nanoseconds>(end - start);

  // 2. Benchmark cálculo con bit shift
  start = high_resolution_clock::now();
  for (size_t i = 0; i < iterations; ++i) {
    sink += traditional_power_of_2<T>(test_exponents[i]);
  }
  end = high_resolution_clock::now();
  auto bitshift_time = duration_cast<nanoseconds>(end - start);

  // 3. Benchmark cálculo manual (bucle)
  start = high_resolution_clock::now();
  for (size_t i = 0; i < iterations; ++i) {
    sink += manual_power_of_2<T>(test_exponents[i]);
  }
  end = high_resolution_clock::now();
  auto manual_time = duration_cast<nanoseconds>(end - start);

  // Resultados
  std::cout << "Tiempo tabla constexpr: " << constexpr_time.count() << " ns\n";
  std::cout << "Tiempo bit shift:       " << bitshift_time.count() << " ns\n";
  std::cout << "Tiempo cálculo manual:  " << manual_time.count() << " ns\n";

  // Cálculo de speedup
  double speedup_vs_bitshift =
      static_cast<double>(bitshift_time.count()) / constexpr_time.count();
  double speedup_vs_manual =
      static_cast<double>(manual_time.count()) / constexpr_time.count();

  std::cout << "\nSpeedup tabla vs bit shift: " << speedup_vs_bitshift << "x\n";
  std::cout << "Speedup tabla vs manual:    " << speedup_vs_manual << "x\n";

  // Tiempo promedio por operación
  double avg_constexpr =
      static_cast<double>(constexpr_time.count()) / iterations;
  double avg_bitshift = static_cast<double>(bitshift_time.count()) / iterations;
  double avg_manual = static_cast<double>(manual_time.count()) / iterations;

  std::cout << "\nTiempo promedio por operación:\n";
  std::cout << "  Tabla constexpr: " << avg_constexpr << " ns/op\n";
  std::cout << "  Bit shift:       " << avg_bitshift << " ns/op\n";
  std::cout << "  Manual:          " << avg_manual << " ns/op\n";

  (void)sink; // Evitar warning de variable no usada
}

// Test de corrección para verificar que todos los métodos dan el mismo
// resultado
template <typename T>
bool correctness_test(const std::string &type_name, int max_exp) {
  std::cout << "Test de corrección " << type_name << ": ";

  for (int exp = 0; exp <= max_exp; ++exp) {
    T constexpr_result = get_power_of_2_from_table<T>(exp);
    T bitshift_result = traditional_power_of_2<T>(exp);
    T manual_result = manual_power_of_2<T>(exp);

    if (constexpr_result != bitshift_result ||
        constexpr_result != manual_result) {
      std::cout << "❌ FALLO en 2^" << exp << "\n";
      std::cout << "  Tabla: " << static_cast<uint64_t>(constexpr_result)
                << "\n";
      std::cout << "  Shift: " << static_cast<uint64_t>(bitshift_result)
                << "\n";
      std::cout << "  Manual: " << static_cast<uint64_t>(manual_result) << "\n";
      return false;
    }
  }

  std::cout << "✅ CORRECTO\n";
  return true;
}

// Test de compilación constexpr
void constexpr_compilation_test() {
  std::cout << "\n=== TEST DE COMPILACIÓN CONSTEXPR ===\n";

  // Estos valores se calculan en tiempo de compilación
  constexpr int8_t c1 = get_power_of_2_int8(5);
  constexpr uint8_t c2 = get_power_of_2_uint8(7);
  constexpr int16_t c3 = get_power_of_2_int16(10);
  constexpr uint16_t c4 = get_power_of_2_uint16(12);
  constexpr int32_t c5 = get_power_of_2_int32(20);
  constexpr uint32_t c6 = get_power_of_2_uint32(25);

  std::cout << "Valores calculados en tiempo de compilación:\n";
  std::cout << "  int8_t(2^5) = " << static_cast<int>(c1) << "\n";
  std::cout << "  uint8_t(2^7) = " << static_cast<int>(c2) << "\n";
  std::cout << "  int16_t(2^10) = " << c3 << "\n";
  std::cout << "  uint16_t(2^12) = " << c4 << "\n";
  std::cout << "  int32_t(2^20) = " << c5 << "\n";
  std::cout << "  uint32_t(2^25) = " << c6 << "\n";

  // Verificación
  assert(c1 == 32);
  assert(c2 == 128);
  assert(c3 == 1024);
  assert(c4 == 4096);
  assert(c5 == 1048576);
  assert(c6 == 33554432);

  std::cout << "✅ Todos los valores constexpr son correctos\n";
}

int main() {
  std::cout << "================================================\n";
  std::cout << "BENCHMARK TABLAS CONSTEXPR vs CÁLCULOS TRADICIONALES\n";
  std::cout << "================================================\n";

  // Test de corrección primero
  std::cout << "\n=== TESTS DE CORRECCIÓN ===\n";
  bool all_correct = true;
  all_correct &= correctness_test<int8_t>("int8_t", 6);
  all_correct &= correctness_test<uint8_t>("uint8_t", 7);
  all_correct &= correctness_test<int16_t>("int16_t", 14);
  all_correct &= correctness_test<uint16_t>("uint16_t", 15);
  all_correct &= correctness_test<int32_t>("int32_t", 30);
  all_correct &= correctness_test<uint32_t>("uint32_t", 31);

  if (!all_correct) {
    std::cout << "\n❌ ERRORES EN TESTS DE CORRECCIÓN - ABORTANDO\n";
    return 1;
  }

  // Test de compilación constexpr
  constexpr_compilation_test();

  // Benchmarks de rendimiento
  std::cout << "\n=== BENCHMARKS DE RENDIMIENTO ===\n";

  const size_t iterations = 5000000; // 5 millones de iteraciones

  benchmark_type<int8_t>("int8_t", 6, iterations);
  benchmark_type<uint8_t>("uint8_t", 7, iterations);
  benchmark_type<int16_t>("int16_t", 14, iterations);
  benchmark_type<uint16_t>("uint16_t", 15, iterations);
  benchmark_type<int32_t>("int32_t", 30, iterations);
  benchmark_type<uint32_t>("uint32_t", 31, iterations);

  std::cout << "\n================================================\n";
  std::cout << "RESUMEN DE RESULTADOS\n";
  std::cout << "================================================\n";
  std::cout << "✅ Todos los tests de corrección pasaron\n";
  std::cout << "✅ Valores constexpr calculados en tiempo de compilación\n";
  std::cout << "✅ Benchmarks completados para todos los tipos\n";
  std::cout << "\n🚀 Las tablas constexpr proporcionan acceso O(1)\n";
  std::cout << "   optimizado para potencias de 2 frecuentes!\n";

  return 0;
}