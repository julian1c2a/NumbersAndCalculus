/**
 * @file test_simple_unified.cpp
 * @brief Prueba simple del sistema I/O unificado
 * @author AlgoritmiaCombinatoria Project
 * @date 2024
 *
 * Compilar con:
 * g++ -std=c++17 -I../include test_simple_unified.cpp -o test_simple_unified
 */

#include "../include/multiprecision_io_compatible.hpp"
#include <cassert>
#include <iostream>


using namespace multiprecision_io_compat;

int main() {
  try {
    std::cout << "=== PRUEBA SIMPLE I/O UNIFICADO ===" << std::endl;

    // Mostrar información del sistema
    show_system_info();

#if defined(__GNUC__) || defined(__clang__)
    std::cout << "\n--- Pruebas con __int128 ---" << std::endl;

    __int128 val = 123456789012345LL;

    std::cout << "Valor: " << universal_to_string(val) << std::endl;
    std::cout << "Hex: " << universal_to_hex(val, true, true) << std::endl;
    std::cout << "Dígitos: " << universal_count_digits(val) << std::endl;
    std::cout << "Con separadores: " << universal_format_thousands(val, ',')
              << std::endl;

    // Verificar algunos valores
    assert(universal_to_string(val) == "123456789012345");
    assert(universal_count_digits(val) == 15);

    std::cout << "✅ Pruebas básicas __int128 pasadas" << std::endl;
#endif

    // Demostración básica sin main conflictivo
    demo_basic();

    std::cout << "\n=== PRUEBA COMPLETADA EXITOSAMENTE ===" << std::endl;

    return 0;

  } catch (const std::exception &e) {
    std::cerr << "❌ Error: " << e.what() << std::endl;
    return 1;
  }
}