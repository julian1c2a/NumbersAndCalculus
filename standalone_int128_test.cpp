#include <iostream>
#include <limits>

int main() {
  std::cout << "=== Test de soporte de __int128 ===" << std::endl;

// Verificar si __int128 está disponible
#ifdef __SIZEOF_INT128__
  std::cout << "✓ __int128 está disponible (tamaño: " << __SIZEOF_INT128__
            << " bytes)" << std::endl;

  // Probar construcción básica
  __int128 test_val = 42;
  unsigned __int128 test_val_u = 84;

  std::cout << "✓ Variables __int128 creadas exitosamente" << std::endl;

  // Verificar std::numeric_limits
  if (std::numeric_limits<__int128>::is_specialized) {
    std::cout << "✓ std::numeric_limits<__int128> está especializado"
              << std::endl;
    std::cout << "  - digits: " << std::numeric_limits<__int128>::digits
              << std::endl;
    std::cout << "  - digits10: " << std::numeric_limits<__int128>::digits10
              << std::endl;
    std::cout << "  - is_signed: " << std::numeric_limits<__int128>::is_signed
              << std::endl;
    std::cout << "  - is_bounded: " << std::numeric_limits<__int128>::is_bounded
              << std::endl;
  } else {
    std::cout << "✗ std::numeric_limits<__int128> NO está especializado"
              << std::endl;
  }

  if (std::numeric_limits<unsigned __int128>::is_specialized) {
    std::cout << "✓ std::numeric_limits<unsigned __int128> está especializado"
              << std::endl;
    std::cout << "  - digits: "
              << std::numeric_limits<unsigned __int128>::digits << std::endl;
    std::cout << "  - is_signed: "
              << std::numeric_limits<unsigned __int128>::is_signed << std::endl;
  } else {
    std::cout
        << "✗ std::numeric_limits<unsigned __int128> NO está especializado"
        << std::endl;
  }

#else
  std::cout << "✗ __int128 NO está disponible" << std::endl;
#endif

  return 0;
}