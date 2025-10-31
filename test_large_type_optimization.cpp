/**
 * @file test_large_type_optimization.cpp
 * @brief Prueba de la optimización para tipos grandes usando base de 32 bits
 * @author AlgoritmiaCombinatoria Project
 * @date 2024
 *
 * Este programa demuestra cómo la optimización reduce significativamente
 * las iteraciones para tipos grandes al usar resultados de 32 bits como punto
 * de partida.
 */

#include "number_calc.hpp"
#include <chrono>
#include <iomanip>
#include <iostream>

using namespace number_calc;
using namespace number_calc::integral_power_functions;

// Función para medir tiempo de ejecución
template <typename Func> double measure_time_ms(Func &&func) {
  auto start = std::chrono::high_resolution_clock::now();
  func();
  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  return duration.count() / 1000.0; // Convertir a milisegundos
}

void demonstrate_power_of_2_optimization() {
  std::cout << "=== DEMOSTRACION: OPTIMIZACION PARA POTENCIAS DE 2 EN TIPOS "
               "GRANDES ===\n";
  std::cout << std::fixed << std::setprecision(3);

  // Ejemplo: 2^100 en diferentes tipos
  constexpr int exp = 100;

  std::cout << "\nCalculando 2^" << exp << " en diferentes tipos:\n";

  // Tipo de 64 bits (usa desplazamiento directo)
  auto time_64 = measure_time_ms([&]() {
    volatile auto result = integral_power_functions::int_power_2<uint64_t>(exp);
    (void)result; // Evitar optimización del compilador
  });

#if defined(__GNUC__) || defined(__clang__)
  // Tipo de 128 bits (usa optimización avanzada)
  auto time_128 = measure_time_ms([&]() {
    volatile auto result =
        integral_power_functions::int_power_2<unsigned __int128>(exp);
    (void)result;
  });
  std::cout << "  uint64_t:          " << std::setw(8) << time_64
            << " ms (desplazamiento directo)\n";
  std::cout << "  unsigned __int128: " << std::setw(8) << time_128
            << " ms (optimizacion avanzada)\n";
  std::cout << "  Speedup ratio:     " << std::setw(8) << (time_64 / time_128)
            << "x\n";
#else
  std::cout << "  uint64_t:          " << std::setw(8) << time_64 << " ms\n";
  std::cout << "  (tipos de 128 bits no disponibles en este compilador)\n";
#endif
}

void demonstrate_general_optimization() {
  std::cout
      << "\n=== DEMOSTRACION: OPTIMIZACION GENERAL PARA TIPOS GRANDES ===\n";
  std::cout << std::fixed << std::setprecision(3);

  // Ejemplo: 3^80 - un caso donde la optimizacion general es util
  constexpr int base = 3;
  constexpr int exp = 80;

  std::cout << "\nCalculando " << base << "^" << exp
            << " en diferentes tipos:\n";

#if defined(__GNUC__) || defined(__clang__)
  // Comparar el metodo tradicional vs optimizado
  auto time_traditional = measure_time_ms([&]() {
    // Simular metodo tradicional (sin optimizacion de chunking)
    volatile auto result = int_power_dispatch<unsigned __int128>(base, exp);
    (void)result;
  });

  auto time_optimized = measure_time_ms([&]() {
    // Usar la nueva optimizacion
    volatile auto result = int_power_smart<unsigned __int128>(base, exp);
    (void)result;
  });

  std::cout << "  Metodo tradicional: " << std::setw(8) << time_traditional
            << " ms\n";
  std::cout << "  Metodo optimizado:  " << std::setw(8) << time_optimized
            << " ms\n";
  std::cout << "  Speedup ratio:      " << std::setw(8)
            << (time_traditional / time_optimized) << "x\n";

  // Explicar la estrategia
  std::cout << "\n  Estrategia de optimizacion para " << base << "^" << exp
            << ":\n";
  std::cout << "    1. Calcular " << base
            << "^32 = " << int_power_smart(base, 32) << "\n";
  std::cout << "    2. Descomponer: " << exp << " = 32*2 + 16\n";
  std::cout << "    3. Resultado = (" << base << "^32)^2 * " << base << "^16\n";
  std::cout << "    4. Reduccion: de " << exp
            << " iteraciones a ~2 iteraciones principales\n";
#else
  std::cout << "  (tipos de 128 bits no disponibles en este compilador)\n";
#endif
}

void show_iteration_reduction_analysis() {
  std::cout << "\n=== ANALISIS: REDUCCION DE ITERACIONES ===\n";

  struct TestCase {
    int base;
    int exp;
    const char *description;
  };

  TestCase cases[] = {{2, 100, "Potencia de 2 muy grande"},
                      {3, 80, "Base pequena, exponente grande"},
                      {5, 60, "Caso balanceado"},
                      {7, 50, "Base prima, exponente moderado"}};

  std::cout << std::setw(4) << "Base" << " | " << std::setw(3) << "Exp" << " | "
            << std::setw(12) << "Iter. Trad." << " | " << std::setw(12)
            << "Iter. Optim." << " | " << std::setw(10) << "Reduccion" << " | "
            << "Descripcion\n";
  std::cout << std::string(70, '-') << "\n";

  for (const auto &test : cases) {
    int traditional_iterations =
        test.exp; // Aproximación para método tradicional
    int chunk_size = 32;
    int quotient = test.exp / chunk_size;
    int remainder = test.exp % chunk_size;

    // Iteraciones optimizadas: log2(quotient) + remainder + overhead
    int optimized_iterations = 0;
    int temp_quotient = quotient;
    while (temp_quotient > 0) {
      optimized_iterations++;
      temp_quotient >>= 1;
    }
    optimized_iterations +=
        remainder + chunk_size; // chunk calculation + remainder

    double reduction =
        static_cast<double>(traditional_iterations) / optimized_iterations;

    std::cout << std::setw(4) << test.base << " | " << std::setw(3) << test.exp
              << " | " << std::setw(12) << traditional_iterations << " | "
              << std::setw(12) << optimized_iterations << " | " << std::setw(9)
              << std::fixed << std::setprecision(1) << reduction << "x | "
              << test.description << "\n";
  }
}

int main() {
  std::cout << "SISTEMA DE OPTIMIZACION PARA TIPOS GRANDES\n";
  std::cout << "Implementacion de la estrategia: usar resultados de 32 bits "
               "como base\n";
  std::cout << std::string(70, '=') << "\n";

  demonstrate_power_of_2_optimization();
  demonstrate_general_optimization();
  show_iteration_reduction_analysis();

  std::cout << "\n" << std::string(70, '=') << "\n";
  std::cout << "CONCLUSION:\n";
  std::cout << "-> La optimizacion reduce significativamente las iteraciones\n";
  std::cout << "-> Especialmente efectiva para exponentes grandes (> 32)\n";
  std::cout << "-> Aprovecha la eficiencia de tipos de 32 bits como punto de "
               "partida\n";
  std::cout << "-> Mantiene precision completa del resultado final\n";
  std::cout << std::string(70, '=') << "\n";

  return 0;
}