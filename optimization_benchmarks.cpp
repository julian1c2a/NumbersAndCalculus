#include <chrono>
#include <iomanip>
#include <iostream>
#include <optional>
#include <random>
#include <vector>

// NOTA: Este archivo está diseñado para analizar optimizaciones del compilador
// Compilar con diferentes niveles de optimización: -O0, -O1, -O2, -O3, -Os

namespace compilation_benchmarks {

//==============================================================================
// TESTS DE OPTIMIZACIÓN DEL COMPILADOR
//==============================================================================

/**
 * @brief Test específico para analizar inlining de std::optional
 */
class OptimizationBenchmarks {
private:
  std::mt19937 gen;

public:
  OptimizationBenchmarks() : gen(std::random_device{}()) {}

  void run_all() {
    std::cout << "### BENCHMARKS DE OPTIMIZACIÓN DEL COMPILADOR ###"
              << std::endl;

    run_inlining_tests();
    run_constant_folding_tests();
    run_loop_optimization_tests();
    run_branch_elimination_tests();
  }

private:
  void run_inlining_tests() {
    std::cout << "Test de inlining de std::optional:" << std::endl;

    const size_t iterations = 100000000;
    auto start = std::chrono::high_resolution_clock::now();

    volatile int sum = 0;
    for (size_t i = 0; i < iterations; ++i) {
      auto result = inline_optional_test(static_cast<int>(i % 100));
      if (result.has_value()) {
        sum += *result;
      }
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "  Inlined optional operations: " << duration.count() << " ms"
              << std::endl;
    std::cout << "  Operaciones por segundo: "
              << (iterations / (duration.count() / 1000.0)) << std::endl;
    std::cout << std::endl;
  }

  __attribute__((always_inline)) inline std::optional<int>
  inline_optional_test(int x) const {
    if (x < 0)
      return std::nullopt;
    if (x > 50)
      return std::nullopt;
    return x * x;
  }

  constexpr std::optional<int> constexpr_optional_test(int x) const {
    if (x < 0)
      return std::nullopt;
    return x * x + x;
  }

  void run_constant_folding_tests() {
    std::cout << "Test de constant folding:" << std::endl;

    const size_t iterations = 200000000;
    auto start = std::chrono::high_resolution_clock::now();

    volatile int sum = 0;
    for (size_t i = 0; i < iterations; ++i) {
      // El compilador debería optimizar estas operaciones constantes
      constexpr auto result1 = constexpr_optional_test(5);
      constexpr auto result2 = constexpr_optional_test(10);
      constexpr auto result3 = constexpr_optional_test(-1);

      if (result1.has_value())
        sum += *result1;
      if (result2.has_value())
        sum += *result2;
      if (result3.has_value())
        sum += *result3;
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "  Constant folding operations: " << duration.count() << " ms"
              << std::endl;
    std::cout << "  Operaciones por segundo: "
              << (iterations * 3 / (duration.count() / 1000.0)) << std::endl;
    std::cout << std::endl;
  }

  void run_loop_optimization_tests() {
    std::cout << "Test de optimización de loops:" << std::endl;

    const size_t iterations = 10000;
    const size_t inner_iterations = 10000;

    auto start = std::chrono::high_resolution_clock::now();

    volatile long long total = 0;
    for (size_t i = 0; i < iterations; ++i) {
      for (size_t j = 0; j < inner_iterations; ++j) {
        auto result = loop_body_optional(static_cast<int>(j % 100));
        if (result.has_value()) {
          total += *result;
        }
      }
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "  Nested loop optimization: " << duration.count() << " ms"
              << std::endl;
    std::cout << "  Total operaciones: " << (iterations * inner_iterations)
              << std::endl;
    std::cout << "  Operaciones por segundo: "
              << (iterations * inner_iterations / (duration.count() / 1000.0))
              << std::endl;
    std::cout << std::endl;
  }

  std::optional<long long> loop_body_optional(int x) const {
    if (x % 7 == 0)
      return std::nullopt;
    return static_cast<long long>(x) * x;
  }

  void run_branch_elimination_tests() {
    std::cout << "Test de eliminación de branches:" << std::endl;

    const size_t iterations = 50000000;

    // Test con branch predecible
    auto start = std::chrono::high_resolution_clock::now();

    volatile int predictable_sum = 0;
    for (size_t i = 0; i < iterations; ++i) {
      // Patrón muy predecible: cada 10mo elemento es error
      auto result =
          predictable_branch_test(i % 10 == 0 ? -1 : static_cast<int>(i % 50));
      if (result.has_value()) {
        predictable_sum += *result;
      }
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto predictable_duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    // Test con branch impredecible
    std::vector<int> random_values;
    random_values.reserve(iterations / 1000);
    std::uniform_int_distribution<int> dist(-10, 60);
    for (size_t i = 0; i < iterations / 1000; ++i) {
      random_values.push_back(dist(gen));
    }

    start = std::chrono::high_resolution_clock::now();

    volatile int unpredictable_sum = 0;
    for (size_t i = 0; i < iterations; ++i) {
      auto result =
          predictable_branch_test(random_values[i % random_values.size()]);
      if (result.has_value()) {
        unpredictable_sum += *result;
      }
    }

    end = std::chrono::high_resolution_clock::now();
    auto unpredictable_duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "  Predictable branches: " << predictable_duration.count()
              << " ms" << std::endl;
    std::cout << "  Unpredictable branches: " << unpredictable_duration.count()
              << " ms" << std::endl;
    std::cout << "  Branch misprediction penalty: " << std::fixed
              << std::setprecision(2)
              << (static_cast<double>(unpredictable_duration.count()) /
                  predictable_duration.count())
              << "x" << std::endl;
    std::cout << std::endl;
  }

  std::optional<int> predictable_branch_test(int x) const {
    if (x < 0)
      return std::nullopt;
    if (x > 50)
      return std::nullopt;
    return x + 1;
  }
};

//==============================================================================
// BENCHMARKS DE DIFERENTES TAMAÑOS DE DATOS
//==============================================================================

class DataSizeBenchmarks {
public:
  void run_all() {
    std::cout << "### BENCHMARKS DE DIFERENTES TAMAÑOS DE DATOS ###"
              << std::endl;

    run_small_data_tests();
    run_medium_data_tests();
    run_large_data_tests();
  }

private:
  void run_small_data_tests() {
    std::cout << "Test con tipos pequeños (int8_t):" << std::endl;

    const size_t iterations = 50000000;
    auto start = std::chrono::high_resolution_clock::now();

    volatile int8_t sum = 0;
    for (size_t i = 0; i < iterations; ++i) {
      auto result = small_data_optional(static_cast<int8_t>(i % 100));
      if (result.has_value()) {
        sum += *result;
      }
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "  int8_t operations: " << duration.count() << " ms"
              << std::endl;
    std::cout << "  sizeof(std::optional<int8_t>): "
              << sizeof(std::optional<int8_t>) << " bytes" << std::endl;
    std::cout << std::endl;
  }

  std::optional<int8_t> small_data_optional(int8_t x) const {
    if (x < 0)
      return std::nullopt;
    if (x > 50)
      return std::nullopt;
    return static_cast<int8_t>(x + 1);
  }

  void run_medium_data_tests() {
    std::cout << "Test con tipos medianos (int32_t):" << std::endl;

    const size_t iterations = 50000000;
    auto start = std::chrono::high_resolution_clock::now();

    volatile int32_t sum = 0;
    for (size_t i = 0; i < iterations; ++i) {
      auto result = medium_data_optional(static_cast<int32_t>(i % 1000));
      if (result.has_value()) {
        sum += *result;
      }
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "  int32_t operations: " << duration.count() << " ms"
              << std::endl;
    std::cout << "  sizeof(std::optional<int32_t>): "
              << sizeof(std::optional<int32_t>) << " bytes" << std::endl;
    std::cout << std::endl;
  }

  std::optional<int32_t> medium_data_optional(int32_t x) const {
    if (x < 0)
      return std::nullopt;
    if (x > 500)
      return std::nullopt;
    return x * 2;
  }

  void run_large_data_tests() {
    std::cout << "Test con tipos grandes (int64_t):" << std::endl;

    const size_t iterations = 50000000;
    auto start = std::chrono::high_resolution_clock::now();

    volatile int64_t sum = 0;
    for (size_t i = 0; i < iterations; ++i) {
      auto result = large_data_optional(static_cast<int64_t>(i % 10000));
      if (result.has_value()) {
        sum += *result;
      }
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "  int64_t operations: " << duration.count() << " ms"
              << std::endl;
    std::cout << "  sizeof(std::optional<int64_t>): "
              << sizeof(std::optional<int64_t>) << " bytes" << std::endl;
    std::cout << std::endl;
  }

  std::optional<int64_t> large_data_optional(int64_t x) const {
    if (x < 0)
      return std::nullopt;
    if (x > 5000)
      return std::nullopt;
    return x * x;
  }
};

//==============================================================================
// BENCHMARKS DE VECTORIZACIÓN
//==============================================================================

class VectorizationBenchmarks {
public:
  void run_all() {
    std::cout << "### BENCHMARKS DE VECTORIZACIÓN ###" << std::endl;

    run_vectorizable_loop();
    run_non_vectorizable_loop();
  }

private:
  void run_vectorizable_loop() {
    std::cout << "Test de loop vectorizable:" << std::endl;

    const size_t size = 10000000;
    std::vector<int> input(size);
    std::vector<int> output(size, 0);

    // Inicializar datos
    for (size_t i = 0; i < size; ++i) {
      input[i] = static_cast<int>(i % 1000);
    }

    auto start = std::chrono::high_resolution_clock::now();

    // Loop que puede ser vectorizado
    for (size_t i = 0; i < size; ++i) {
      if (input[i] >= 0 && input[i] < 500) {
        output[i] = input[i] * 2;
      } else {
        output[i] = 0; // Error case
      }
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "  Vectorizable loop: " << duration.count() << " ms"
              << std::endl;
    std::cout << "  Elementos procesados: " << size << std::endl;
    std::cout << std::endl;
  }

  void run_non_vectorizable_loop() {
    std::cout << "Test de loop no vectorizable (con std::optional):"
              << std::endl;

    const size_t size = 10000000;
    std::vector<int> input(size);
    std::vector<std::optional<int>> output(size);

    // Inicializar datos
    for (size_t i = 0; i < size; ++i) {
      input[i] = static_cast<int>(i % 1000);
    }

    auto start = std::chrono::high_resolution_clock::now();

    // Loop con std::optional (más difícil de vectorizar)
    for (size_t i = 0; i < size; ++i) {
      output[i] = process_with_optional(input[i]);
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    size_t valid_count = 0;
    for (const auto &opt : output) {
      if (opt.has_value())
        valid_count++;
    }

    std::cout << "  Non-vectorizable loop: " << duration.count() << " ms"
              << std::endl;
    std::cout << "  Elementos válidos: " << valid_count << "/" << size
              << std::endl;
    std::cout << std::endl;
  }

  std::optional<int> process_with_optional(int x) const {
    if (x < 0)
      return std::nullopt;
    if (x >= 500)
      return std::nullopt;
    return x * 2;
  }
};

} // namespace compilation_benchmarks

//==============================================================================
// FUNCIÓN PRINCIPAL
//==============================================================================

int main() {
  std::cout
      << "=================================================================="
      << std::endl;
  std::cout << "      BENCHMARKS DE OPTIMIZACIÓN Y COMPILACIÓN - C++17"
            << std::endl;
  std::cout
      << "=================================================================="
      << std::endl;
  std::cout << std::endl;

  std::cout << "Compilado con:" << std::endl;
  std::cout << "  C++ standard: " << __cplusplus << std::endl;

#ifdef __OPTIMIZE__
  std::cout << "  Optimización: Habilitada" << std::endl;
#else
  std::cout << "  Optimización: Deshabilitada" << std::endl;
#endif

#ifdef __GNUC__
  std::cout << "  Compilador: GCC " << __GNUC__ << "." << __GNUC_MINOR__ << "."
            << __GNUC_PATCHLEVEL__ << std::endl;
#endif

  std::cout << std::endl;

  try {
    compilation_benchmarks::OptimizationBenchmarks opt_benchmarks;
    opt_benchmarks.run_all();

    compilation_benchmarks::DataSizeBenchmarks size_benchmarks;
    size_benchmarks.run_all();

    compilation_benchmarks::VectorizationBenchmarks vec_benchmarks;
    vec_benchmarks.run_all();

    std::cout
        << "=================================================================="
        << std::endl;
    std::cout << "             BENCHMARKS DE OPTIMIZACIÓN COMPLETADOS"
              << std::endl;
    std::cout
        << "=================================================================="
        << std::endl;

  } catch (const std::exception &e) {
    std::cerr << "Error durante benchmarking: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}