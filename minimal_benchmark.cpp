#include <chrono>
#include <cstdint>
#include <iostream>


/**
 * @file minimal_benchmark.cpp
 * @brief Benchmark mínimo para demostrar tablas constexpr
 */

// Tablas constexpr simples
constexpr int8_t power_table_int8[7] = {1, 2, 4, 8, 16, 32, 64};
constexpr uint8_t power_table_uint8[8] = {1, 2, 4, 8, 16, 32, 64, 128};

// Acceso a tabla
int8_t table_lookup_int8(int exp) {
  return (exp >= 0 && exp < 7) ? power_table_int8[exp] : 0;
}

// Cálculo tradicional
int8_t shift_calc_int8(int exp) {
  return (exp >= 0 && exp < 7) ? static_cast<int8_t>(1 << exp) : 0;
}

// Cálculo manual
int8_t manual_calc_int8(int exp) {
  if (exp < 0 || exp >= 7)
    return 0;
  int8_t result = 1;
  for (int i = 0; i < exp; ++i) {
    result *= 2;
  }
  return result;
}

int main() {
  std::cout << "BENCHMARK MINIMAL: TABLAS CONSTEXPR vs CALCULOS\n";
  std::cout << "===============================================\n\n";

  // Test de corrección
  std::cout << "Test de corrección:\n";
  bool all_correct = true;
  for (int i = 0; i < 7; ++i) {
    int8_t table_val = table_lookup_int8(i);
    int8_t shift_val = shift_calc_int8(i);
    int8_t manual_val = manual_calc_int8(i);

    std::cout << "2^" << i << ": tabla=" << static_cast<int>(table_val)
              << " shift=" << static_cast<int>(shift_val)
              << " manual=" << static_cast<int>(manual_val);

    if (table_val == shift_val && table_val == manual_val) {
      std::cout << " ✓\n";
    } else {
      std::cout << " ❌\n";
      all_correct = false;
    }
  }

  if (!all_correct) {
    std::cout << "\nError en corrección - abortando\n";
    return 1;
  }

  // Benchmark simple
  std::cout << "\nBenchmark de rendimiento:\n";
  const size_t iterations = 10000000; // 10 millones

  std::cout << "Iteraciones: " << iterations << "\n";

  volatile int8_t sink = 0; // Evitar optimizaciones

  // Benchmark tabla
  auto start = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i < iterations; ++i) {
    sink += table_lookup_int8(i % 7);
  }
  auto end = std::chrono::high_resolution_clock::now();
  auto table_time =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  // Benchmark shift
  start = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i < iterations; ++i) {
    sink += shift_calc_int8(i % 7);
  }
  end = std::chrono::high_resolution_clock::now();
  auto shift_time =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  // Benchmark manual
  start = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i < iterations; ++i) {
    sink += manual_calc_int8(i % 7);
  }
  end = std::chrono::high_resolution_clock::now();
  auto manual_time =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  // Resultados
  std::cout << "\nResultados:\n";
  std::cout << "Tabla lookup: " << table_time.count() << " ms\n";
  std::cout << "Bit shift:    " << shift_time.count() << " ms\n";
  std::cout << "Manual:       " << manual_time.count() << " ms\n";

  // Cálculo de speedup
  if (table_time.count() > 0) {
    double speedup_shift =
        static_cast<double>(shift_time.count()) / table_time.count();
    double speedup_manual =
        static_cast<double>(manual_time.count()) / table_time.count();
    std::cout << "\nSpeedup tabla vs shift: " << speedup_shift << "x\n";
    std::cout << "Speedup tabla vs manual: " << speedup_manual << "x\n";
  }

  // Test constexpr
  std::cout << "\nTest constexpr (calculado en tiempo de compilación):\n";
  constexpr int8_t c1 = power_table_int8[3]; // 2^3 = 8
  constexpr int8_t c2 = power_table_int8[5]; // 2^5 = 32
  std::cout << "constexpr 2^3 = " << static_cast<int>(c1) << "\n";
  std::cout << "constexpr 2^5 = " << static_cast<int>(c2) << "\n";

  std::cout << "\n===============================================\n";
  std::cout << "CONCLUSIONES:\n";
  std::cout << "✅ Todas las funciones dan resultados correctos\n";
  std::cout << "🚀 Las tablas constexpr son inmediatas en compilación\n";
  std::cout << "⚡ En runtime, depende de las optimizaciones del compilador\n";
  std::cout << "💡 Para tipos pequeños, las tablas eliminan cálculos\n";
  std::cout << "===============================================\n";

  (void)sink; // Evitar warning
  return 0;
}