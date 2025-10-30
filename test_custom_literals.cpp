#include "custom_literals.hpp"
#include <iostream>


int main() {
  using namespace custom_literals;

  std::cout << "=== Test de literales personalizados ===" << std::endl;

#if defined(__GNUC__) || defined(__clang__)
  std::cout << "\n1. Literales para __int128:" << std::endl;

  // Literales básicos para __int128
  auto val_i128 = 123456789_i128;
  auto val_ui128 = 987654321_ui128;

  std::cout << "✓ Literales básicos _i128 y _ui128 creados" << std::endl;

  // Literal para potencias de 2
  auto power64 = 64_pow2_i128;   // 2^64
  auto power100 = 100_pow2_i128; // 2^100

  std::cout << "✓ Literales de potencias de 2 creados" << std::endl;

  // Literal hexadecimal
  auto hex_val = "DEADBEEFCAFEBABE"_hex128;

  std::cout << "✓ Literal hexadecimal creado" << std::endl;
#endif

  std::cout << "\n2. Literales para Boost.Multiprecision:" << std::endl;

  // Literales para cpp_int
  auto big_from_str = "123456789012345678901234567890"_cpp_int;
  auto big_from_int = 987654321_cpp_int;

  std::cout << "✓ Literales cpp_int creados" << std::endl;

  // Literales para bases específicas
  auto binary_val = "11110000111100001111000011110000"_bin;
  auto hex_big = "DEADBEEFCAFEBABE123456789ABCDEF0"_hex;

  std::cout << "✓ Literales binarios y hexadecimales creados" << std::endl;

  // Operaciones básicas
  auto result = big_from_int * big_from_int;

  std::cout << "\n3. Resultados:" << std::endl;
  std::cout << "cpp_int grande: " << big_from_str << std::endl;
  std::cout << "cpp_int al cuadrado: " << result << std::endl;

  return 0;
}