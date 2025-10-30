#include <iostream>
#include <limits>

// Test simple para verificar soporte de __int128
int main() {
  std::cout << "Probando __int128..." << std::endl;

#ifdef __SIZEOF_INT128__
  std::cout << "__SIZEOF_INT128__ definido: " << __SIZEOF_INT128__ << std::endl;
#else
  std::cout << "__SIZEOF_INT128__ NO definido" << std::endl;
#endif

  // Verificar si numeric_limits está especializado
  std::cout << "numeric_limits<__int128>::is_specialized: "
            << std::numeric_limits<__int128>::is_specialized << std::endl;

  return 0;
}