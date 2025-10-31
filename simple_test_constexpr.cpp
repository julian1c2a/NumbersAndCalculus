#include "simplified_power_tables_cpp14.hpp"
#include <cassert>
#include <cstdint>
#include <iostream>


int main() {
  std::cout << "=== PRUEBA SIMPLE DE OPTIMIZACIONES CONSTEXPR ===\n\n";

  using namespace number_calc::integral_power_functions::lookup_tables;

  // Prueba básica int8_t
  std::cout << "Pruebas int8_t:\n";
  for (int i = 0; i <= 6; ++i) {
    int8_t result = get_power_of_2_int8(i);
    std::cout << "  2^" << i << " = " << static_cast<int>(result) << "\n";
  }

  // Prueba básica uint8_t
  std::cout << "\nPruebas uint8_t:\n";
  for (int i = 0; i <= 7; ++i) {
    uint8_t result = get_power_of_2_uint8(i);
    std::cout << "  2^" << i << " = " << static_cast<int>(result) << "\n";
  }

  // Prueba template
  std::cout << "\nPrueba template:\n";
  int8_t t1 = get_power_of_2_from_table<int8_t>(4);
  uint16_t t2 = get_power_of_2_from_table<uint16_t>(10);
  std::cout << "  get_power_of_2_from_table<int8_t>(4) = "
            << static_cast<int>(t1) << "\n";
  std::cout << "  get_power_of_2_from_table<uint16_t>(10) = " << t2 << "\n";

  // Prueba constexpr
  std::cout << "\nPrueba constexpr:\n";
  constexpr int8_t compile_time = get_power_of_2_int8(5);
  std::cout << "  constexpr result: 2^5 = " << static_cast<int>(compile_time)
            << "\n";

  std::cout << "\n=== TODAS LAS PRUEBAS COMPLETADAS ===\n";
  return 0;
}