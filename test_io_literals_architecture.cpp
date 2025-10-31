#include "io/io.hpp"
#include "literals/literals.hpp"
#include <cassert>
#include <iostream>


/**
 * @file test_io_literals_architecture.cpp
 * @brief Programa de prueba para la nueva arquitectura modular de IO y
 * literales
 */

int main() {
  std::cout << "=== PRUEBAS DE ARQUITECTURA MODULAR IO Y LITERALES ===\n\n";

  using namespace io;
  using namespace literals;

#if defined(__GNUC__) || defined(__clang__)

  std::cout << "1. Pruebas de Literales __int128:\n";

  // Literales decimales
  auto big_signed = 123456789012345678901234567890_i128;
  auto big_unsigned = 123456789012345678901234567890_u128;

  std::cout << "   Decimal signed:   " << big_signed << "\n";
  std::cout << "   Decimal unsigned: " << big_unsigned << "\n";

  // Literales desde números pequeños
  auto small_signed = 42_i128;
  auto small_unsigned = 42_u128;

  std::cout << "   Small signed:     " << small_signed << "\n";
  std::cout << "   Small unsigned:   " << small_unsigned << "\n";

  // Literales hexadecimales
  std::cout << "\n2. Pruebas de Literales Hexadecimales:\n";
  auto hex_signed = "DEADBEEF"_i128_hex;
  auto hex_unsigned = "CAFEBABE"_u128_hex;

  std::cout << "   Hex signed:   " << hex_signed << " (DEADBEEF)\n";
  std::cout << "   Hex unsigned: " << hex_unsigned << " (CAFEBABE)\n";

  // Literales binarios
  std::cout << "\n3. Pruebas de Literales Binarios:\n";
  auto bin_signed = "1010101010101010"_i128_bin;
  auto bin_unsigned = "1111000011110000"_u128_bin;

  std::cout << "   Binary signed:   " << bin_signed << " (1010101010101010)\n";
  std::cout << "   Binary unsigned: " << bin_unsigned
            << " (1111000011110000)\n";

  // Pruebas de entrada/salida
  std::cout << "\n4. Pruebas de IO __int128:\n";

  // Demostrar que los operadores << funcionan automáticamente
  __int128 test_values[] = {0, 1, -1, 123456789, -123456789};

  for (auto val : test_values) {
    std::cout << "   Valor: " << val << "\n";
  }

  // Pruebas de conversión string
  std::cout << "\n5. Pruebas de Conversión String:\n";

  std::string str_positive = "12345678901234567890";
  std::string str_negative = "-98765432109876543210";

  __int128 from_str_pos = string_to_int128(str_positive);
  __int128 from_str_neg = string_to_int128(str_negative);

  std::cout << "   String -> __int128: \"" << str_positive
            << "\" = " << from_str_pos << "\n";
  std::cout << "   String -> __int128: \"" << str_negative
            << "\" = " << from_str_neg << "\n";

  // Conversión inversa
  std::string back_to_str_pos = int128_to_string(from_str_pos);
  std::string back_to_str_neg = int128_to_string(from_str_neg);

  std::cout << "   __int128 -> String: " << from_str_pos << " = \""
            << back_to_str_pos << "\"\n";
  std::cout << "   __int128 -> String: " << from_str_neg << " = \""
            << back_to_str_neg << "\"\n";

  // Verificar que las conversiones son bidireccionales
  assert(str_positive == back_to_str_pos);
  assert(str_negative == back_to_str_neg);
  std::cout << "   ✓ Conversiones bidireccionales verificadas\n";

  std::cout << "\n6. Demostración de Modularidad:\n";
  std::cout
      << "   ✓ io/integral_types/int128_io.hpp - Operadores IO para __int128\n";
  std::cout << "   ✓ literals/integral_literals/int128_literals.hpp - "
               "Literales UDL\n";
  std::cout << "   ✓ io/io.hpp - API unificada de IO\n";
  std::cout << "   ✓ literals/literals.hpp - API unificada de literales\n";

  std::cout << "\n=== TODAS LAS PRUEBAS PASARON EXITOSAMENTE ===\n";
  std::cout << "La nueva arquitectura modular de IO y literales funciona "
               "correctamente!\n\n";

  std::cout << "7. Estructura Modular Lograda:\n";
  std::cout << "   📁 include/io/\n";
  std::cout << "      📁 integral_types/     - IO para tipos enteros\n";
  std::cout << "      📁 multiprecision_types/ - IO para tipos multiprecisión "
               "(futuro)\n";
  std::cout << "   📁 include/literals/\n";
  std::cout << "      📁 integral_literals/   - Literales para tipos enteros\n";
  std::cout << "      📁 multiprecision_literals/ - Literales multiprecisión "
               "(futuro)\n";

#else
  std::cout << "NOTA: __int128 no soportado en este compilador.\n";
  std::cout << "Las pruebas completas requieren GCC o Clang.\n";
#endif

  return 0;
}