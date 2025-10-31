#include <iostream>

// Test para verificar qué estándar C++ estamos usando
int main() {
  std::cout << "C++ standard: " << __cplusplus << std::endl;

#if __cplusplus >= 201703L
  std::cout << "C++17 or later is supported" << std::endl;
#elif __cplusplus >= 201402L
  std::cout << "C++14 is supported" << std::endl;
#elif __cplusplus >= 201103L
  std::cout << "C++11 is supported" << std::endl;
#else
  std::cout << "Pre-C++11" << std::endl;
#endif

  return 0;
}