#include "include/int128_io.hpp"
#include <iostream>

int main() {
  using namespace int128_io;

  try {
#if defined(__GNUC__) || defined(__clang__)
    std::cout << "=== Pruebas básicas con __int128 ===" << std::endl;

    // Prueba simple
    __int128 simple = 42;
    std::cout << "Número simple: " << simple << std::endl;

    // Conversión a string
    std::string str_simple = to_string(simple);
    std::cout << "Como string: " << str_simple << std::endl;

    // Conversión desde string
    __int128 from_str = from_string_i128("12345");
    std::cout << "Desde string: " << from_str << std::endl;

    std::cout << "Pruebas básicas completadas exitosamente!" << std::endl;

#else
    std::cout << "__int128 no está disponible en este compilador" << std::endl;
#endif

  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}