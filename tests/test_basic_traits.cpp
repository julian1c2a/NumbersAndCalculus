/**
 * @file test_basic_traits.cpp
 * @brief Test simple para verificar especializaciones por traits
 * @author AlgoritmiaCombinatoria Project
 * @date 2024
 *
 * Compilar con:
 * g++ -std=c++20 test_basic_traits.cpp -o test_traits.exe
 */

#include <cassert>
#include <chrono>
#include <iostream>
#include <type_traits>


//==============================================================================
// IMPLEMENTACIÓN SIMPLE DE ESPECIALIZACIONES POR TRAITS
//==============================================================================

/**
 * @brief Función template genérica para potencia entera
 * @tparam T Tipo entero
 * @param base Base de la potencia
 * @param exp Exponente
 * @return base^exp
 */
template <typename T> constexpr T int_power_basic(T base, T exp) noexcept {
  if (exp == 0)
    return T{1};
  if (base == 0)
    return T{0};
  if (base == 1)
    return T{1};

  T result = T{1};
  T current_base = base;
  T current_exp = exp;

  while (current_exp > 0) {
    if (current_exp & 1) {
      result *= current_base;
    }
    current_base *= current_base;
    current_exp >>= 1;
  }

  return result;
}

/**
 * @brief Implementación para tipos signed
 * @tparam T Tipo entero con signo
 * @param base Base
 * @param exp Exponente
 * @return base^exp con optimizaciones para signed
 */
template <typename T>
constexpr T int_power_signed_impl(T base, T exp) noexcept
  requires std::is_signed_v<T>
{
  // Optimización especial para -1
  if (base == -1) {
    return (exp & 1) ? T{-1} : T{1};
  }

  // Para otros casos, usar implementación básica
  return int_power_basic(base, exp);
}

/**
 * @brief Implementación para tipos unsigned
 * @tparam T Tipo entero sin signo
 * @param base Base
 * @param exp Exponente
 * @return base^exp sin verificaciones de signo
 */
template <typename T>
constexpr T int_power_unsigned_impl(T base, T exp) noexcept
  requires std::is_unsigned_v<T>
{
  // Sin verificaciones de signo
  return int_power_basic(base, exp);
}

/**
 * @brief Función dispatch que selecciona la implementación correcta
 * @tparam T Tipo entero
 * @param base Base
 * @param exp Exponente
 * @return base^exp usando la especialización adecuada
 */
template <typename T>
constexpr T int_power_dispatch(T base, T exp) noexcept
  requires std::is_integral_v<T>
{
  if constexpr (std::is_signed_v<T>) {
    return int_power_signed_impl(base, exp);
  } else {
    return int_power_unsigned_impl(base, exp);
  }
}

/**
 * @brief Función especializada para potencias de 2
 * @tparam T Tipo entero
 * @param exp Exponente
 * @return 2^exp usando bit shifting
 */
template <typename T>
constexpr T int_power_2(T exp) noexcept
  requires std::is_integral_v<T>
{
  if (exp == 0)
    return T{1};

  // Usar bit shifting para potencias de 2
  if constexpr (sizeof(T) == 1) {
    // Para tipos de 8 bits
    return static_cast<T>(T{1} << exp);
  } else if constexpr (sizeof(T) == 2) {
    // Para tipos de 16 bits
    return static_cast<T>(T{1} << exp);
  } else if constexpr (sizeof(T) == 4) {
    // Para tipos de 32 bits
    return static_cast<T>(T{1} << exp);
  } else if constexpr (sizeof(T) == 8) {
    // Para tipos de 64 bits
    return static_cast<T>(T{1} << exp);
  } else {
    // Para tipos más grandes, usar multiplicación
    T result = T{1};
    for (T i = 0; i < exp; ++i) {
      result *= T{2};
    }
    return result;
  }
}

/**
 * @brief Función principal que detecta automáticamente potencias de 2
 * @tparam T Tipo entero
 * @param base Base
 * @param exp Exponente
 * @return base^exp con optimizaciones automáticas
 */
template <typename T>
constexpr T int_power_new(T base, T exp) noexcept
  requires std::is_integral_v<T>
{
  // Detectar potencias de 2
  if (base == 2) {
    return int_power_2<T>(exp);
  }

  // Usar dispatch para otros casos
  return int_power_dispatch(base, exp);
}

//==============================================================================
// FUNCIONES DE TEST
//==============================================================================

void test_basic_functionality() {
  std::cout << "\n=== TEST FUNCIONALIDAD BÁSICA ===" << std::endl;

  // Test con tipos signed
  assert(int_power_dispatch(2, 0) == 1);
  assert(int_power_dispatch(2, 1) == 2);
  assert(int_power_dispatch(2, 10) == 1024);
  assert(int_power_dispatch(3, 5) == 243);
  assert(int_power_dispatch(-1, 0) == 1);
  assert(int_power_dispatch(-1, 2) == 1);
  assert(int_power_dispatch(-1, 3) == -1);
  assert(int_power_dispatch(-2, 4) == 16);

  std::cout << "✅ Tests signed completados" << std::endl;

  // Test con tipos unsigned
  assert(int_power_dispatch(2U, 0U) == 1U);
  assert(int_power_dispatch(2U, 10U) == 1024U);
  assert(int_power_dispatch(3U, 5U) == 243U);

  std::cout << "✅ Tests unsigned completados" << std::endl;
}

void test_power_of_2_specialization() {
  std::cout << "\n=== TEST ESPECIALIZACIÓN POTENCIAS DE 2 ===" << std::endl;

  // Test int_power_2
  assert(int_power_2<int>(0) == 1);
  assert(int_power_2<int>(1) == 2);
  assert(int_power_2<int>(10) == 1024);
  assert(int_power_2<long long>(20) == 1048576LL);

  std::cout << "✅ int_power_2<int>(10) = " << int_power_2<int>(10)
            << std::endl;
  std::cout << "✅ int_power_2<long long>(20) = " << int_power_2<long long>(20)
            << std::endl;

  // Test detección automática
  assert(int_power_new(2, 10) == 1024);
  assert(int_power_new(2, 15) == 32768);
  assert(int_power_new(3, 5) == 243); // No es potencia de 2

  std::cout << "✅ Detección automática funcionando" << std::endl;
}

void test_signed_vs_unsigned() {
  std::cout << "\n=== TEST TRAITS SIGNED VS UNSIGNED ===" << std::endl;

  // Casos especiales para signed
  std::cout << "Casos especiales signed:" << std::endl;
  std::cout << "  (-1)^0 = " << int_power_dispatch(-1, 0) << std::endl;
  std::cout << "  (-1)^2 = " << int_power_dispatch(-1, 2) << std::endl;
  std::cout << "  (-1)^3 = " << int_power_dispatch(-1, 3) << std::endl;
  std::cout << "  (-2)^4 = " << int_power_dispatch(-2, 4) << std::endl;

  // Casos para unsigned
  std::cout << "Casos unsigned:" << std::endl;
  std::cout << "  2U^10 = " << int_power_dispatch(2U, 10U) << std::endl;
  std::cout << "  3U^5 = " << int_power_dispatch(3U, 5U) << std::endl;

  std::cout << "✅ Especializaciones por signo funcionando" << std::endl;
}

void benchmark_implementations() {
  std::cout << "\n=== BENCHMARK IMPLEMENTACIONES ===" << std::endl;

  const int iterations = 1000000;
  const int base = 3;
  const int exp = 12;

  // Benchmark básico
  auto start = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < iterations; ++i) {
    volatile int result = int_power_basic(base, exp);
    (void)result;
  }
  auto end = std::chrono::high_resolution_clock::now();
  auto duration_basic =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start);

  // Benchmark con dispatch
  start = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < iterations; ++i) {
    volatile int result = int_power_dispatch(base, exp);
    (void)result;
  }
  end = std::chrono::high_resolution_clock::now();
  auto duration_dispatch =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start);

  // Benchmark int_power_2
  start = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < iterations; ++i) {
    volatile int result = int_power_2<int>(exp);
    (void)result;
  }
  end = std::chrono::high_resolution_clock::now();
  auto duration_power2 =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start);

  std::cout << "Implementación básica: " << duration_basic.count() << " μs"
            << std::endl;
  std::cout << "Dispatch por traits: " << duration_dispatch.count() << " μs"
            << std::endl;
  std::cout << "int_power_2 especializada: " << duration_power2.count() << " μs"
            << std::endl;
}

void demo_constexpr_evaluation() {
  std::cout << "\n=== DEMO EVALUACIÓN CONSTEXPR ===" << std::endl;

  // Estas se evalúan en tiempo de compilación
  constexpr int power2_10 = int_power_2<int>(10);
  constexpr int power_dispatch = int_power_dispatch(3, 5);
  constexpr int power_new = int_power_new(2, 8);

  std::cout << "constexpr int_power_2<int>(10) = " << power2_10 << std::endl;
  std::cout << "constexpr int_power_dispatch(3, 5) = " << power_dispatch
            << std::endl;
  std::cout << "constexpr int_power_new(2, 8) = " << power_new << std::endl;

  // Verificar que se pueden usar en contextos constexpr
  static_assert(int_power_2<int>(10) == 1024);
  static_assert(int_power_dispatch(3, 5) == 243);
  static_assert(int_power_new(2, 8) == 256);

  std::cout << "✅ Todas las funciones son correctamente constexpr"
            << std::endl;
}

//==============================================================================
// FUNCIÓN PRINCIPAL
//==============================================================================

int main() {
  try {
    std::cout
        << "=== DEMOSTRACIÓN ESPECIALIZACIONES POR TRAITS SIMPLIFICADA ==="
        << std::endl;

    test_basic_functionality();
    test_power_of_2_specialization();
    test_signed_vs_unsigned();
    benchmark_implementations();
    demo_constexpr_evaluation();

    std::cout << "\n=== VENTAJAS DE LAS ESPECIALIZACIONES POR TRAITS ==="
              << std::endl;
    std::cout << "✅ Una sola implementación para todos los tipos signed"
              << std::endl;
    std::cout << "✅ Una sola implementación para todos los tipos unsigned"
              << std::endl;
    std::cout << "✅ Optimizaciones específicas por características del tipo"
              << std::endl;
    std::cout << "✅ Función int_power_2 altamente optimizada para base=2"
              << std::endl;
    std::cout << "✅ Código más mantenible y extensible" << std::endl;
    std::cout << "✅ Detección automática de optimizaciones (potencias de 2)"
              << std::endl;
    std::cout << "✅ Compatibilidad total con evaluación constexpr"
              << std::endl;
    std::cout << "✅ Usa std::is_signed_v y std::is_unsigned_v como traits"
              << std::endl;

    std::cout << "\n🎉 DEMOSTRACIÓN COMPLETADA EXITOSAMENTE" << std::endl;

    return 0;

  } catch (const std::exception &e) {
    std::cerr << "❌ Error durante la demostración: " << e.what() << std::endl;
    return 1;
  } catch (...) {
    std::cerr << "❌ Error desconocido durante la demostración" << std::endl;
    return 1;
  }
}