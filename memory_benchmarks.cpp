#include <algorithm>
#include <chrono>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <optional>
#include <random>
#include <vector>


// NOTA: Benchmark especializado en análisis de memoria y cache para
// std::optional

namespace memory_benchmarks {

//==============================================================================
// ANÁLISIS DE CACHE Y MEMORIA
//==============================================================================

class CacheAnalysisBenchmarks {
private:
  std::mt19937 gen;

public:
  CacheAnalysisBenchmarks() : gen(std::random_device{}()) {}

  void run_all() {
    std::cout << "### ANÁLISIS DETALLADO DE CACHE Y MEMORIA ###" << std::endl;

    analyze_cache_line_effects();
    analyze_memory_layout();
    analyze_false_sharing();
    analyze_prefetching();
  }

private:
  void analyze_cache_line_effects() {
    std::cout << "Análisis de efectos de cache line:" << std::endl;

    const size_t size = 64 * 1024 * 1024 / sizeof(std::optional<int>); // ~64MB
    std::vector<std::optional<int>> data(size);

    // Inicializar datos
    for (size_t i = 0; i < size; ++i) {
      data[i] = static_cast<int>(i % 1000);
    }

    // Test 1: Acceso secuencial (cache-friendly)
    auto start = std::chrono::high_resolution_clock::now();
    volatile long long sum = 0;
    for (size_t i = 0; i < size; ++i) {
      if (data[i].has_value()) {
        sum += *data[i];
      }
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto sequential_time =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    // Test 2: Acceso con stride (cache-unfriendly)
    const size_t stride = 64; // Saltar cache lines
    start = std::chrono::high_resolution_clock::now();
    sum = 0;
    for (size_t i = 0; i < size; i += stride) {
      if (data[i].has_value()) {
        sum += *data[i];
      }
    }
    end = std::chrono::high_resolution_clock::now();
    auto strided_time =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "  Sequential access: " << sequential_time.count() << " ms"
              << std::endl;
    std::cout << "  Strided access (stride=" << stride
              << "): " << strided_time.count() << " ms" << std::endl;
    std::cout << "  Cache penalty: " << std::fixed << std::setprecision(2)
              << (static_cast<double>(strided_time.count()) /
                  sequential_time.count())
              << "x" << std::endl;
    std::cout << std::endl;
  }

  void analyze_memory_layout() {
    std::cout << "Análisis de layout de memoria:" << std::endl;

    // Comparar arrays de std::optional vs arrays separados
    const size_t size = 1000000;

    // Método 1: Array de std::optional
    std::vector<std::optional<int>> optional_array(size);

    // Método 2: Arrays separados (values + validity flags)
    std::vector<int> values_array(size);
    std::vector<bool> valid_array(size);

    // Inicializar datos
    std::uniform_int_distribution<int> dist(1, 1000);
    for (size_t i = 0; i < size; ++i) {
      int value = dist(gen);
      bool is_valid = (i % 7 != 0); // ~85% válidos

      if (is_valid) {
        optional_array[i] = value;
        values_array[i] = value;
        valid_array[i] = true;
      } else {
        optional_array[i] = std::nullopt;
        values_array[i] = 0;
        valid_array[i] = false;
      }
    }

    // Test 1: Procesar array de std::optional
    auto start = std::chrono::high_resolution_clock::now();
    volatile long long sum1 = 0;
    for (size_t i = 0; i < size; ++i) {
      if (optional_array[i].has_value()) {
        sum1 += *optional_array[i];
      }
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto optional_time =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    // Test 2: Procesar arrays separados
    start = std::chrono::high_resolution_clock::now();
    volatile long long sum2 = 0;
    for (size_t i = 0; i < size; ++i) {
      if (valid_array[i]) {
        sum2 += values_array[i];
      }
    }
    end = std::chrono::high_resolution_clock::now();
    auto separated_time =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << "  std::optional array: " << optional_time.count() << " μs"
              << std::endl;
    std::cout << "  Separated arrays: " << separated_time.count() << " μs"
              << std::endl;
    std::cout << "  Memory layout penalty: " << std::fixed
              << std::setprecision(2)
              << (static_cast<double>(optional_time.count()) /
                  separated_time.count())
              << "x" << std::endl;

    std::cout << "  Memory usage:" << std::endl;
    std::cout << "    std::optional array: "
              << (sizeof(std::optional<int>) * size / 1024) << " KB"
              << std::endl;
    std::cout << "    Separated arrays: "
              << ((sizeof(int) + sizeof(bool)) * size / 1024) << " KB"
              << std::endl;
    std::cout << std::endl;
  }

  void analyze_false_sharing() {
    std::cout << "Análisis de false sharing:" << std::endl;

    const size_t size = 1000000;
    const size_t cache_line_size = 64; // bytes
    const size_t elements_per_cache_line =
        cache_line_size / sizeof(std::optional<int>);

    std::vector<std::optional<int>> data(size);

    // Inicializar datos
    for (size_t i = 0; i < size; ++i) {
      data[i] = static_cast<int>(i % 1000);
    }

    // Test 1: Acceso que evita false sharing
    auto start = std::chrono::high_resolution_clock::now();
    volatile long long sum = 0;
    for (size_t i = 0; i < size; i += elements_per_cache_line) {
      if (data[i].has_value()) {
        sum += *data[i];
      }
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto no_sharing_time =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    // Test 2: Acceso que puede causar false sharing
    start = std::chrono::high_resolution_clock::now();
    sum = 0;
    for (size_t i = 1; i < size; i += elements_per_cache_line) { // Offset by 1
      if (data[i].has_value()) {
        sum += *data[i];
      }
    }
    end = std::chrono::high_resolution_clock::now();
    auto potential_sharing_time =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << "  Cache line aligned access: " << no_sharing_time.count()
              << " μs" << std::endl;
    std::cout << "  Potentially unaligned access: "
              << potential_sharing_time.count() << " μs" << std::endl;
    std::cout << "  Elements per cache line: " << elements_per_cache_line
              << std::endl;
    std::cout << std::endl;
  }

  void analyze_prefetching() {
    std::cout << "Análisis de hardware prefetching:" << std::endl;

    const size_t size = 10000000;
    std::vector<std::optional<int>> data(size);

    // Inicializar datos
    for (size_t i = 0; i < size; ++i) {
      data[i] = static_cast<int>(i % 1000);
    }

    // Test 1: Patrón predecible (prefetcher-friendly)
    auto start = std::chrono::high_resolution_clock::now();
    volatile long long sum = 0;
    for (size_t i = 0; i < size; i += 2) { // Patrón regular
      if (data[i].has_value()) {
        sum += *data[i];
      }
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto predictable_time =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    // Test 2: Patrón impredecible (prefetcher-unfriendly)
    std::vector<size_t> random_indices;
    random_indices.reserve(size / 2);
    for (size_t i = 0; i < size; i += 2) {
      random_indices.push_back(i);
    }
    std::shuffle(random_indices.begin(), random_indices.end(), gen);

    start = std::chrono::high_resolution_clock::now();
    sum = 0;
    for (size_t idx : random_indices) {
      if (data[idx].has_value()) {
        sum += *data[idx];
      }
    }
    end = std::chrono::high_resolution_clock::now();
    auto unpredictable_time =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "  Predictable pattern: " << predictable_time.count() << " ms"
              << std::endl;
    std::cout << "  Unpredictable pattern: " << unpredictable_time.count()
              << " ms" << std::endl;
    std::cout << "  Prefetcher penalty: " << std::fixed << std::setprecision(2)
              << (static_cast<double>(unpredictable_time.count()) /
                  predictable_time.count())
              << "x" << std::endl;
    std::cout << std::endl;
  }
};

//==============================================================================
// ANÁLISIS DE FRAGMENTACIÓN DE MEMORIA
//==============================================================================

class MemoryFragmentationBenchmarks {
public:
  void run_all() {
    std::cout << "### ANÁLISIS DE FRAGMENTACIÓN DE MEMORIA ###" << std::endl;

    analyze_allocation_patterns();
    analyze_container_growth();
  }

private:
  void analyze_allocation_patterns() {
    std::cout << "Análisis de patrones de asignación:" << std::endl;

    const size_t iterations = 100000;

    // Test 1: Asignaciones continuas
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::vector<std::optional<int>>> continuous_vectors;
    for (size_t i = 0; i < iterations; ++i) {
      std::vector<std::optional<int>> vec(100);
      for (size_t j = 0; j < 100; ++j) {
        vec[j] = static_cast<int>(j);
      }
      continuous_vectors.push_back(std::move(vec));
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto continuous_time =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    // Test 2: Asignaciones intercaladas con liberaciones
    start = std::chrono::high_resolution_clock::now();

    std::vector<std::vector<std::optional<int>>> fragmented_vectors;
    for (size_t i = 0; i < iterations; ++i) {
      std::vector<std::optional<int>> vec(100);
      for (size_t j = 0; j < 100; ++j) {
        vec[j] = static_cast<int>(j);
      }
      fragmented_vectors.push_back(std::move(vec));

      // Cada 10 iteraciones, liberar algunos vectores
      if (i % 10 == 0 && fragmented_vectors.size() > 5) {
        fragmented_vectors.erase(fragmented_vectors.begin(),
                                 fragmented_vectors.begin() + 3);
      }
    }

    end = std::chrono::high_resolution_clock::now();
    auto fragmented_time =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "  Continuous allocations: " << continuous_time.count()
              << " ms" << std::endl;
    std::cout << "  Fragmented allocations: " << fragmented_time.count()
              << " ms" << std::endl;
    std::cout << "  Fragmentation penalty: " << std::fixed
              << std::setprecision(2)
              << (static_cast<double>(fragmented_time.count()) /
                  continuous_time.count())
              << "x" << std::endl;
    std::cout << std::endl;
  }

  void analyze_container_growth() {
    std::cout << "Análisis de crecimiento de contenedores:" << std::endl;

    // Test con diferentes estrategias de reserva
    const size_t final_size = 1000000;

    // Test 1: Sin reserva (muchas reasignaciones)
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::optional<int>> no_reserve_vec;
    for (size_t i = 0; i < final_size; ++i) {
      no_reserve_vec.push_back(static_cast<int>(i % 1000));
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto no_reserve_time =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    // Test 2: Con reserva (sin reasignaciones)
    start = std::chrono::high_resolution_clock::now();

    std::vector<std::optional<int>> reserved_vec;
    reserved_vec.reserve(final_size);
    for (size_t i = 0; i < final_size; ++i) {
      reserved_vec.push_back(static_cast<int>(i % 1000));
    }

    end = std::chrono::high_resolution_clock::now();
    auto reserved_time =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "  Without reserve(): " << no_reserve_time.count() << " ms"
              << std::endl;
    std::cout << "  With reserve(): " << reserved_time.count() << " ms"
              << std::endl;
    std::cout << "  Reserve speedup: " << std::fixed << std::setprecision(2)
              << (static_cast<double>(no_reserve_time.count()) /
                  reserved_time.count())
              << "x" << std::endl;
    std::cout << std::endl;
  }
};

} // namespace memory_benchmarks

//==============================================================================
// FUNCIÓN PRINCIPAL
//==============================================================================

int main() {
  std::cout
      << "=================================================================="
      << std::endl;
  std::cout << "        BENCHMARKS DE MEMORIA Y CACHE - std::optional"
            << std::endl;
  std::cout
      << "=================================================================="
      << std::endl;
  std::cout << std::endl;

  std::cout << "Información del sistema:" << std::endl;
  std::cout << "  sizeof(std::optional<int>): " << sizeof(std::optional<int>)
            << " bytes" << std::endl;
  std::cout << "  sizeof(std::optional<int8_t>): "
            << sizeof(std::optional<int8_t>) << " bytes" << std::endl;
  std::cout << "  sizeof(std::optional<int64_t>): "
            << sizeof(std::optional<int64_t>) << " bytes" << std::endl;
  std::cout << "  alignof(std::optional<int>): " << alignof(std::optional<int>)
            << " bytes" << std::endl;
  std::cout << std::endl;

  try {
    memory_benchmarks::CacheAnalysisBenchmarks cache_benchmarks;
    cache_benchmarks.run_all();

    memory_benchmarks::MemoryFragmentationBenchmarks frag_benchmarks;
    frag_benchmarks.run_all();

    std::cout
        << "=================================================================="
        << std::endl;
    std::cout << "              BENCHMARKS DE MEMORIA COMPLETADOS" << std::endl;
    std::cout
        << "=================================================================="
        << std::endl;

  } catch (const std::exception &e) {
    std::cerr << "Error durante benchmarking: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}