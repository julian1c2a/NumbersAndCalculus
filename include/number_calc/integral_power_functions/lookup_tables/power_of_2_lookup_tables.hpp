#pragma once

/**
 * @file power_of_2_lookup_tables.hpp
 * @brief Tablas constexpr precalculadas para potencias de 2 en tipos pequeños
 * @author AlgoritmiaCombinatoria Project
 * @date 2024
 * @version 1.0
 *
 * Este archivo contiene tablas constexpr precalculadas para potencias de 2
 * optimizadas para tipos enteros pequeños (8 y 16 bits). Proporciona acceso
 * inmediato en tiempo de compilación sin necesidad de cálculos.
 *
 * Optimizaciones incluidas:
 * - int8_t:  2^0 hasta 2^6 (2^7 = 128 overflow)
 * - uint8_t: 2^0 hasta 2^7 (2^8 = 256 overflow)
 * - int16_t: 2^0 hasta 2^14 (2^15 = 32768 overflow)
 * - uint16_t: 2^0 hasta 2^15 (2^16 = 65536 overflow)
 * - int32_t: 2^0 hasta 2^30 (2^31 = 2147483648 overflow)
 * - uint32_t: 2^0 hasta 2^31 (2^32 = 4294967296 overflow)
 */

#include "../../../type_traits/extended_integral_traits.hpp"
#include "../../error_handling.hpp"
#include <array>
#include <cstdint>
#include <optional>
#include <stdexcept>

namespace number_calc {
namespace integral_power_functions {
namespace lookup_tables {

using namespace number_calc::type_traits;

//==============================================================================
// TABLAS CONSTEXPR PARA POTENCIAS DE 2
//==============================================================================

/**
 * @brief Tabla constexpr de potencias de 2 para int8_t
 * Contiene 2^0, 2^1, 2^2, ..., 2^6
 * No incluye 2^7 = 128 porque causaría overflow (int8_t max = 127)
 */
constexpr std::array<int8_t, 7> power_of_2_int8 = {
    1,  // 2^0
    2,  // 2^1
    4,  // 2^2
    8,  // 2^3
    16, // 2^4
    32, // 2^5
    64  // 2^6
};

/**
 * @brief Tabla constexpr de potencias de 2 para uint8_t
 * Contiene 2^0, 2^1, 2^2, ..., 2^7
 * No incluye 2^8 = 256 porque causaría overflow (uint8_t max = 255)
 */
constexpr std::array<uint8_t, 8> power_of_2_uint8 = {
    1,  // 2^0
    2,  // 2^1
    4,  // 2^2
    8,  // 2^3
    16, // 2^4
    32, // 2^5
    64, // 2^6
    128 // 2^7
};

/**
 * @brief Tabla constexpr de potencias de 2 para int16_t
 * Contiene 2^0, 2^1, 2^2, ..., 2^14
 * No incluye 2^15 = 32768 porque causaría overflow (int16_t max = 32767)
 */
constexpr std::array<int16_t, 15> power_of_2_int16 = {
    1,    // 2^0
    2,    // 2^1
    4,    // 2^2
    8,    // 2^3
    16,   // 2^4
    32,   // 2^5
    64,   // 2^6
    128,  // 2^7
    256,  // 2^8
    512,  // 2^9
    1024, // 2^10
    2048, // 2^11
    4096, // 2^12
    8192, // 2^13
    16384 // 2^14
};

/**
 * @brief Tabla constexpr de potencias de 2 para uint16_t
 * Contiene 2^0, 2^1, 2^2, ..., 2^15
 * No incluye 2^16 = 65536 porque causaría overflow (uint16_t max = 65535)
 */
constexpr std::array<uint16_t, 16> power_of_2_uint16 = {
    1,     // 2^0
    2,     // 2^1
    4,     // 2^2
    8,     // 2^3
    16,    // 2^4
    32,    // 2^5
    64,    // 2^6
    128,   // 2^7
    256,   // 2^8
    512,   // 2^9
    1024,  // 2^10
    2048,  // 2^11
    4096,  // 2^12
    8192,  // 2^13
    16384, // 2^14
    32768  // 2^15
};

/**
 * @brief Tabla constexpr precalculada para potencias de 2 en int32_t
 *
 * Contiene 2^0 hasta 2^30 (31 valores).
 * 2^31 = 2147483648 causaría overflow en int32_t (max = 2147483647).
 */
constexpr std::array<int32_t, 31> power_of_2_int32 = {
    1,         // 2^0
    2,         // 2^1
    4,         // 2^2
    8,         // 2^3
    16,        // 2^4
    32,        // 2^5
    64,        // 2^6
    128,       // 2^7
    256,       // 2^8
    512,       // 2^9
    1024,      // 2^10
    2048,      // 2^11
    4096,      // 2^12
    8192,      // 2^13
    16384,     // 2^14
    32768,     // 2^15
    65536,     // 2^16
    131072,    // 2^17
    262144,    // 2^18
    524288,    // 2^19
    1048576,   // 2^20
    2097152,   // 2^21
    4194304,   // 2^22
    8388608,   // 2^23
    16777216,  // 2^24
    33554432,  // 2^25
    67108864,  // 2^26
    134217728, // 2^27
    268435456, // 2^28
    536870912, // 2^29
    1073741824 // 2^30
};

/**
 * @brief Tabla constexpr precalculada para potencias de 2 en uint32_t
 *
 * Contiene 2^0 hasta 2^31 (32 valores).
 * 2^32 = 4294967296 causaría overflow en uint32_t (max = 4294967295).
 */
constexpr std::array<uint32_t, 32> power_of_2_uint32 = {
    1U,          // 2^0
    2U,          // 2^1
    4U,          // 2^2
    8U,          // 2^3
    16U,         // 2^4
    32U,         // 2^5
    64U,         // 2^6
    128U,        // 2^7
    256U,        // 2^8
    512U,        // 2^9
    1024U,       // 2^10
    2048U,       // 2^11
    4096U,       // 2^12
    8192U,       // 2^13
    16384U,      // 2^14
    32768U,      // 2^15
    65536U,      // 2^16
    131072U,     // 2^17
    262144U,     // 2^18
    524288U,     // 2^19
    1048576U,    // 2^20
    2097152U,    // 2^21
    4194304U,    // 2^22
    8388608U,    // 2^23
    16777216U,   // 2^24
    33554432U,   // 2^25
    67108864U,   // 2^26
    134217728U,  // 2^27
    268435456U,  // 2^28
    536870912U,  // 2^29
    1073741824U, // 2^30
    2147483648U  // 2^31
};

//==============================================================================
// FUNCIONES DE ACCESO SEGURO A LAS TABLAS
//==============================================================================

/**
 * @brief Obtiene 2^exp desde tabla constexpr para int8_t (versión segura con
 * std::optional)
 * @param exp Exponente (0 <= exp <= 6)
 * @return std::optional<int8_t> con 2^exp si exp es válido, std::nullopt si
 * está fuera de rango
 * @note Esta función es noexcept y usa std::optional para manejo de errores
 * (requiere C++17)
 *
 * Esta función proporciona acceso inmediato en tiempo de compilación
 * cuando exp es una constante conocida, con manejo seguro de errores.
 */
constexpr std::optional<int8_t> get_power_of_2_int8_safe(int exp) noexcept {
  if (exp < 0 || exp >= static_cast<int>(power_of_2_int8.size())) {
    return std::nullopt;
  }
  return power_of_2_int8[exp];
}

/**
 * @brief Obtiene 2^exp desde tabla constexpr para int8_t (versión legacy con
 * excepciones)
 * @param exp Exponente (0 <= exp <= 6)
 * @return 2^exp como int8_t
 * @throws std::out_of_range si exp está fuera del rango válido
 * @deprecated Usar get_power_of_2_int8_safe() para funciones de cálculo
 *
 * Esta función proporciona acceso inmediato en tiempo de compilación
 * cuando exp es una constante conocida.
 */
constexpr int8_t get_power_of_2_int8(int exp) {
  if (exp < 0 || exp >= static_cast<int>(power_of_2_int8.size())) {
    throw std::out_of_range(
        "get_power_of_2_int8: exponente fuera de rango [0, 6]");
  }
  return power_of_2_int8[exp];
}

/**
 * @brief Obtiene 2^exp desde tabla constexpr para uint8_t (versión segura con
 * std::optional)
 * @param exp Exponente (0 <= exp <= 7)
 * @return std::optional<uint8_t> con 2^exp si exp es válido, std::nullopt si
 * está fuera de rango
 * @note Esta función es noexcept y usa std::optional para manejo de errores
 * (requiere C++17)
 */
constexpr std::optional<uint8_t> get_power_of_2_uint8_safe(int exp) noexcept {
  if (exp < 0 || exp >= static_cast<int>(power_of_2_uint8.size())) {
    return std::nullopt;
  }
  return power_of_2_uint8[exp];
}

/**
 * @brief Obtiene 2^exp desde tabla constexpr para uint8_t (versión legacy)
 * @param exp Exponente (0 <= exp <= 7)
 * @return 2^exp como uint8_t
 * @throws std::out_of_range si exp está fuera del rango válido
 * @deprecated Usar get_power_of_2_uint8_safe() para funciones de cálculo
 */
constexpr uint8_t get_power_of_2_uint8(int exp) {
  if (exp < 0 || exp >= static_cast<int>(power_of_2_uint8.size())) {
    throw std::out_of_range(
        "get_power_of_2_uint8: exponente fuera de rango [0, 7]");
  }
  return power_of_2_uint8[exp];
}

/**
 * @brief Obtiene 2^exp desde tabla constexpr para int16_t
 * @param exp Exponente (0 <= exp <= 14)
 * @return 2^exp como int16_t
 * @throws std::out_of_range si exp está fuera del rango válido
 */
constexpr int16_t get_power_of_2_int16(int exp) {
  if (exp < 0 || exp >= static_cast<int>(power_of_2_int16.size())) {
    throw std::out_of_range(
        "get_power_of_2_int16: exponente fuera de rango [0, 14]");
  }
  return power_of_2_int16[exp];
}

/**
 * @brief Obtiene 2^exp desde tabla constexpr para uint16_t
 * @param exp Exponente (0 <= exp <= 15)
 * @return 2^exp como uint16_t
 * @throws std::out_of_range si exp está fuera del rango válido
 */
constexpr uint16_t get_power_of_2_uint16(int exp) {
  if (exp < 0 || exp >= static_cast<int>(power_of_2_uint16.size())) {
    throw std::out_of_range(
        "get_power_of_2_uint16: exponente fuera de rango [0, 15]");
  }
  return power_of_2_uint16[exp];
}

/**
 * @brief Obtiene 2^exp desde tabla constexpr para int32_t
 * @param exp Exponente (0 <= exp <= 30)
 * @return 2^exp como int32_t
 * @throws std::out_of_range si exp está fuera del rango válido
 */
constexpr int32_t get_power_of_2_int32(int exp) {
  if (exp < 0 || exp >= static_cast<int>(power_of_2_int32.size())) {
    throw std::out_of_range(
        "get_power_of_2_int32: exponente fuera de rango [0, 30]");
  }
  return power_of_2_int32[exp];
}

/**
 * @brief Obtiene 2^exp desde tabla constexpr para uint32_t
 * @param exp Exponente (0 <= exp <= 31)
 * @return 2^exp como uint32_t
 * @throws std::out_of_range si exp está fuera del rango válido
 */
constexpr uint32_t get_power_of_2_uint32(int exp) {
  if (exp < 0 || exp >= static_cast<int>(power_of_2_uint32.size())) {
    throw std::out_of_range(
        "get_power_of_2_uint32: exponente fuera de rango [0, 31]");
  }
  return power_of_2_uint32[exp];
}

//==============================================================================
// FUNCIONES TEMPLATE GENÉRICAS CON ESPECIALIZACIÓN AUTOMÁTICA
//==============================================================================

/**
 * @brief Función template genérica para obtener potencias de 2 desde tabla
 * @tparam T Tipo entero pequeño (int8_t, uint8_t, int16_t, uint16_t)
 * @param exp Exponente
 * @return 2^exp desde tabla precalculada
 *
 * Esta función se especializa automáticamente para cada tipo y proporciona
 * acceso inmediato en tiempo de compilación cuando es posible.
 */
template <typename T> constexpr T get_power_of_2_from_table(int exp) {
  if constexpr (std::is_same_v<T, int8_t>) {
    return get_power_of_2_int8(exp);
  } else if constexpr (std::is_same_v<T, uint8_t>) {
    return get_power_of_2_uint8(exp);
  } else if constexpr (std::is_same_v<T, int16_t>) {
    return get_power_of_2_int16(exp);
  } else if constexpr (std::is_same_v<T, uint16_t>) {
    return get_power_of_2_uint16(exp);
  } else {
    static_assert(std::is_same_v<T, int8_t> || std::is_same_v<T, uint8_t> ||
                      std::is_same_v<T, int16_t> || std::is_same_v<T, uint16_t>,
                  "get_power_of_2_from_table solo soporta int8_t, uint8_t, "
                  "int16_t, uint16_t");
  }
}

//==============================================================================
// FUNCIONES DE VERIFICACIÓN DE RANGOS
//==============================================================================

/**
 * @brief Verifica si un exponente está en el rango válido para un tipo
 * @tparam T Tipo entero pequeño
 * @param exp Exponente a verificar
 * @return true si exp está en rango válido, false en caso contrario
 */
template <typename T> constexpr bool is_valid_power_of_2_exponent(int exp) {
  if constexpr (std::is_same_v<T, int8_t>) {
    return exp >= 0 && exp <= 6;
  } else if constexpr (std::is_same_v<T, uint8_t>) {
    return exp >= 0 && exp <= 7;
  } else if constexpr (std::is_same_v<T, int16_t>) {
    return exp >= 0 && exp <= 14;
  } else if constexpr (std::is_same_v<T, uint16_t>) {
    return exp >= 0 && exp <= 15;
  } else {
    return false;
  }
}

/**
 * @brief Obtiene el exponente máximo válido para un tipo
 * @tparam T Tipo entero pequeño
 * @return Exponente máximo sin overflow
 */
template <typename T> constexpr int get_max_power_of_2_exponent() {
  if constexpr (std::is_same_v<T, int8_t>) {
    return 6;
  } else if constexpr (std::is_same_v<T, uint8_t>) {
    return 7;
  } else if constexpr (std::is_same_v<T, int16_t>) {
    return 14;
  } else if constexpr (std::is_same_v<T, uint16_t>) {
    return 15;
  } else {
    static_assert(std::is_same_v<T, int8_t> || std::is_same_v<T, uint8_t> ||
                      std::is_same_v<T, int16_t> || std::is_same_v<T, uint16_t>,
                  "get_max_power_of_2_exponent solo soporta tipos pequeños");
    return -1;
  }
}

} // namespace lookup_tables
} // namespace integral_power_functions
} // namespace number_calc