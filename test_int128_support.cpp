#include <iostream>
#include <limits>

int main() {
// Probar si __int128 está disponible
#ifdef __SIZEOF_INT128__
  std::cout << "__int128 está disponible (tamaño: " << __SIZEOF_INT128__
            << " bytes)" << std::endl;

  // Probar si std::numeric_limits<__int128> está disponible
  if (std::numeric_limits<__int128>::is_specialized) {
    std::cout << "std::numeric_limits<__int128> está especializado"
              << std::endl;
    std::cout << "digits: " << std::numeric_limits<__int128>::digits
              << std::endl;
    std::cout << "digits10: " << std::numeric_limits<__int128>::digits10
              << std::endl;
    std::cout << "is_signed: " << std::numeric_limits<__int128>::is_signed
              << std::endl;
    std::cout << "is_bounded: " << std::numeric_limits<__int128>::is_bounded
              << std::endl;
  } else {
    std::cout << "std::numeric_limits<__int128> NO está especializado"
              << std::endl;
  }

  // Probar operaciones básicas
  __int128 test_val = 1;
  test_val <<= 100; // 2^100
  std::cout << "Operaciones básicas con __int128 funcionan" << std::endl;

#else
  std::cout << "__int128 NO está disponible" << std::endl;
#endif

  return 0;
}