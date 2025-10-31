#pragma once

/**
 * @file literals.hpp
 * @brief Header principal unificado para todos los literales de usuario
 * @author AlgoritmiaCombinatoria Project
 * @date 2024
 * @version 1.0
 *
 * Este header incluye todos los literales de usuario (UDL) del proyecto
 * organizados en módulos especializados:
 *
 * - integral_literals/: Literales para tipos enteros (__int128, binarios, hex)
 * - multiprecision_literals/: Literales para tipos de precisión arbitraria
 *
 * Uso recomendado:
 * ```cpp
 * #include "literals/literals.hpp"
 * using namespace literals;
 *
 * // Todos los literales disponibles automáticamente
 * auto n1 = 123456789012345678901234567890_i128;     // __int128
 * auto n2 = 123456789012345678901234567890_u128;     // unsigned __int128
 * auto n3 = "DEADBEEF"_i128_hex;                     // Hexadecimal
 * auto n4 = "10101010"_i128_bin;                     // Binario
 * auto n5 = 999999999999999999999999999_cpp_int;     // Boost cpp_int
 * ```
 */

//==============================================================================
// MÓDULOS DE LITERALES INTEGRALES
//==============================================================================

#include "integral_literals/int128_literals.hpp"
// #include "integral_literals/constexpr_literals.hpp"  // TODO: Migrar
// #include "integral_literals/binary_hex_literals.hpp" // TODO: Crear

//==============================================================================
// MÓDULOS DE LITERALES MULTIPRECISIÓN
//==============================================================================

// #include "multiprecision_literals/cpp_int_literals.hpp"         // TODO:
// Migrar #include "multiprecision_literals/fixed_precision_literals.hpp" //
// TODO: Crear #include "multiprecision_literals/rational_literals.hpp" // TODO:
// Crear

//==============================================================================
// API PÚBLICA UNIFICADA
//==============================================================================

namespace literals {

// Todos los literales integrales disponibles automáticamente
using namespace integral_literals;

// TODO: Cuando se creen los módulos multiprecisión
// using namespace multiprecision_literals;

//==============================================================================
// EJEMPLOS Y DOCUMENTACIÓN
//==============================================================================

/**
 * @example Uso de literales integrales
 *
 * ```cpp
 * #include "literals/literals.hpp"
 * using namespace literals;
 *
 * void ejemplos_integrales() {
 *     // Literales decimales para __int128
 *     auto big1 = 123456789012345678901234567890_i128;
 *     auto big2 = 123456789012345678901234567890_u128;
 *
 *     // Literales desde números pequeños
 *     auto small1 = 42_i128;
 *     auto small2 = 42_u128;
 *
 *     // Literales hexadecimales
 *     auto hex1 = "DEADBEEF"_i128_hex;
 *     auto hex2 = "CAFEBABE"_u128_hex;
 *
 *     // Literales binarios
 *     auto bin1 = "1010101010101010"_i128_bin;
 *     auto bin2 = "1111000011110000"_u128_bin;
 * }
 * ```
 */

/**
 * @example Uso de literales multiprecisión (futuro)
 *
 * ```cpp
 * #include "literals/literals.hpp"
 * using namespace literals;
 *
 * void ejemplos_multiprecision() {
 *     // TODO: Cuando se implementen
 *
 *     // Literales para cpp_int
 *     // auto huge = 999999999999999999999999999999999999_cpp_int;
 *
 *     // Literales para decimales de precisión fija
 *     // auto pi = 3.14159265358979323846264338327950288_cpp_dec_50;
 *
 *     // Literales para números racionales
 *     // auto frac = 1/3_rational;
 * }
 * ```
 */

} // namespace literals

//==============================================================================
// ALIAS GLOBALES PARA COMPATIBILIDAD
//==============================================================================

// Los literales también están disponibles en el namespace global
// para compatibilidad con código existente
using namespace literals;