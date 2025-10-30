#include <iostream>
#include <limits>

// Test para literales de tipos no estándar como __int128
int main() {
  std::cout << "=== Test de literales para tipos no estándar ===" << std::endl;

#ifdef __SIZEOF_INT128__
  std::cout << "\n1. __int128 está disponible:" << std::endl;

  // Construcción básica
  __int128 basic = 42;
  unsigned __int128 basic_u = 84;

  std::cout << "Construcción básica exitosa" << std::endl;

  // Literales estándar
  __int128 from_decimal = 123456789012345;
  __int128 from_hex = 0xDEADBEEFCAFEBABE;
  __int128 from_binary = 0b1111000011110000111100001111;

  std::cout << "Literales estándar funcionan" << std::endl;

  // Prueba de literales con sufijos
  std::cout << "\n2. Intentando literales con sufijos:" << std::endl;

  // __int128 no tiene sufijos específicos en C++
  // Los literales grandes se pueden crear, pero no hay sufijos como _i128

  // Construcción de números que requieren más de 64 bits
  __int128 large_positive = 1;
  large_positive <<= 100; // 2^100

  unsigned __int128 large_unsigned = 1;
  large_unsigned <<= 100;

  std::cout << "Números grandes creados exitosamente" << std::endl;

  // Uso de std::numeric_limits para obtener valores extremos
  __int128 max_val = std::numeric_limits<__int128>::max();
  __int128 min_val = std::numeric_limits<__int128>::min();

  std::cout << "min y max obtenidos de numeric_limits" << std::endl;

  std::cout << "\n3. Resultado:" << std::endl;
  std::cout << "✓ __int128 soporta literales estándar (decimal, hex, binary)"
            << std::endl;
  std::cout << "✗ __int128 NO tiene sufijos específicos como _i128"
            << std::endl;
  std::cout << "✓ Se pueden crear valores grandes usando desplazamiento de bits"
            << std::endl;
  std::cout << "✓ std::numeric_limits proporciona valores extremos"
            << std::endl;

#else
  std::cout << "✗ __int128 no está disponible" << std::endl;
#endif

  return 0;
}