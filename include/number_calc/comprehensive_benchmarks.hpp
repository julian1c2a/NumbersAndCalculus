#pragma once

/**
 * @file comprehensive_benchmarks.hpp
 * @brief Sistema completo de benchmarking para comparar rendimiento
 * @author AlgoritmiaCombinatoria Project
 * @date 2024
 * @version 1.0
 *
 * Este archivo proporciona un sistema extensivo de benchmarking para:
 * - Comparar CalculationResult vs std::optional
 * - Medir rendimiento de lookup tables
 * - Analizar eficiencia de memoria y cache
 * - Evaluar tiempos de compilación
 */

#include <array>
#include <chrono>
#include <cstdint>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <optional>
#include <random>
#include <sstream>
#include <string>
#include <vector>

// Incluir nuestros headers
#include "error_handling.hpp"

namespace number_calc {
namespace benchmarks {

//==============================================================================
// DEFINICIONES DE LEGACY SYSTEM PARA COMPARACIÓN
//==============================================================================

/**
 * @brief Sistema legacy CalculationResult para comparar con std::optional
 */
template <typename T> struct LegacyCalculationResult {
  T value;
  error_handling::CalculationError error;

  constexpr LegacyCalculationResult(T val) noexcept
      : value(val), error(error_handling::CalculationError::None) {}

  constexpr LegacyCalculationResult(
      error_handling::CalculationError err) noexcept
      : value{}, error(err) {}

  constexpr bool is_valid() const noexcept {
    return error == error_handling::CalculationError::None;
  }

  constexpr bool has_error() const noexcept {
    return error != error_handling::CalculationError::None;
  }

  constexpr T get_value() const noexcept { return value; }
  constexpr error_handling::CalculationError get_error() const noexcept {
    return error;
  }
};

template <typename T>
constexpr LegacyCalculationResult<T> make_legacy_success(T value) noexcept {
  return LegacyCalculationResult<T>(value);
}

template <typename T>
constexpr LegacyCalculationResult<T>
make_legacy_error(error_handling::CalculationError error) noexcept {
  return LegacyCalculationResult<T>(error);
}

//==============================================================================
// FUNCIONES DE PRUEBA PARA BENCHMARKING
//==============================================================================

// Lookup tables para testing
constexpr std::array<int8_t, 7> power_of_2_int8 = {1, 2, 4, 8, 16, 32, 64};
constexpr std::array<uint8_t, 8> power_of_2_uint8 = {1,  2,  4,  8,
                                                     16, 32, 64, 128};
constexpr std::array<int16_t, 15> power_of_2_int16 = {
    1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384};

/**
 * @brief Versión legacy con CalculationResult
 */
constexpr LegacyCalculationResult<int8_t>
legacy_get_power_of_2_int8(int exp) noexcept {
  if (exp < 0 || exp >= static_cast<int>(power_of_2_int8.size())) {
    return make_legacy_error<int8_t>(
        error_handling::CalculationError::OutOfRange);
  }
  return make_legacy_success(power_of_2_int8[exp]);
}

/**
 * @brief Versión moderna con std::optional
 */
constexpr std::optional<int8_t> modern_get_power_of_2_int8(int exp) noexcept {
  if (exp < 0 || exp >= static_cast<int>(power_of_2_int8.size())) {
    return std::nullopt;
  }
  return power_of_2_int8[exp];
}

/**
 * @brief Funciones de división para comparar
 */
constexpr LegacyCalculationResult<int> legacy_safe_divide(int a,
                                                          int b) noexcept {
  if (b == 0) {
    return make_legacy_error<int>(
        error_handling::CalculationError::DivisionByZero);
  }
  if (a == INT_MIN && b == -1) {
    return make_legacy_error<int>(error_handling::CalculationError::Overflow);
  }
  return make_legacy_success(a / b);
}

constexpr std::optional<int> modern_safe_divide(int a, int b) noexcept {
  if (b == 0) {
    return std::nullopt;
  }
  if (a == INT_MIN && b == -1) {
    return std::nullopt;
  }
  return a / b;
}

//==============================================================================
// UTILIDADES DE BENCHMARKING
//==============================================================================

/**
 * @brief Clase para medir tiempos de ejecución
 */
class Timer {
private:
  std::chrono::high_resolution_clock::time_point start_time;

public:
  void start() { start_time = std::chrono::high_resolution_clock::now(); }

  double stop_ms() {
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
        end_time - start_time);
    return duration.count() / 1000.0; // convertir a milisegundos
  }

  double stop_ns() {
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(
        end_time - start_time);
    return static_cast<double>(duration.count());
  }
};

/**
 * @brief Estructura para almacenar resultados de benchmark
 */
struct BenchmarkResult {
  std::string test_name;
  double avg_time_ns;
  double min_time_ns;
  double max_time_ns;
  size_t iterations;
  size_t successful_operations;
  size_t failed_operations;

  void print() const {
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "=== " << test_name << " ===" << std::endl;
    std::cout << "  Iterations: " << iterations << std::endl;
    std::cout << "  Successful: " << successful_operations << std::endl;
    std::cout << "  Failed: " << failed_operations << std::endl;
    std::cout << "  Avg time: " << avg_time_ns << " ns" << std::endl;
    std::cout << "  Min time: " << min_time_ns << " ns" << std::endl;
    std::cout << "  Max time: " << max_time_ns << " ns" << std::endl;
    std::cout << "  Operations/sec: " << static_cast<size_t>(1e9 / avg_time_ns)
              << std::endl;
    std::cout << std::endl;
  }
};

/**
 * @brief Función template para ejecutar benchmarks
 */
template <typename Function>
BenchmarkResult run_benchmark(const std::string &name, Function func,
                              size_t iterations = 1000000) {
  Timer timer;
  std::vector<double> times;
  times.reserve(iterations);

  size_t successful = 0;
  size_t failed = 0;

  // Calentamiento
  for (size_t i = 0; i < iterations / 10; ++i) {
    func();
  }

  // Medición real
  for (size_t i = 0; i < iterations; ++i) {
    timer.start();
    bool success = func();
    double time_ns = timer.stop_ns();
    times.push_back(time_ns);

    if (success) {
      successful++;
    } else {
      failed++;
    }
  }

  // Calcular estadísticas
  double sum = 0.0;
  double min_time = times[0];
  double max_time = times[0];

  for (double time : times) {
    sum += time;
    if (time < min_time)
      min_time = time;
    if (time > max_time)
      max_time = time;
  }

  return BenchmarkResult{name,       sum / iterations, min_time, max_time,
                         iterations, successful,       failed};
}

//==============================================================================
// GENERADORES DE DATOS DE PRUEBA
//==============================================================================

/**
 * @brief Generador de datos aleatorios para testing
 */
class TestDataGenerator {
private:
  mutable std::mt19937 gen;

public:
  TestDataGenerator() : gen(std::random_device{}()) {}

  std::vector<int> generate_random_ints(size_t count, int min = -1000,
                                        int max = 1000) const {
    std::uniform_int_distribution<int> dist(min, max);
    std::vector<int> data;
    data.reserve(count);

    for (size_t i = 0; i < count; ++i) {
      data.push_back(dist(gen));
    }

    return data;
  }

  std::vector<int> generate_lookup_indices(size_t count,
                                           int max_index = 6) const {
    std::uniform_int_distribution<int> dist(
        -2, max_index + 2); // Incluir algunos índices inválidos
    std::vector<int> data;
    data.reserve(count);

    for (size_t i = 0; i < count; ++i) {
      data.push_back(dist(gen));
    }

    return data;
  }

  std::vector<std::pair<int, int>> generate_division_pairs(size_t count) const {
    std::uniform_int_distribution<int> dist(-100, 100);
    std::vector<std::pair<int, int>> data;
    data.reserve(count);

    for (size_t i = 0; i < count; ++i) {
      int a = dist(gen);
      int b = dist(gen);
      data.emplace_back(a, b);
    }

    return data;
  }
};

//==============================================================================
// EXPORTADORES DE RESULTADOS
//==============================================================================

/**
 * @brief Clase para exportar resultados de benchmarks a diferentes formatos
 */
class BenchmarkExporter {
private:
  std::vector<BenchmarkResult> results;
  std::string timestamp;

public:
  BenchmarkExporter() {
    // Generar timestamp
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%Y%m%d_%H%M%S");
    timestamp = ss.str();
  }

  void add_result(const BenchmarkResult &result) { results.push_back(result); }

  void clear_results() { results.clear(); }

  // Exportar a CSV
  bool export_to_csv(const std::string &filename = "") const {
    std::string actual_filename =
        filename.empty()
            ? ("benchmark_results/benchmark_results_" + timestamp + ".csv")
            : filename;

    std::ofstream file(actual_filename);
    if (!file.is_open()) {
      std::cerr << "Error: No se pudo crear archivo CSV: " << actual_filename
                << std::endl;
      return false;
    }

    // Header CSV
    file << "Test Name,Iterations,Successful Operations,Failed Operations,";
    file << "Average Time (ns),Min Time (ns),Max Time (ns),Operations per "
            "Second\n";

    // Datos
    for (const auto &result : results) {
      file << "\"" << result.test_name << "\",";
      file << result.iterations << ",";
      file << result.successful_operations << ",";
      file << result.failed_operations << ",";
      file << std::fixed << std::setprecision(3) << result.avg_time_ns << ",";
      file << std::fixed << std::setprecision(3) << result.min_time_ns << ",";
      file << std::fixed << std::setprecision(3) << result.max_time_ns << ",";
      file << static_cast<size_t>(1e9 / result.avg_time_ns) << "\n";
    }

    file.close();
    std::cout << "> Resultados exportados a CSV: " << actual_filename
              << std::endl;
    return true;
  }

  // Exportar a Markdown
  bool export_to_markdown(const std::string &filename = "") const {
    std::string actual_filename =
        filename.empty()
            ? ("benchmark_results/benchmark_results_" + timestamp + ".md")
            : filename;

    std::ofstream file(actual_filename);
    if (!file.is_open()) {
      std::cerr << "Error: No se pudo crear archivo Markdown: "
                << actual_filename << std::endl;
      return false;
    }

    // Header Markdown
    file << "# Resultados de Benchmarking\n\n";
    file << "**Timestamp:** " << timestamp << "\n";
    file << "**Total de tests:** " << results.size() << "\n\n";

    // Resumen ejecutivo
    if (!results.empty()) {
      double total_avg_time = 0.0;
      size_t total_operations = 0;

      for (const auto &result : results) {
        total_avg_time += result.avg_time_ns;
        total_operations += result.successful_operations;
      }

      file << "## Resumen Ejecutivo\n\n";
      file << "- **Tiempo promedio general:** " << std::fixed
           << std::setprecision(3) << (total_avg_time / results.size())
           << " ns por operación\n";
      file << "- **Total de operaciones exitosas:** " << total_operations
           << "\n";
      file << "- **Throughput promedio:** "
           << static_cast<size_t>(1e9 / (total_avg_time / results.size()))
           << " ops/segundo\n\n";
    }

    // Tabla detallada
    file << "## Resultados Detallados\n\n";
    file << "| Test Name | Iterations | Success | Failed | Avg Time (ns) | Min "
            "Time (ns) | Max Time (ns) | Ops/Second |\n";
    file << "|-----------|------------|---------|--------|---------------|-----"
            "----------|---------------|------------|\n";

    for (const auto &result : results) {
      file << "| " << result.test_name << " | ";
      file << result.iterations << " | ";
      file << result.successful_operations << " | ";
      file << result.failed_operations << " | ";
      file << std::fixed << std::setprecision(3) << result.avg_time_ns << " | ";
      file << std::fixed << std::setprecision(3) << result.min_time_ns << " | ";
      file << std::fixed << std::setprecision(3) << result.max_time_ns << " | ";
      file << static_cast<size_t>(1e9 / result.avg_time_ns) << " |\n";
    }

    // Análisis de rendimiento
    file << "\n## Análisis de Rendimiento\n\n";

    // Buscar tests comparativos (legacy vs modern)
    for (size_t i = 0; i < results.size(); ++i) {
      if (results[i].test_name.find("Legacy") != std::string::npos) {
        for (size_t j = 0; j < results.size(); ++j) {
          if (i != j &&
              results[j].test_name.find("Modern") != std::string::npos) {
            // Calcular speedup
            double speedup = results[i].avg_time_ns / results[j].avg_time_ns;
            file << "### " << results[i].test_name << " vs "
                 << results[j].test_name << "\n\n";
            file << "- **Speedup:** " << std::fixed << std::setprecision(2)
                 << speedup << "x ";
            if (speedup > 1.0) {
              file << "(Modern es más rápido)\n";
            } else {
              file << "(Legacy es más rápido)\n";
            }
            file << "- **Diferencia:** " << std::fixed << std::setprecision(3)
                 << std::abs(results[i].avg_time_ns - results[j].avg_time_ns)
                 << " ns\n\n";
            break;
          }
        }
      }
    }

    file.close();
    std::cout << "> Resultados exportados a Markdown: " << actual_filename
              << std::endl;
    return true;
  }

  // Exportar a JSON
  bool export_to_json(const std::string &filename = "") const {
    std::string actual_filename =
        filename.empty()
            ? ("benchmark_results/benchmark_results_" + timestamp + ".json")
            : filename;

    std::ofstream file(actual_filename);
    if (!file.is_open()) {
      std::cerr << "Error: No se pudo crear archivo JSON: " << actual_filename
                << std::endl;
      return false;
    }

    file << "{\n";
    file << "  \"timestamp\": \"" << timestamp << "\",\n";
    file << "  \"total_tests\": " << results.size() << ",\n";
    file << "  \"results\": [\n";

    for (size_t i = 0; i < results.size(); ++i) {
      const auto &result = results[i];
      file << "    {\n";
      file << "      \"test_name\": \"" << result.test_name << "\",\n";
      file << "      \"iterations\": " << result.iterations << ",\n";
      file << "      \"successful_operations\": "
           << result.successful_operations << ",\n";
      file << "      \"failed_operations\": " << result.failed_operations
           << ",\n";
      file << "      \"avg_time_ns\": " << std::fixed << std::setprecision(3)
           << result.avg_time_ns << ",\n";
      file << "      \"min_time_ns\": " << std::fixed << std::setprecision(3)
           << result.min_time_ns << ",\n";
      file << "      \"max_time_ns\": " << std::fixed << std::setprecision(3)
           << result.max_time_ns << ",\n";
      file << "      \"operations_per_second\": "
           << static_cast<size_t>(1e9 / result.avg_time_ns) << "\n";
      file << "    }";
      if (i < results.size() - 1)
        file << ",";
      file << "\n";
    }

    file << "  ]\n";
    file << "}\n";

    file.close();
    std::cout << "> Resultados exportados a JSON: " << actual_filename
              << std::endl;
    return true;
  }

  // Exportar todos los formatos
  void export_all_formats(const std::string &base_name = "") const {
    std::string base =
        base_name.empty() ? ("benchmark_results_" + timestamp) : base_name;

    export_to_csv("benchmark_results/" + base + ".csv");
    export_to_markdown("benchmark_results/" + base + ".md");
    export_to_json("benchmark_results/" + base + ".json");

    std::cout << "\n> Todos los formatos exportados con base: " << base
              << std::endl;
  }

  // Generar reporte de comparación específico
  void export_comparison_report(const std::string &filename = "") const {
    std::string actual_filename =
        filename.empty()
            ? ("benchmark_results/comparison_report_" + timestamp + ".md")
            : filename;

    std::ofstream file(actual_filename);
    if (!file.is_open()) {
      std::cerr << "Error: No se pudo crear reporte de comparación: "
                << actual_filename << std::endl;
      return;
    }

    file << "# Reporte de Comparación: CalculationResult vs std::optional\n\n";
    file << "**Generado:** " << timestamp << "\n\n";

    // Buscar pares de comparación
    std::vector<std::pair<size_t, size_t>> comparison_pairs;
    for (size_t i = 0; i < results.size(); ++i) {
      if (results[i].test_name.find("Legacy") != std::string::npos) {
        for (size_t j = 0; j < results.size(); ++j) {
          if (i != j &&
              results[j].test_name.find("Modern") != std::string::npos) {
            comparison_pairs.emplace_back(i, j);
            break;
          }
        }
      }
    }

    if (comparison_pairs.empty()) {
      file << "No se encontraron pares de comparación Legacy vs Modern.\n";
      file.close();
      return;
    }

    // Análisis detallado
    file << "## Resumen de Comparaciones\n\n";

    double total_speedup = 0.0;
    size_t modern_wins = 0;
    size_t legacy_wins = 0;

    for (const auto &pair : comparison_pairs) {
      const auto &legacy = results[pair.first];
      const auto &modern = results[pair.second];

      double speedup = legacy.avg_time_ns / modern.avg_time_ns;
      total_speedup += speedup;

      if (speedup > 1.0) {
        modern_wins++;
      } else {
        legacy_wins++;
      }
    }

    file << "- **Total de comparaciones:** " << comparison_pairs.size() << "\n";
    file << "- **std::optional gana:** " << modern_wins << " tests\n";
    file << "- **CalculationResult gana:** " << legacy_wins << " tests\n";
    file << "- **Speedup promedio:** " << std::fixed << std::setprecision(2)
         << (total_speedup / comparison_pairs.size()) << "x\n\n";

    file << "## Comparaciones Detalladas\n\n";

    for (const auto &pair : comparison_pairs) {
      const auto &legacy = results[pair.first];
      const auto &modern = results[pair.second];

      double speedup = legacy.avg_time_ns / modern.avg_time_ns;

      file << "### " << legacy.test_name << " vs " << modern.test_name
           << "\n\n";
      file << "| Métrica | CalculationResult | std::optional | Ratio |\n";
      file << "|---------|-------------------|---------------|-------|\n";
      file << "| Avg Time (ns) | " << std::fixed << std::setprecision(3)
           << legacy.avg_time_ns;
      file << " | " << std::fixed << std::setprecision(3) << modern.avg_time_ns;
      file << " | " << std::fixed << std::setprecision(2) << speedup << "x |\n";
      file << "| Ops/Second | "
           << static_cast<size_t>(1e9 / legacy.avg_time_ns);
      file << " | " << static_cast<size_t>(1e9 / modern.avg_time_ns);
      file << " | " << std::fixed << std::setprecision(2)
           << (modern.avg_time_ns / legacy.avg_time_ns) << "x |\n";
      file << "| Success Rate | " << std::fixed << std::setprecision(1)
           << (100.0 * legacy.successful_operations / legacy.iterations) << "%";
      file << " | " << std::fixed << std::setprecision(1)
           << (100.0 * modern.successful_operations / modern.iterations)
           << "% | - |\n\n";

      if (speedup > 1.05) {
        file << "**🏆 Winner: std::optional** - " << std::fixed
             << std::setprecision(1) << ((speedup - 1.0) * 100)
             << "% más rápido\n\n";
      } else if (speedup < 0.95) {
        file << "**🏆 Winner: CalculationResult** - " << std::fixed
             << std::setprecision(1) << ((1.0 / speedup - 1.0) * 100)
             << "% más rápido\n\n";
      } else {
        file << "**🤝 Empate técnico** - Diferencia < 5%\n\n";
      }
    }

    // Recomendaciones
    file << "## Recomendaciones\n\n";
    if (modern_wins > legacy_wins) {
      file << "> **Recomendacion: Migrar a std::optional**\n\n";
      file << "std::optional demuestra mejor rendimiento en la mayoría de "
              "casos de prueba. ";
      file << "Además, ofrece ventajas adicionales:\n\n";
      file << "- Estándar C++17 reconocido\n";
      file << "- Mejor soporte de herramientas\n";
      file << "- Sintaxis más limpia\n";
      file << "- Menos código que mantener\n\n";
    } else if (legacy_wins > modern_wins) {
      file << "⚠️ **Recomendación: Evaluar caso por caso**\n\n";
      file << "CalculationResult muestra ventajas en algunos casos. "
              "Considerar:\n\n";
      file << "- Mantener CalculationResult para casos críticos de "
              "rendimiento\n";
      file << "- Migrar gradualmente funciones menos críticas\n";
      file << "- Realizar profiling en aplicaciones reales\n\n";
    } else {
      file << "🤝 **Recomendación: Migración basada en mantenibilidad**\n\n";
      file << "Rendimiento similar en ambos sistemas. La decisión debe basarse "
              "en:\n\n";
      file << "- Preferir std::optional por ser estándar\n";
      file << "- Considerar el costo de migración\n";
      file << "- Evaluar la experiencia del equipo\n\n";
    }

    file.close();
    std::cout << "> Reporte de comparacion exportado: " << actual_filename
              << std::endl;
  }
};

} // namespace benchmarks
} // namespace number_calc