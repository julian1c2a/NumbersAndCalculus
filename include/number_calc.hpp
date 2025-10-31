#pragma once

/**
 * @file number_calc.hpp
 * @brief Header principal del proyecto AlgoritmiaCombinatoria
 * @author AlgoritmiaCombinatoria Project
 * @date 2024
 * @version 3.0
 *
 * Este header unifica todos los módulos de cálculo numérico del proyecto.
 * Incluye funciones optimizadas para:
 *
 * - Potencias enteras con detección automática de optimizaciones
 * - Traits extendidos para soporte de __int128
 * - Especializaciones por tipos signed/unsigned
 * - Optimizaciones específicas para potencias de 2
 *
 * Arquitectura modular:
 * - type_traits/: Traits extendidos para tipos no estándar
 * - integral_power_functions/: Funciones de potencias organizadas por
 * especialización
 */

// Headers principales
#include "number_calc/integral_power_functions.hpp"
#include "type_traits/extended_integral_traits.hpp"


//==============================================================================
// API PÚBLICA - REDIRIGIR A MÓDULOS ESPECIALIZADOS
//==============================================================================

/**
 * @brief Namespace público del proyecto
 *
 * Todas las funciones están disponibles a través de los módulos especializados
 * incluidos automáticamente. Ver documentación en:
 * - integral_power_functions.hpp para funciones de potencias
 * - extended_integral_traits.hpp para traits extendidos
 */
namespace number_calc {
// Todas las funciones están disponibles a través de:
// - number_calc::int_power_smart() - Función principal recomendada
// - number_calc::int_power_2() - Optimizada para potencias de 2
// - number_calc::int_power_basic() - Función básica
// - Y muchas más... ver integral_power_functions.hpp
}

//==============================================================================
// EJEMPLOS DE USO
//==============================================================================

/**
 * @example Uso básico de la API modular
 *
 * ```cpp
 * #include "number_calc.hpp"
 * using namespace number_calc;
 *
 * // Función principal recomendada (detección automática de optimizaciones)
 * int result1 = int_power_smart(2, 10);        // 1024, usa optimización 2^n
 * long result2 = int_power_smart(3L, 15);      // Dispatch automático
 * signed/unsigned
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