#include "combinatorics.hpp"
#include <iomanip>
#include <iostream>

int main() {
  std::cout << "=== Proyecto AlgoritmiaCombinatoria ===" << std::endl;
  std::cout << "Usando C++23 con Boost::multiprecision y Catch2" << std::endl;
  std::cout << std::endl;

  try {
    // Demostración de factorial
    std::cout << "Factoriales:" << std::endl;
    for (int i = 0; i <= 20; ++i) {
      std::cout << i << "! = " << algo_comb::factorial(i) << std::endl;
    }
    std::cout << std::endl;

    // Demostración de combinaciones
    std::cout << "Combinaciones C(10, k):" << std::endl;
    for (int k = 0; k <= 10; ++k) {
      std::cout << "C(10, " << k << ") = " << algo_comb::combinations(10, k)
                << std::endl;
    }
    std::cout << std::endl;

    // Demostración de números de Fibonacci
    std::cout << "Números de Fibonacci:" << std::endl;
    for (int i = 0; i <= 20; ++i) {
      std::cout << "F(" << i << ") = " << algo_comb::fibonacci(i) << std::endl;
    }
    std::cout << std::endl;

    // Demostración de números de Catalan
    std::cout << "Números de Catalan:" << std::endl;
    for (int i = 0; i <= 10; ++i) {
      std::cout << "C(" << i << ") = " << algo_comb::catalan(i) << std::endl;
    }
    std::cout << std::endl;

    // Demostración con BigInt
    std::cout << "Factorial de 100 (BigInt):" << std::endl;
    std::cout << "100! = " << algo_comb::factorial(algo_comb::BigInt(100))
              << std::endl;
    std::cout << std::endl;

    // Demostración de racionales
    std::cout << "Coeficientes binomiales como racionales:" << std::endl;
    for (int k = 0; k <= 5; ++k) {
      auto rational_coeff = algo_comb::binomial_coefficient_rational(10, k);
      std::cout << "C(10, " << k << ") = " << rational_coeff << std::endl;
    }
    std::cout << std::endl;

    // Demostración de permutaciones de un vector pequeño
    std::vector<int> elements = {1, 2, 3};
    std::cout << "Permutaciones de {1, 2, 3}:" << std::endl;
    auto perms = algo_comb::generate_permutations(elements);
    for (const auto &perm : perms) {
      std::cout << "{";
      for (size_t i = 0; i < perm.size(); ++i) {
        std::cout << perm[i];
        if (i < perm.size() - 1)
          std::cout << ", ";
      }
      std::cout << "}" << std::endl;
    }
    std::cout << std::endl;

    // Demostración de combinaciones de un vector
    std::cout << "Combinaciones de 2 elementos de {1, 2, 3, 4}:" << std::endl;
    std::vector<int> elements2 = {1, 2, 3, 4};
    auto combs = algo_comb::generate_combinations(elements2, 2);
    for (const auto &comb : combs) {
      std::cout << "{";
      for (size_t i = 0; i < comb.size(); ++i) {
        std::cout << comb[i];
        if (i < comb.size() - 1)
          std::cout << ", ";
      }
      std::cout << "}" << std::endl;
    }

  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  std::cout << std::endl << "¡Proyecto compilado exitosamente!" << std::endl;
  return 0;
}