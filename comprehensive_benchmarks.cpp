#include <algorithm>
#include <chrono>
#include <cstring>
#include <iostream>
#include <memory>
#include <vector>

// NOTA: Este código requiere compilación con -std=c++17 para std::optional
#include "include/number_calc/comprehensive_benchmarks.hpp"

using namespace number_calc::benchmarks;

//==============================================================================
// BENCHMARKS ESPECÍFICOS
//==============================================================================

/**
 * @brief Benchmark de comparación entre CalculationResult y std::optional
 */
class ErrorHandlingBenchmarks {
private:
  TestDataGenerator generator;
  std::vector<std::pair<int, int>> division_data;
  std::vector<int> lookup_data;

public:
  ErrorHandlingBenchmarks() {
    // Generar datos de prueba
    division_data = generator.generate_division_pairs(100000);
    lookup_data = generator.generate_lookup_indices(100000, 6);

    std::cout << "=== BENCHMARKS DE MANEJO DE ERRORES ===" << std::endl;
    std::cout << "Datos generados: " << division_data.size()
              << " pares de división, " << lookup_data.size()
              << " índices de lookup" << std::endl;
    std::cout << std::endl;
  }

  void run_all() {
    run_division_benchmarks();
    run_lookup_benchmarks();
    run_memory_benchmarks();
    run_branch_prediction_benchmarks();
  }

  void run_all_with_export(BenchmarkExporter &exporter) {
    std::cout << "=== BENCHMARKS DE MANEJO DE ERRORES ===" << std::endl;

    size_t data_idx = 0;

    // Benchmark Legacy Division
    auto legacy_result = run_benchmark(
        "Legacy CalculationResult Division",
        [this, &data_idx]() -> bool {
          auto &pair = division_data[data_idx % division_data.size()];
          data_idx++;
          auto result = legacy_safe_divide(pair.first, pair.second);
          return result.is_valid();
        },
        500000);
    exporter.add_result(legacy_result);

    // Reset counter
    data_idx = 0;

    // Benchmark Modern Division
    auto modern_result = run_benchmark(
        "Modern std::optional Division",
        [this, &data_idx]() -> bool {
          auto &pair = division_data[data_idx % division_data.size()];
          data_idx++;
          auto result = modern_safe_divide(pair.first, pair.second);
          return result.has_value();
        },
        500000);
    exporter.add_result(modern_result);

    // Reset counter para lookup tests
    data_idx = 0;

    // Benchmark Legacy Lookup
    auto legacy_lookup_result = run_benchmark(
        "Legacy CalculationResult Lookup",
        [this, &data_idx]() -> bool {
          int index = lookup_data[data_idx % lookup_data.size()];
          data_idx++;
          auto result = legacy_get_power_of_2_int8(index);
          return result.is_valid();
        },
        1000000);
    exporter.add_result(legacy_lookup_result);

    // Reset counter
    data_idx = 0;

    // Benchmark Modern Lookup
    auto modern_lookup_result = run_benchmark(
        "Modern std::optional Lookup",
        [this, &data_idx]() -> bool {
          int index = lookup_data[data_idx % lookup_data.size()];
          data_idx++;
          auto result = modern_get_power_of_2_int8(index);
          return result.has_value();
        },
        1000000);
    exporter.add_result(modern_lookup_result);

    // También ejecutar los otros benchmarks sin export (por compatibilidad)
    run_memory_benchmarks();
    run_branch_prediction_benchmarks();
  }

private:
  void run_division_benchmarks() {
    std::cout << "### BENCHMARKS DE DIVISIÓN ###" << std::endl;

    size_t data_idx = 0;

    // Benchmark CalculationResult (legacy)
    auto legacy_result = run_benchmark(
        "Legacy CalculationResult Division",
        [this, &data_idx]() -> bool {
          auto &pair = division_data[data_idx % division_data.size()];
          data_idx++;
          auto result = legacy_safe_divide(pair.first, pair.second);
          return result.is_valid();
        },
        500000);
    legacy_result.print();

    // Reset counter
    data_idx = 0;

    // Benchmark std::optional (modern)
    auto modern_result = run_benchmark(
        "Modern std::optional Division",
        [this, &data_idx]() -> bool {
          auto &pair = division_data[data_idx % division_data.size()];
          data_idx++;
          auto result = modern_safe_divide(pair.first, pair.second);
          return result.has_value();
        },
        500000);
    modern_result.print();

    // Comparación
    double speedup = legacy_result.avg_time_ns / modern_result.avg_time_ns;
    std::cout << "SPEEDUP: std::optional es " << std::fixed
              << std::setprecision(2) << speedup << "x "
              << (speedup > 1.0 ? "más rápido" : "más lento")
              << " que CalculationResult" << std::endl;
    std::cout << std::endl;
  }

  void run_lookup_benchmarks() {
    std::cout << "### BENCHMARKS DE LOOKUP TABLES ###" << std::endl;

    size_t data_idx = 0;

    // Benchmark CalculationResult lookup
    auto legacy_lookup_result = run_benchmark(
        "Legacy CalculationResult Lookup",
        [this, &data_idx]() -> bool {
          int index = lookup_data[data_idx % lookup_data.size()];
          data_idx++;
          auto result = legacy_get_power_of_2_int8(index);
          return result.is_valid();
        },
        1000000);
    legacy_lookup_result.print();

    // Reset counter
    data_idx = 0;

    // Benchmark std::optional lookup
    auto modern_lookup_result = run_benchmark(
        "Modern std::optional Lookup",
        [this, &data_idx]() -> bool {
          int index = lookup_data[data_idx % lookup_data.size()];
          data_idx++;
          auto result = modern_get_power_of_2_int8(index);
          return result.has_value();
        },
        1000000);
    modern_lookup_result.print();

    // Comparación
    double speedup =
        legacy_lookup_result.avg_time_ns / modern_lookup_result.avg_time_ns;
    std::cout << "SPEEDUP: std::optional lookup es " << std::fixed
              << std::setprecision(2) << speedup << "x "
              << (speedup > 1.0 ? "más rápido" : "más lento")
              << " que CalculationResult lookup" << std::endl;
    std::cout << std::endl;
  }

  void run_memory_benchmarks() {
    std::cout << "### BENCHMARKS DE MEMORIA ###" << std::endl;

    // Tamaños de estructuras
    std::cout << "Tamaños de estructuras:" << std::endl;
    std::cout << "  sizeof(LegacyCalculationResult<int>): "
              << sizeof(LegacyCalculationResult<int>) << " bytes" << std::endl;
    std::cout << "  sizeof(std::optional<int>): " << sizeof(std::optional<int>)
              << " bytes" << std::endl;
    std::cout << "  sizeof(LegacyCalculationResult<int8_t>): "
              << sizeof(LegacyCalculationResult<int8_t>) << " bytes"
              << std::endl;
    std::cout << "  sizeof(std::optional<int8_t>): "
              << sizeof(std::optional<int8_t>) << " bytes" << std::endl;

    // Alineación
    std::cout << std::endl << "Alineación de estructuras:" << std::endl;
    std::cout << "  alignof(LegacyCalculationResult<int>): "
              << alignof(LegacyCalculationResult<int>) << " bytes" << std::endl;
    std::cout << "  alignof(std::optional<int>): "
              << alignof(std::optional<int>) << " bytes" << std::endl;

    // Test de uso de memoria intensivo
    const size_t count = 1000000;

    Timer timer;

    // Benchmark creación de vectores legacy
    timer.start();
    std::vector<LegacyCalculationResult<int>> legacy_vec;
    legacy_vec.reserve(count);
    for (size_t i = 0; i < count; ++i) {
      if (i % 7 == 0) {
        legacy_vec.emplace_back(make_legacy_error<int>(
            number_calc::error_handling::CalculationError::OutOfRange));
      } else {
        legacy_vec.emplace_back(make_legacy_success(static_cast<int>(i)));
      }
    }
    double legacy_creation_time = timer.stop_ms();

    // Benchmark creación de vectores modern
    timer.start();
    std::vector<std::optional<int>> modern_vec;
    modern_vec.reserve(count);
    for (size_t i = 0; i < count; ++i) {
      if (i % 7 == 0) {
        modern_vec.emplace_back(std::nullopt);
      } else {
        modern_vec.emplace_back(static_cast<int>(i));
      }
    }
    double modern_creation_time = timer.stop_ms();

    std::cout << std::endl
              << "Creación de " << count << " elementos:" << std::endl;
    std::cout << "  Legacy: " << legacy_creation_time << " ms" << std::endl;
    std::cout << "  Modern: " << modern_creation_time << " ms" << std::endl;
    std::cout << "  Speedup: " << std::fixed << std::setprecision(2)
              << (legacy_creation_time / modern_creation_time) << "x"
              << std::endl;

    // Test de iteración
    timer.start();
    size_t legacy_valid_count = 0;
    for (const auto &item : legacy_vec) {
      if (item.is_valid()) {
        legacy_valid_count++;
      }
    }
    double legacy_iteration_time = timer.stop_ms();

    timer.start();
    size_t modern_valid_count = 0;
    for (const auto &item : modern_vec) {
      if (item.has_value()) {
        modern_valid_count++;
      }
    }
    double modern_iteration_time = timer.stop_ms();

    std::cout << std::endl
              << "Iteración sobre " << count << " elementos:" << std::endl;
    std::cout << "  Legacy: " << legacy_iteration_time << " ms ("
              << legacy_valid_count << " válidos)" << std::endl;
    std::cout << "  Modern: " << modern_iteration_time << " ms ("
              << modern_valid_count << " válidos)" << std::endl;
    std::cout << "  Speedup: " << std::fixed << std::setprecision(2)
              << (legacy_iteration_time / modern_iteration_time) << "x"
              << std::endl;
    std::cout << std::endl;
  }

  void run_branch_prediction_benchmarks() {
    std::cout << "### BENCHMARKS DE PREDICCIÓN DE BRANCHES ###" << std::endl;

    const size_t iterations = 10000000;
    Timer timer;

    // Test con datos predecibles (patrón regular)
    std::cout << "Test con patrón predecible (cada 5to elemento es error):"
              << std::endl;

    timer.start();
    size_t legacy_successes = 0;
    for (size_t i = 0; i < iterations; ++i) {
      auto result = legacy_get_power_of_2_int8(i % 5 == 0 ? -1 : i % 7);
      if (result.is_valid()) {
        legacy_successes++;
      }
    }
    double legacy_predictable_time = timer.stop_ms();

    timer.start();
    size_t modern_successes = 0;
    for (size_t i = 0; i < iterations; ++i) {
      auto result = modern_get_power_of_2_int8(i % 5 == 0 ? -1 : i % 7);
      if (result.has_value()) {
        modern_successes++;
      }
    }
    double modern_predictable_time = timer.stop_ms();

    std::cout << "  Legacy: " << legacy_predictable_time << " ms ("
              << legacy_successes << " éxitos)" << std::endl;
    std::cout << "  Modern: " << modern_predictable_time << " ms ("
              << modern_successes << " éxitos)" << std::endl;
    std::cout << "  Speedup: " << std::fixed << std::setprecision(2)
              << (legacy_predictable_time / modern_predictable_time) << "x"
              << std::endl;

    // Test con datos impredecibles (aleatorios)
    std::cout << std::endl
              << "Test con patrón impredecible (índices aleatorios):"
              << std::endl;

    auto random_indices = generator.generate_lookup_indices(iterations, 6);

    timer.start();
    legacy_successes = 0;
    for (size_t i = 0; i < iterations; ++i) {
      auto result =
          legacy_get_power_of_2_int8(random_indices[i % random_indices.size()]);
      if (result.is_valid()) {
        legacy_successes++;
      }
    }
    double legacy_random_time = timer.stop_ms();

    timer.start();
    modern_successes = 0;
    for (size_t i = 0; i < iterations; ++i) {
      auto result =
          modern_get_power_of_2_int8(random_indices[i % random_indices.size()]);
      if (result.has_value()) {
        modern_successes++;
      }
    }
    double modern_random_time = timer.stop_ms();

    std::cout << "  Legacy: " << legacy_random_time << " ms ("
              << legacy_successes << " éxitos)" << std::endl;
    std::cout << "  Modern: " << modern_random_time << " ms ("
              << modern_successes << " éxitos)" << std::endl;
    std::cout << "  Speedup: " << std::fixed << std::setprecision(2)
              << (legacy_random_time / modern_random_time) << "x" << std::endl;
    std::cout << std::endl;
  }
};

//==============================================================================
// BENCHMARKS DE FUNCIONES PURAS
//==============================================================================

class PureFunctionBenchmarks {
public:
  void run_all() {
    std::cout << "### BENCHMARKS DE FUNCIONES PURAS ###" << std::endl;

    const size_t iterations = 50000000;
    Timer timer;

    // Benchmark función cuadrado (siempre exitosa)
    timer.start();
    volatile long long sum = 0; // volatile para evitar optimización
    for (size_t i = 0; i < iterations; ++i) {
      int x = static_cast<int>(i % 1000);
      sum += x * x; // inline square function
    }
    double square_time = timer.stop_ms();

    std::cout << "Square function (inline): " << square_time << " ms"
              << std::endl;
    std::cout << "Operaciones por segundo: " << std::fixed
              << std::setprecision(0) << (iterations / (square_time / 1000.0))
              << std::endl;
    std::cout << "Nanosegundos por operación: " << std::fixed
              << std::setprecision(3) << (square_time * 1000000.0 / iterations)
              << std::endl;
    std::cout << std::endl;
  }

  void run_all_with_export(BenchmarkExporter &exporter) {
    std::cout << "=== BENCHMARKS DE FUNCIONES PURAS ===" << std::endl;

    // Para funciones puras, solo agregamos un resultado representativo
    const size_t iterations = 50000000;
    Timer timer;

    timer.start();
    volatile long long sum = 0;
    for (size_t i = 0; i < iterations; ++i) {
      int x = static_cast<int>(i % 1000);
      sum += x * x;
    }
    double square_time = timer.stop_ms();

    // Crear un BenchmarkResult para funciones puras
    BenchmarkResult pure_result;
    pure_result.test_name = "Pure Functions (Square)";
    pure_result.iterations = iterations;
    pure_result.successful_operations = iterations; // Siempre exitosas
    pure_result.failed_operations = 0;
    pure_result.avg_time_ns = (square_time * 1000000.0) / iterations;
    pure_result.min_time_ns = pure_result.avg_time_ns; // Aproximación
    pure_result.max_time_ns = pure_result.avg_time_ns; // Aproximación

    exporter.add_result(pure_result);

    std::cout << "Square function: " << square_time << " ms" << std::endl;
    std::cout << "Operaciones por segundo: " << std::fixed
              << std::setprecision(0) << (iterations / (square_time / 1000.0))
              << std::endl;
    std::cout << std::endl;
  }
};

//==============================================================================
// BENCHMARKS DE LOOKUP TABLES EXTENSIVOS
//==============================================================================

class LookupTableBenchmarks {
private:
  TestDataGenerator generator;

public:
  void run_all() {
    std::cout << "### BENCHMARKS EXTENSIVOS DE LOOKUP TABLES ###" << std::endl;

    run_different_sizes();
    run_cache_performance();
    run_sequential_vs_random_access();
  }

private:
  void run_different_sizes() {
    std::cout << "Comparación de diferentes tamaños de lookup tables:"
              << std::endl;

    const size_t iterations = 5000000;
    Timer timer;

    // int8_t lookup (7 elementos)
    auto indices_int8 = generator.generate_lookup_indices(iterations, 6);
    timer.start();
    size_t int8_successes = 0;
    for (size_t i = 0; i < iterations; ++i) {
      auto result =
          modern_get_power_of_2_int8(indices_int8[i % indices_int8.size()]);
      if (result.has_value()) {
        int8_successes++;
      }
    }
    double int8_time = timer.stop_ms();

    std::cout << "  int8_t (7 elementos): " << int8_time << " ms ("
              << int8_successes << " éxitos)" << std::endl;

    // Simular int16_t lookup (15 elementos)
    auto indices_int16 = generator.generate_lookup_indices(iterations, 14);
    timer.start();
    size_t int16_successes = 0;
    for (size_t i = 0; i < iterations; ++i) {
      int index = indices_int16[i % indices_int16.size()];
      if (index >= 0 && index < 15) {
        int16_successes++;
      }
    }
    double int16_time = timer.stop_ms();

    std::cout << "  int16_t (15 elementos): " << int16_time << " ms ("
              << int16_successes << " éxitos)" << std::endl;
    std::cout << std::endl;
  }

  void run_cache_performance() {
    std::cout << "Análisis de rendimiento de cache:" << std::endl;

    const size_t small_iterations = 1000000;
    const size_t large_iterations = 10000000;
    Timer timer;

    // Test pequeño (fit en L1 cache)
    timer.start();
    volatile int8_t result = 0;
    for (size_t i = 0; i < small_iterations; ++i) {
      auto opt_result = modern_get_power_of_2_int8(i % 7);
      if (opt_result.has_value()) {
        result += *opt_result;
      }
    }
    double small_time = timer.stop_ms();

    // Test grande (no fit en cache)
    timer.start();
    result = 0;
    for (size_t i = 0; i < large_iterations; ++i) {
      auto opt_result = modern_get_power_of_2_int8(i % 7);
      if (opt_result.has_value()) {
        result += *opt_result;
      }
    }
    double large_time = timer.stop_ms();

    std::cout << "  Small dataset: " << small_time << " ms ("
              << small_iterations << " ops)" << std::endl;
    std::cout << "  Large dataset: " << large_time << " ms ("
              << large_iterations << " ops)" << std::endl;
    std::cout << "  Tiempo por operación (small): "
              << (small_time * 1000000.0 / small_iterations) << " ns"
              << std::endl;
    std::cout << "  Tiempo por operación (large): "
              << (large_time * 1000000.0 / large_iterations) << " ns"
              << std::endl;
    std::cout << std::endl;
  }

  void run_sequential_vs_random_access() {
    std::cout << "Acceso secuencial vs aleatorio:" << std::endl;

    const size_t iterations = 5000000;
    Timer timer;

    // Acceso secuencial
    timer.start();
    size_t sequential_successes = 0;
    for (size_t i = 0; i < iterations; ++i) {
      auto result = modern_get_power_of_2_int8(i % 7);
      if (result.has_value()) {
        sequential_successes++;
      }
    }
    double sequential_time = timer.stop_ms();

    // Acceso aleatorio
    auto random_indices = generator.generate_lookup_indices(iterations / 10, 6);
    timer.start();
    size_t random_successes = 0;
    for (size_t i = 0; i < iterations; ++i) {
      auto result =
          modern_get_power_of_2_int8(random_indices[i % random_indices.size()]);
      if (result.has_value()) {
        random_successes++;
      }
    }
    double random_time = timer.stop_ms();

    std::cout << "  Sequential: " << sequential_time << " ms ("
              << sequential_successes << " éxitos)" << std::endl;
    std::cout << "  Random: " << random_time << " ms (" << random_successes
              << " éxitos)" << std::endl;
    std::cout << "  Penalty: " << std::fixed << std::setprecision(2)
              << (random_time / sequential_time)
              << "x más lento el acceso aleatorio" << std::endl;
    std::cout << std::endl;
  }

public:
  void run_all_with_export(BenchmarkExporter &exporter) {
    std::cout << "=== BENCHMARKS DE LOOKUP TABLES ===" << std::endl;

    const size_t iterations = 5000000;
    Timer timer;

    // Sequential access benchmark
    timer.start();
    size_t sequential_successes = 0;
    for (size_t i = 0; i < iterations; ++i) {
      auto result = modern_get_power_of_2_int8(i % 7);
      if (result.has_value()) {
        sequential_successes++;
      }
    }
    double sequential_time = timer.stop_ms();

    // Crear resultado para acceso secuencial
    BenchmarkResult sequential_result;
    sequential_result.test_name = "Lookup Tables Sequential Access";
    sequential_result.iterations = iterations;
    sequential_result.successful_operations = sequential_successes;
    sequential_result.failed_operations = iterations - sequential_successes;
    sequential_result.avg_time_ns = (sequential_time * 1000000.0) / iterations;
    sequential_result.min_time_ns = sequential_result.avg_time_ns;
    sequential_result.max_time_ns = sequential_result.avg_time_ns;

    exporter.add_result(sequential_result);

    // Random access benchmark
    auto random_indices = generator.generate_lookup_indices(iterations / 10, 6);
    timer.start();
    size_t random_successes = 0;
    for (size_t i = 0; i < iterations; ++i) {
      auto result =
          modern_get_power_of_2_int8(random_indices[i % random_indices.size()]);
      if (result.has_value()) {
        random_successes++;
      }
    }
    double random_time = timer.stop_ms();

    // Crear resultado para acceso aleatorio
    BenchmarkResult random_result;
    random_result.test_name = "Lookup Tables Random Access";
    random_result.iterations = iterations;
    random_result.successful_operations = random_successes;
    random_result.failed_operations = iterations - random_successes;
    random_result.avg_time_ns = (random_time * 1000000.0) / iterations;
    random_result.min_time_ns = random_result.avg_time_ns;
    random_result.max_time_ns = random_result.avg_time_ns;

    exporter.add_result(random_result);

    std::cout << "Sequential: " << sequential_time << " ms ("
              << sequential_successes << " éxitos)" << std::endl;
    std::cout << "Random: " << random_time << " ms (" << random_successes
              << " éxitos)" << std::endl;
    std::cout << std::endl;

    // También ejecutar otros benchmarks sin exportar por compatibilidad
    run_different_sizes();
    run_cache_performance();
  }
};

//==============================================================================
// FUNCIÓN PRINCIPAL DE BENCHMARKING
//==============================================================================

int main() {
  std::cout
      << "=================================================================="
      << std::endl;
  std::cout << "    SISTEMA COMPLETO DE BENCHMARKING - C++17 std::optional"
            << std::endl;
  std::cout
      << "=================================================================="
      << std::endl;
  std::cout << std::endl;

  std::cout << "Información del sistema:" << std::endl;
  std::cout << "  Compilado con C++: " << __cplusplus << std::endl;
  std::cout << "  sizeof(void*): " << sizeof(void *) << " bytes" << std::endl;
  std::cout << "  sizeof(int): " << sizeof(int) << " bytes" << std::endl;
  std::cout << "  sizeof(size_t): " << sizeof(size_t) << " bytes" << std::endl;
  std::cout << std::endl;

  try {
    // Crear exportador de resultados
    BenchmarkExporter exporter;

    std::cout << "=== EJECUTANDO BENCHMARKS Y RECOLECTANDO RESULTADOS ==="
              << std::endl;

    // Ejecutar todos los benchmarks y recolectar resultados
    ErrorHandlingBenchmarks error_benchmarks;
    error_benchmarks.run_all_with_export(exporter);

    PureFunctionBenchmarks pure_benchmarks;
    pure_benchmarks.run_all_with_export(exporter);

    LookupTableBenchmarks lookup_benchmarks;
    lookup_benchmarks.run_all_with_export(exporter);

    std::cout
        << "=================================================================="
        << std::endl;
    std::cout << "                     BENCHMARKING COMPLETADO" << std::endl;
    std::cout
        << "=================================================================="
        << std::endl;

    // Exportar resultados a diferentes formatos
    std::cout << std::endl << "=== EXPORTANDO RESULTADOS ===" << std::endl;

    exporter.export_all_formats("comprehensive_benchmarks");
    exporter.export_comparison_report(
        "benchmark_results/comprehensive_comparison_report.md");

    std::cout << std::endl
              << "✅ Todos los resultados han sido exportados exitosamente."
              << std::endl;

  } catch (const std::exception &e) {
    std::cerr << "Error durante benchmarking: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}