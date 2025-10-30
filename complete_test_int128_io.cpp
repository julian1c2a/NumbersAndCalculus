#include "include/int128_io.hpp"
#include <iostream>
#include <sstream>

int main() {
  using namespace int128_io;

  try {
#if defined(__GNUC__) || defined(__clang__)
    std::cout << "=== DEMOSTRACIÓN COMPLETA DE I/O PARA __int128 ==="
              << std::endl;

    // 1. Pruebas básicas de salida
    std::cout << "\n--- Pruebas de salida básica ---" << std::endl;
    __int128 small = 42;
    __int128 big = static_cast<__int128>(1000000000000000000LL) * 1000000000LL;
    __int128 negative = -big;

    std::cout << "Número pequeño: " << small << std::endl;
    std::cout << "Número grande: " << big << std::endl;
    std::cout << "Número negativo: " << negative << std::endl;

    // 2. Pruebas con unsigned
    std::cout << "\n--- Pruebas con unsigned __int128 ---" << std::endl;
    unsigned __int128 ubig = static_cast<unsigned __int128>(big) * 2;
    std::cout << "Unsigned grande: " << ubig << std::endl;

    // 3. Conversiones string
    std::cout << "\n--- Conversiones a string ---" << std::endl;
    std::string str1 = to_string(big);
    std::string str2 = to_string(ubig);
    std::cout << "big como string: " << str1 << std::endl;
    std::cout << "ubig como string: " << str2 << std::endl;

    // 4. Conversiones desde string
    std::cout << "\n--- Conversiones desde string ---" << std::endl;
    __int128 parsed1 = from_string_i128("123456789012345678901234567890");
    unsigned __int128 parsed2 =
        from_string_ui128("987654321098765432109876543210");
    std::cout << "Parsed signed: " << parsed1 << std::endl;
    std::cout << "Parsed unsigned: " << parsed2 << std::endl;

    // 5. Pruebas con stringstream
    std::cout << "\n--- Pruebas con stringstream ---" << std::endl;
    std::stringstream ss;
    ss << big << " " << ubig;

    __int128 read1;
    unsigned __int128 read2;
    ss >> read1 >> read2;

    std::cout << "Leído signed: " << read1 << std::endl;
    std::cout << "Leído unsigned: " << read2 << std::endl;

    // 6. Pruebas con valores extremos
    std::cout << "\n--- Valores extremos ---" << std::endl;
    __int128 min_val = std::numeric_limits<__int128>::min();
    __int128 max_val = std::numeric_limits<__int128>::max();
    unsigned __int128 umax_val = std::numeric_limits<unsigned __int128>::max();

    std::cout << "Mínimo __int128: " << min_val << std::endl;
    std::cout << "Máximo __int128: " << max_val << std::endl;
    std::cout << "Máximo unsigned: " << umax_val << std::endl;

    // 7. Pruebas de manejo de errores
    std::cout << "\n--- Pruebas de manejo de errores ---" << std::endl;
    try {
      from_string_i128(""); // String vacía
    } catch (const std::invalid_argument &e) {
      std::cout << "Capturado error esperado: " << e.what() << std::endl;
    }

    try {
      from_string_i128("abc123"); // Caracteres inválidos
    } catch (const std::invalid_argument &e) {
      std::cout << "Capturado error esperado: " << e.what() << std::endl;
    }

    try {
      from_string_ui128("-123"); // Negativo en unsigned
    } catch (const std::invalid_argument &e) {
      std::cout << "Capturado error esperado: " << e.what() << std::endl;
    }

    std::cout << "\n=== TODAS LAS PRUEBAS COMPLETADAS EXITOSAMENTE ==="
              << std::endl;

#else
    std::cout << "__int128 no está disponible en este compilador" << std::endl;
#endif

  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}