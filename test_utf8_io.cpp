#include "include/utf8_io.hpp"
#include <iostream>
#include <string>

int main() {
  // Inicializar soporte UTF-8
  utf8_io::initialize_utf8_support();

  std::cout << "=== Prueba de UTF-8 IO ===" << std::endl;

  // Pruebas básicas
  std::string test_ascii = "Hello, World! Basic ASCII test.";
  std::string test_mixed = "Mixed content: ASCII + symbols [OK] -> safe";

  std::cout << "ASCII test: " << test_ascii << std::endl;
  std::cout << "Mixed test: " << test_mixed << std::endl;

  // Validación UTF-8
  std::cout << "ASCII is valid UTF-8: "
            << (utf8_io::is_valid_utf8(test_ascii) ? "true" : "false")
            << std::endl;
  std::cout << "Mixed is valid UTF-8: "
            << (utf8_io::is_valid_utf8(test_mixed) ? "true" : "false")
            << std::endl;

  // Prueba de sanitización
  std::string potentially_problematic = "Test with potential issues";
  std::string sanitized = utf8_io::sanitize_to_ascii(potentially_problematic);
  std::cout << "Sanitized: " << sanitized << std::endl;

  // Prueba de escritura de archivo UTF-8
  {
    utf8_io::utf8_ofstream file("test_utf8_output.txt");
    file << "UTF-8 file test" << std::endl;
    file << "Line 2 with safe characters: [OK] -> success" << std::endl;
    file << "Mathematical symbols converted: PI = " << "PI" << std::endl;
  }

  // Prueba de lectura de archivo UTF-8
  {
    utf8_io::utf8_ifstream file("test_utf8_output.txt");
    std::string line;
    int line_num = 1;
    while (std::getline(file, line)) {
      std::cout << "File line " << line_num++ << ": " << line << std::endl;
    }
  }

  std::cout << "=== UTF-8 IO test completed ===" << std::endl;
  return 0;
}