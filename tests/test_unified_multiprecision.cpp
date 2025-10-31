/**
 * @file test_unified_multiprecision.cpp
 * @brief Pruebas para el sistema I/O unificado de multiprecisión
 * @author AlgoritmiaCombinatoria Project
 * @date 2024
 *
 * Compilar con:
 * g++ -std=c++17 test_unified_multiprecision.cpp -o test_unified_multiprecision
 *
 * Si tienes Boost.Multiprecision instalado:
 * g++ -std=c++17 -DHAS_BOOST_AVAILABLE test_unified_multiprecision.cpp
 * -lboost_system -o test_unified_multiprecision
 */

#define MULTIPRECISION_IO_COMPATIBLE_EXAMPLE
#include "../include/multiprecision_io_compatible.hpp"

#include <cassert>
#include <iostream>
#include <vector>

using namespace multiprecision_io_compat;

//==============================================================================
// FUNCIONES DE PRUEBA
//==============================================================================

void test_universal_to_string() {
  std::cout << "\n=== TEST: universal_to_string ===" << std::endl;

#if defined(__GNUC__) || defined(__clang__)
  // Pruebas con __int128
  __int128 val1 = 123456789012345LL;
  __int128 val2 = -987654321098765LL;
  __int128 val3 = 0;

  std::cout << "val1: " << universal_to_string(val1) << std::endl;
  std::cout << "val2: " << universal_to_string(val2) << std::endl;
  std::cout << "val3: " << universal_to_string(val3) << std::endl;

  // Verificar que funciona como esperamos
  assert(universal_to_string(val1) == "123456789012345");
  assert(universal_to_string(val2) == "-987654321098765");
  assert(universal_to_string(val3) == "0");

  std::cout << "✅ Pruebas __int128 pasadas" << std::endl;
#endif

  // Pruebas con tipos estándar
  int normal_int = 42;
  long long big_long = 9223372036854775807LL;

  std::cout << "int: " << universal_to_string(normal_int) << std::endl;
  std::cout << "long long: " << universal_to_string(big_long) << std::endl;

  assert(universal_to_string(normal_int) == "42");
  assert(universal_to_string(big_long) == "9223372036854775807");

  std::cout << "✅ Pruebas tipos estándar pasadas" << std::endl;
}

void test_universal_to_hex() {
  std::cout << "\n=== TEST: universal_to_hex ===" << std::endl;

#if defined(__GNUC__) || defined(__clang__)
  __int128 val = 255;

  std::cout << "255 en hex (mayúsculas con prefijo): "
            << universal_to_hex(val, true, true) << std::endl;
  std::cout << "255 en hex (minúsculas con prefijo): "
            << universal_to_hex(val, false, true) << std::endl;
  std::cout << "255 en hex (mayúsculas sin prefijo): "
            << universal_to_hex(val, true, false) << std::endl;
  std::cout << "255 en hex (minúsculas sin prefijo): "
            << universal_to_hex(val, false, false) << std::endl;

  __int128 big_val = static_cast<__int128>(0xDEADBEEF) << 32 | 0xCAFEBABE;
  std::cout << "Valor grande en hex: " << universal_to_hex(big_val, true, true)
            << std::endl;

  std::cout << "✅ Pruebas hex __int128 completadas" << std::endl;
#endif

  // Pruebas con tipos estándar
  int val_int = 255;
  std::cout << "int 255 en hex: " << universal_to_hex(val_int, true, true)
            << std::endl;

  std::cout << "✅ Pruebas hex tipos estándar completadas" << std::endl;
}

void test_universal_count_digits() {
  std::cout << "\n=== TEST: universal_count_digits ===" << std::endl;

#if defined(__GNUC__) || defined(__clang__)
  __int128 val1 = 123;          // 3 dígitos
  __int128 val2 = -123;         // 3 dígitos (sin contar signo)
  __int128 val3 = 1000000000LL; // 10 dígitos

  std::cout << "Dígitos de 123: " << universal_count_digits(val1) << std::endl;
  std::cout << "Dígitos de -123: " << universal_count_digits(val2) << std::endl;
  std::cout << "Dígitos de 1000000000: " << universal_count_digits(val3)
            << std::endl;

  assert(universal_count_digits(val1) == 3);
  assert(universal_count_digits(val2) == 3);
  assert(universal_count_digits(val3) == 10);

  std::cout << "✅ Pruebas count_digits __int128 pasadas" << std::endl;
#endif

  // Pruebas con tipos estándar
  int val_small = 42;
  long long val_big = 1234567890123456789LL;

  std::cout << "Dígitos de 42: " << universal_count_digits(val_small)
            << std::endl;
  std::cout << "Dígitos de número grande: " << universal_count_digits(val_big)
            << std::endl;

  assert(universal_count_digits(val_small) == 2);
  assert(universal_count_digits(val_big) == 19);

  std::cout << "✅ Pruebas count_digits tipos estándar pasadas" << std::endl;
}

void test_universal_format_thousands() {
  std::cout << "\n=== TEST: universal_format_thousands ===" << std::endl;

#if defined(__GNUC__) || defined(__clang__)
  __int128 val1 = 1234567890LL;
  __int128 val2 = -1234567890LL;
  __int128 val3 = 123;

  std::cout << "1234567890 con comas: " << universal_format_thousands(val1, ',')
            << std::endl;
  std::cout << "-1234567890 con puntos: "
            << universal_format_thousands(val2, '.') << std::endl;
  std::cout << "123 (sin cambios): " << universal_format_thousands(val3, ',')
            << std::endl;

  assert(universal_format_thousands(val1, ',') == "1,234,567,890");
  assert(universal_format_thousands(val2, '.') == "-1.234.567.890");
  assert(universal_format_thousands(val3, ',') == "123");

  std::cout << "✅ Pruebas format_thousands __int128 pasadas" << std::endl;
#endif

  // Pruebas con tipos estándar
  long long big_num = 9876543210LL;
  std::cout << "Número grande con apostrofes: "
            << universal_format_thousands(big_num, '\'') << std::endl;

  assert(universal_format_thousands(big_num, '\'') == "9'876'543'210");

  std::cout << "✅ Pruebas format_thousands tipos estándar pasadas"
            << std::endl;
}

void test_edge_cases() {
  std::cout << "\n=== TEST: Casos límite ===" << std::endl;

#if defined(__GNUC__) || defined(__clang__)
  // Valores límite para __int128
  __int128 max_val = std::numeric_limits<__int128>::max();
  __int128 min_val = std::numeric_limits<__int128>::min();

  std::cout << "Valor máximo __int128:" << std::endl;
  std::cout << "  Decimal: " << universal_to_string(max_val) << std::endl;
  std::cout << "  Con separadores: " << universal_format_thousands(max_val, ',')
            << std::endl;
  std::cout << "  Dígitos: " << universal_count_digits(max_val) << std::endl;

  std::cout << "Valor mínimo __int128:" << std::endl;
  std::cout << "  Decimal: " << universal_to_string(min_val) << std::endl;
  std::cout << "  Con separadores: " << universal_format_thousands(min_val, ',')
            << std::endl;
  std::cout << "  Dígitos: " << universal_count_digits(min_val) << std::endl;

  // Valor cero
  __int128 zero = 0;
  std::cout << "Cero:" << std::endl;
  std::cout << "  Decimal: " << universal_to_string(zero) << std::endl;
  std::cout << "  Hex: " << universal_to_hex(zero, true, true) << std::endl;
  std::cout << "  Dígitos: " << universal_count_digits(zero) << std::endl;

  std::cout << "✅ Pruebas casos límite completadas" << std::endl;
#endif
}

void demo_real_world_usage() {
  std::cout << "\n=== DEMOSTRACIÓN: Uso del mundo real ===" << std::endl;

#if defined(__GNUC__) || defined(__clang__)
  // Simulación de cálculos combinatorios
  std::vector<__int128> factorials;

  __int128 factorial = 1;
  for (int i = 1; i <= 25; ++i) {
    factorial *= i;
    factorials.push_back(factorial);
  }

  std::cout << "Factoriales de 1 a 25:" << std::endl;
  for (size_t i = 0; i < factorials.size(); ++i) {
    std::cout << "  " << (i + 1)
              << "! = " << universal_format_thousands(factorials[i], ',')
              << " (" << universal_count_digits(factorials[i]) << " dígitos)"
              << std::endl;

    // Solo mostrar algunos para no saturar la salida
    if (i >= 4 && i < factorials.size() - 3) {
      if (i == 5) {
        std::cout << "  ... (saltando algunos) ..." << std::endl;
      }
      continue;
    }
  }

  // Mostrar en diferentes formatos
  __int128 big_factorial = factorials.back();
  std::cout << "\n25! en diferentes formatos:" << std::endl;
  std::cout << "  Decimal normal: " << universal_to_string(big_factorial)
            << std::endl;
  std::cout << "  Con comas: " << universal_format_thousands(big_factorial, ',')
            << std::endl;
  std::cout << "  Con puntos: "
            << universal_format_thousands(big_factorial, '.') << std::endl;
  std::cout << "  Con apostrofes: "
            << universal_format_thousands(big_factorial, '\'') << std::endl;
  std::cout << "  Hexadecimal: " << universal_to_hex(big_factorial, true, true)
            << std::endl;
  std::cout << "  Número de dígitos: " << universal_count_digits(big_factorial)
            << std::endl;

  std::cout << "✅ Demostración del mundo real completada" << std::endl;
#endif
}

//==============================================================================
// FUNCIÓN PRINCIPAL
//==============================================================================

int main() {
  try {
    std::cout << "=== PRUEBAS SISTEMA I/O UNIFICADO MULTIPRECISIÓN ==="
              << std::endl;

    // Mostrar información del sistema
    show_system_info();

    // Ejecutar todas las pruebas
    test_universal_to_string();
    test_universal_to_hex();
    test_universal_count_digits();
    test_universal_format_thousands();
    test_edge_cases();
    demo_real_world_usage();

    std::cout << "\n=== TODAS LAS PRUEBAS COMPLETADAS EXITOSAMENTE ==="
              << std::endl;

#if HAS_BOOST_MULTIPRECISION
    std::cout << "🚀 Sistema funcionando con Boost.Multiprecision" << std::endl;
#else
    std::cout << "⚡ Sistema funcionando solo con __int128" << std::endl;
    std::cout << "💡 Para funcionalidad completa, instalar Boost.Multiprecision"
              << std::endl;
#endif

    return 0;

  } catch (const std::exception &e) {
    std::cerr << "❌ Error durante las pruebas: " << e.what() << std::endl;
    return 1;
  } catch (...) {
    std::cerr << "❌ Error desconocido durante las pruebas" << std::endl;
    return 1;
  }
}