#pragma once

/**
 * @file integral_power_functions.hpp
 * @brief Header principal para todas las funciones de potencias enteras
 * @author AlgoritmiaCombinatoria Project
 * @date 2024
 * @version 1.0
 *
 * Este es el header principal que incluye todas las funciones optimizadas
 * para cálculo de potencias con enteros. Proporciona una API unificada
 * que incluye:
 *
 * - Funciones básicas de potencias
 * - Especializaciones por traits (signed/unsigned)
 * - Optimizaciones para potencias de 2
 * - Detección automática de optimizaciones
 *
 * Uso recomendado:
 * ```cpp
 * #include "number_calc/integral_power_functions.hpp"
 *
 * // Función inteligente con detección automática
 * auto result = number_calc::integral_power_functions::int_power_smart(base,
 * exp);
 *
 * // O usar la función específica
 * auto result2 = number_calc::integral_power_functions::int_power_2<int>(10);
 * // 2^10
 * ```
 */

//==============================================================================
// INCLUDES DE TODOS LOS MÓDULOS
//==============================================================================

#include "integral_power_functions/basic_power_functions.hpp"
#include "integral_power_functions/power_of_2_optimizations.hpp"
#include "integral_power_functions/trait_based_specializations.hpp"

//==============================================================================
// API PÚBLICA SIMPLIFICADA
//==============================================================================

namespace number_calc {

/**
 * @brief API pública simplificada para funciones de potencias
 *
 * Esta API proporciona acceso directo a las funciones más útiles
 * sin tener que navegar por los namespaces internos.
 */

// Función principal recomendada
using integral_power_functions::int_power_smart;

// Funciones básicas
using integral_power_functions::int_power_basic;
using integral_power_functions::int_power_checked;
using integral_power_functions::int_power_safe;
using integral_power_functions::int_power_safe_pair;

// Función con dispatch automático
using integral_power_functions::int_power_new;

// Optimizaciones específicas
using integral_power_functions::int_power_2;

// Funciones de análisis
using integral_power_functions::find_power_of_2_exponent;
using integral_power_functions::is_power_of_2;

} // namespace number_calc

//==============================================================================
// DOCUMENTACIÓN DE LA API
//==============================================================================

/**
 * @namespace number_calc::integral_power_functions
 * @brief Namespace interno con todas las implementaciones detalladas
 *
 * Contiene todas las funciones organizadas por categorías:
 * - basic_power_functions.hpp: Implementaciones base
 * - trait_based_specializations.hpp: Especializaciones signed/unsigned
 * - power_of_2_optimizations.hpp: Optimizaciones para base = 2
 */

/**
 * @example Ejemplos de uso básico
 *
 * ```cpp
 * #include "number_calc/integral_power_functions.hpp"
 * using namespace number_calc;
 *
 * // Casos básicos
 * int result1 = int_power_smart(2, 10);        // 1024, usa optimización 2^n
 * long result2 = int_power_smart(3L, 15);      // Dispatch automático
 *
 * // Especializaciones directas
 * auto result3 = int_power_2<int>(20);         // 2^20 = 1048576
 *
 * // Con verificación de overflow
 * auto [result4, success] = int_power_checked(2, 30);
 * if (success) {
 *     std::cout << "2^30 = " << result4 << std::endl;
 * }
 *
 * // Análisis de optimización
 * if (is_power_of_2(8)) {
 *     int exp = find_power_of_2_exponent(8);  // exp = 3
 *     std::cout << "8 = 2^" << exp << std::endl;
 * }
 * ```
 */

/**
 * @example Casos de uso avanzados
 *
 * ```cpp
 * #include "number_calc/integral_power_functions.hpp"
 * using namespace number_calc;
 *
 * // Con tipos de 128 bits (si están disponibles)
 * #if defined(__GNUC__) || defined(__clang__)
 * __int128 big_result = int_power_smart(__int128{2}, 100);
 * #endif
 *
 * // Template genérico
 * template<typename T>
 * T compute_power_sequence(T base, int max_exp) {
 *     T result = 1;
 *     for (int i = 1; i <= max_exp; ++i) {
 *         result += int_power_smart(base, i);
 *     }
 *     return result;
 * }
 *
 * // Optimización automática para diferentes bases
 * int test_bases[] = {2, 3, 4, 8, 16, 17};
 * for (int base : test_bases) {
 *     int result = int_power_smart(base, 10);
 *     bool is_pow2 = is_power_of_2(base);
 *     std::cout << base << "^10 = " << result
 *               << (is_pow2 ? " (optimized)" : "") << std::endl;
 * }
 * ```
 */