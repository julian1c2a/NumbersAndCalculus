#include "combinatorics.hpp"
#include "number_type_traits.hpp"
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/gmp.hpp>
#include <boost/multiprecision/tommath.hpp>
#include <chrono>
#include <iostream>


using namespace algo_comb;
using namespace integral_type_traits;

// Función template que funciona con cualquier tipo de entero arbitrario
template <integral T> void test_big_integer_type(const std::string &type_name) {
  std::cout << "\n=== Probando " << type_name << " ===" << std::endl;

  auto start = std::chrono::high_resolution_clock::now();

  // Prueba factorial grande
  T big_num = 50;
  auto fact_result = factorial(big_num);

  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start);

  std::cout << "50! = " << fact_result << std::endl;
  std::cout << "Tiempo: " << duration.count() << " microsegundos" << std::endl;

  // Prueba combinaciones
  start = std::chrono::high_resolution_clock::now();
  auto comb_result = combinations(T(100), T(50));
  end = std::chrono::high_resolution_clock::now();
  duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

  std::cout << "C(100,50) = " << comb_result << std::endl;
  std::cout << "Tiempo: " << duration.count() << " microsegundos" << std::endl;

  // Verificar propiedades del tipo
  std::cout << "Es integral: " << integral<T> << std::endl;
  std::cout << "Es signed_integral: " << signed_integral<T> << std::endl;
  std::cout << "Es infty_integral: " << infty_integral<T> << std::endl;
  std::cout << "Es finite_integral: " << finite_integral<T> << std::endl;
}

int main() {
  std::cout << "=== Comparación de tipos de enteros de precisión arbitraria ==="
            << std::endl;
  std::cout << "Boost.Multiprecision soporta tres implementaciones principales:"
            << std::endl;
  std::cout << "1. cpp_int: Implementación propia (sin dependencias externas)"
            << std::endl;
  std::cout << "2. tom_int: Basado en libtommath (requiere libtommath)"
            << std::endl;
  std::cout << "3. mpz_int: Basado en GMP (requiere libgmp)" << std::endl;

  try {
    // Probar cpp_int (siempre disponible)
    test_big_integer_type<boost::multiprecision::cpp_int>("cpp_int");

    // Probar mpz_int (requiere GMP)
    try {
      test_big_integer_type<boost::multiprecision::mpz_int>("mpz_int (GMP)");
    } catch (const std::exception &e) {
      std::cout << "\nmpz_int no disponible (GMP no encontrado): " << e.what()
                << std::endl;
    }

    // Probar tom_int (requiere libtommath)
    try {
      test_big_integer_type<boost::multiprecision::tom_int>(
          "tom_int (libtommath)");
    } catch (const std::exception &e) {
      std::cout << "\ntom_int no disponible (libtommath no encontrado): "
                << e.what() << std::endl;
    }

    // Comparación con enteros estándar para contexto
    std::cout << "\n=== Comparación con tipos estándar ===" << std::endl;
    std::cout << "int es integral: " << integral<int> << std::endl;
    std::cout << "int es finite_integral: "
              << finite_integral<int> << std::endl;
    std::cout << "int es infty_integral: " << infty_integral<int> << std::endl;

    std::cout << "\nlong long es integral: "
              << integral<long long> << std::endl;
    std::cout << "long long es finite_integral: "
              << finite_integral<long long> << std::endl;

    // Demostrar la limitación de tipos estándar
    std::cout << "\n=== Límites de tipos estándar ===" << std::endl;
    std::cout << "long long máximo: " << std::numeric_limits<long long>::max()
              << std::endl;
    std::cout << "20! con long long: ";
    try {
      // Esto podría desbordarse
      long long fact20 = 1;
      for (int i = 2; i <= 20; ++i) {
        fact20 *= i;
      }
      std::cout << fact20 << std::endl;
    } catch (...) {
      std::cout << "DESBORDAMIENTO" << std::endl;
    }

    std::cout << "20! con cpp_int: "
              << factorial(boost::multiprecision::cpp_int(20)) << std::endl;

  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  std::cout << "\n¡Demostración completada!" << std::endl;
  return 0;
}