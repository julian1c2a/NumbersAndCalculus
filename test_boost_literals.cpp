#include <boost/multiprecision/cpp_int.hpp>
#include <iostream>


#ifdef BOOST_HAS_GMP
#include <boost/multiprecision/gmp.hpp>
#endif

#ifdef BOOST_HAS_TOMMATH
#include <boost/multiprecision/tommath.hpp>
#endif

// Test para literales definidos por el usuario
int main() {
  std::cout << "=== Test de literales de Boost.Multiprecision ===" << std::endl;

  using namespace boost::multiprecision;

  // Test básico sin literales
  std::cout << "\n1. Construcción básica:" << std::endl;
  cpp_int basic_int = 42;
  std::cout << "cpp_int basic: " << basic_int << std::endl;

  // Test de literales estándar de C++
  std::cout << "\n2. Literales estándar:" << std::endl;
  cpp_int from_decimal = 123456789;
  cpp_int from_hex = 0xDEADBEEF;
  cpp_int from_binary = 0b11110000;

  std::cout << "Decimal: " << from_decimal << std::endl;
  std::cout << "Hex: " << from_hex << std::endl;
  std::cout << "Binary: " << from_binary << std::endl;

  // Test para ver si Boost proporciona literales UDL
  std::cout << "\n3. Intentando usar literales UDL de Boost:" << std::endl;

  // Vamos a intentar diferentes sufijos que Boost podría proporcionar
  // Nota: Estos pueden fallar si no están implementados

  try {
    // Intentar acceso a namespace de literales si existe
    // using namespace boost::multiprecision::literals;

    std::cout << "Boost no parece proporcionar literales UDL estándar"
              << std::endl;
    std::cout << "Se debe usar construcción directa:" << std::endl;

    // Construcción directa de números grandes
    cpp_int big_num("123456789012345678901234567890");
    std::cout << "Número grande: " << big_num << std::endl;

    // Operaciones con números grandes
    cpp_int huge_result = big_num * big_num;
    std::cout << "Cuadrado: " << huge_result << std::endl;

  } catch (...) {
    std::cout << "Error al procesar literales grandes" << std::endl;
  }

  std::cout
      << "\n4. Verificando si existen literales en namespaces específicos:"
      << std::endl;

// Verificar si hay literales en namespaces específicos
#ifdef BOOST_MULTIPRECISION_LITERALS_HPP
  std::cout << "✓ Header de literales de Boost incluido" << std::endl;
#else
  std::cout << "✗ No se encontró header específico de literales" << std::endl;
#endif

  return 0;
}