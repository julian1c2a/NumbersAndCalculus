#pragma once

/**
 * @file io.hpp
 * @brief Header principal unificado para todas las utilidades de entrada/salida
 * @author AlgoritmiaCombinatoria Project
 * @date 2024
 * @version 1.0
 *
 * Este header incluye todas las utilidades de IO del proyecto organizadas
 * en módulos especializados:
 *
 * - integral_types/: IO para tipos enteros (__int128, signed, unsigned)
 * - multiprecision_types/: IO para tipos de precisión arbitraria (Boost, GMP)
 *
 * Uso recomendado:
 * ```cpp
 * #include "io/io.hpp"
 * using namespace io;
 *
 * __int128 big = /* ... */
;
*std::cout << big << std::endl;             // Automático
*std::cout << format_hex(big) << std::endl; // Formateo hex
* ``` * /

//==============================================================================
// MÓDULOS DE TIPOS INTEGRALES
//==============================================================================

#include "integral_types/int128_io.hpp"
    // #include "integral_types/standard_io.hpp"      // TODO: Crear
    // #include "integral_types/formatted_output.hpp" // TODO: Crear

    //==============================================================================
    // MÓDULOS DE TIPOS MULTIPRECISIÓN
    //==============================================================================

    // #include "multiprecision_types/boost_multiprecision_io.hpp" // TODO:
    // Migrar #include "multiprecision_types/gmp_io.hpp"                  //
    // TODO: Crear #include "multiprecision_types/conversion_utilities.hpp" //
    // TODO: Crear

    //==============================================================================
    // API PÚBLICA UNIFICADA
    //==============================================================================

    namespace io {

  // Importar todos los namespaces de módulos especializados
  using namespace integral_types;
  // using namespace multiprecision_types;  // TODO: Cuando se creen

  //==============================================================================
  // FUNCIONES DE FORMATEO CONVENIENTES (FUTURAS)
  //==============================================================================

#if defined(__GNUC__) || defined(__clang__)

  /**
   * @brief Formatea un número entero en hexadecimal
   * @tparam T Tipo entero
   * @param value Valor a formatear
   * @param uppercase Si usar mayúsculas (A-F) o minúsculas (a-f)
   * @return String formateado
   *
   * TODO: Implementar en integral_types/formatted_output.hpp
   */
  template <typename T> std::string format_hex(T value, bool uppercase = true) {
    // Placeholder - implementación real en módulo específico
    return "0x" + std::to_string(static_cast<long long>(value));
  }

  /**
   * @brief Formatea un número entero en binario
   * @tparam T Tipo entero
   * @param value Valor a formatear
   * @param width Ancho mínimo (rellena con ceros a la izquierda)
   * @return String formateado
   *
   * TODO: Implementar en integral_types/formatted_output.hpp
   */
  template <typename T> std::string format_binary(T value, int width = 0) {
    // Placeholder - implementación real en módulo específico
    return "0b" + std::to_string(static_cast<long long>(value));
  }

#endif

} // namespace io

//==============================================================================
// EJEMPLOS DE USO
//==============================================================================

/**
 * @example Uso básico de IO unificado
 *
 * ```cpp
 * #include "io/io.hpp"
 * using namespace io;
 *
 * int main() {
 *     // Tipos estándar - funcionan automáticamente
 *     int normal = 42;
 *     std::cout << normal << std::endl;
 *
 *     // Tipos __int128 - ahora funcionan automáticamente
 *     __int128 big = /* valor grande */
;
*std::cout << big << std::endl;
** // Formateo especial (cuando se implemente)
    *std::cout
    << format_hex(big) << std::endl;
*std::cout << format_binary(big, 64) << std::endl;
** // Entrada también funciona
 *__int128 input;
*std::cin >> input;
**return 0;
*
}
* ``` * /