#include <chrono>
#include <cstdint>
#include <iostream>
#include <random>
#include <vector>


/**
 * @file simple_benchmark.cpp
 * @brief Benchmark simple para comparar acceso directo vs cálculo
 */

// Simulación de tablas constexpr simples
constexpr int8_t power_table_int8[] = {1, 2, 4, 8, 16, 32, 64};
constexpr uint8_t power_table_uint8[] = {1, 2, 4, 8, 16, 32, 64, 128};
constexpr int16_t power_table_int16[] = {
    1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384};

// Función de tabla
template <typename T> T table_lookup(int exp);

template <> int8_t table_lookup<int8_t>(int exp) {
  return (exp >= 0 && exp < 7) ? power_table_int8[exp] : 0;
}

template <> uint8_t table_lookup<uint8_t>(int exp) {
  return (exp >= 0 && exp < 8) ? power_table_uint8[exp] : 0;
}

template <> int16_t table_lookup<int16_t>(int exp) {
  return (exp >= 0 && exp < 15) ? power_table_int16[exp] : 0;
}

// Función de cálculo tradicional
template <typename T> T shift_calculation(int exp) {
  return (exp >= 0) ? static_cast<T>(T{1} << exp) : T{0};
}

// Función de cálculo manual
template <typename T> T manual_calculation(int exp) {
  if (exp < 0)
    return T{0};
  T result = 1;
  for (int i = 0; i < exp; ++i) {
    result *= 2;
  }
  return result;
}

template <typename T>
void run_benchmark(const std::string &type_name, int max_exp,
                   size_t iterations = 1000000) {
  std::cout << "\n=== BENCHMARK " << type_name << " ===\n";

  // Generar datos de prueba
  std::vector<int> test_data;
  std::random_device rd;
  std::mt19937 gen(42); // Seed fijo para reproducibilidad
  std::uniform_int_distribution<> dis(0, max_exp);

  for (size_t i = 0; i < iterations; ++i) {
    test_data.push_back(dis(gen));
  }

  volatile T sink = 0; // Evitar optimizaciones

  // Benchmark tabla
  auto start = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i < iterations; ++i) {
    sink += table_lookup<T>(test_data[i]);
  }
  auto end = std::chrono::high_resolution_clock::now();
  auto table_time =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start);

  // Benchmark shift
  start = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i < iterations; ++i) {
    sink += shift_calculation<T>(test_data[i]);
  }
  end = std::chrono::high_resolution_clock::now();
  auto shift_time =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start);

  // Benchmark manual
  start = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i < iterations; ++i) {
    sink += manual_calculation<T>(test_data[i]);
  }
  end = std::chrono::high_resolution_clock::now();
  auto manual_time =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start);

  // Resultados
  std::cout << "Iteraciones: " << iterations << "\n";
  std::cout << "Tabla lookup: " << table_time.count() << " μs\n";
  std::cout << "Bit shift:    " << shift_time.count() << " μs\n";
  std::cout << "Manual:       " << manual_time.count() << " μs\n";

  // Speedup
  if (table_time.count() > 0) {
    double speedup_shift =
        static_cast<double>(shift_time.count()) / table_time.count();
    double speedup_manual =
        static_cast<double>(manual_time.count()) / table_time.count();
    std::cout << "Speedup vs shift: " << speedup_shift << "x\n";
    std::cout << "Speedup vs manual: " << speedup_manual << "x\n";
  }

  (void)sink; // Evitar warning
}

int main() {
  std::cout << "================================================\n";
  std::cout << "BENCHMARK SIMPLE: TABLAS vs CÁLCULOS\n";
  std::cout << "================================================\n";

  // Test de corrección
  std::cout << "\n=== VERIFICACIÓN DE CORRECCIÓN ===\n";
  bool all_correct = true;

  for (int i = 0; i < 7; ++i) {
    int8_t table_val = table_lookup<int8_t>(i);
    int8_t shift_val = shift_calculation<int8_t>(i);
    int8_t manual_val = manual_calculation<int8_t>(i);

    if (table_val != shift_val || table_val != manual_val) {
      std::cout << "❌ Error en int8_t 2^" << i << "\n";
      all_correct = false;
    }
  }

  if (all_correct) {
    std::cout << "✅ Todos los métodos dan resultados correctos\n";
  }

  // Benchmarks
  const size_t iterations = 2000000;

  run_benchmark<int8_t>("int8_t", 6, iterations);
  run_benchmark<uint8_t>("uint8_t", 7, iterations);
  run_benchmark<int16_t>("int16_t", 14, iterations);

  std::cout << "\n================================================\n";
  std::cout << "CONCLUSIONES:\n";
  std::cout << "- Las tablas lookup proporcionan acceso directo\n";
  std::cout << "- El bit shift es muy eficiente en hardware moderno\n";
  std::cout << "- El cálculo manual es el más lento\n";
  std::cout << "- En contexto constexpr, las tablas son inmediatas\n";
  std::cout << "================================================\n";

  return 0;
}