#include <iostream>

int main() {
  std::cout << "Hola mundo" << std::endl;

#ifdef __SIZEOF_INT128__
  std::cout << "__int128 disponible, tamaño: " << __SIZEOF_INT128__
            << std::endl;
  __int128 test = 42;
  std::cout << "Variable __int128 creada exitosamente" << std::endl;
#else
  std::cout << "__int128 NO disponible" << std::endl;
#endif

  return 0;
}