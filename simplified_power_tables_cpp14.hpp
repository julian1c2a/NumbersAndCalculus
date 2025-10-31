#pragma once

/**
 * @file simplified_power_tables_cpp14.hpp
 * @brief Versión C++14 compatible de tablas constexpr para pruebas
 */

#include <array>
#include <cstdint>
#include <type_traits>

namespace number_calc {
namespace integral_power_functions {
namespace lookup_tables {

// Tablas constexpr para potencias de 2
constexpr std::array<int8_t, 7> power_of_2_int8 = {{
    1, 2, 4, 8, 16, 32, 64 // 2^0 a 2^6
}};

constexpr std::array<uint8_t, 8> power_of_2_uint8 = {{
    1, 2, 4, 8, 16, 32, 64, 128 // 2^0 a 2^7
}};

constexpr std::array<int16_t, 15> power_of_2_int16 = {{
    1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192,
    16384 // 2^0 a 2^14
}};

constexpr std::array<uint16_t, 16> power_of_2_uint16 = {{
    1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384,
    32768 // 2^0 a 2^15
}};

constexpr std::array<int32_t, 31> power_of_2_int32 = {{
    1,         2,         4,         8,        16,       32,       64,
    128,       256,       512,       1024,     2048,     4096,     8192,
    16384,     32768,     65536,     131072,   262144,   524288,   1048576,
    2097152,   4194304,   8388608,   16777216, 33554432, 67108864, 134217728,
    268435456, 536870912, 1073741824
    // 2^0 a 2^30 (2^31 = 2147483648 causa overflow en int32_t)
}};

constexpr std::array<uint32_t, 32> power_of_2_uint32 = {{
    1U,          2U,         4U,        8U,         16U,        32U,
    64U,         128U,       256U,      512U,       1024U,      2048U,
    4096U,       8192U,      16384U,    32768U,     65536U,     131072U,
    262144U,     524288U,    1048576U,  2097152U,   4194304U,   8388608U,
    16777216U,   33554432U,  67108864U, 134217728U, 268435456U, 536870912U,
    1073741824U, 2147483648U
    // 2^0 a 2^31 (2^32 = 4294967296 causa overflow en uint32_t)
}};

// Funciones específicas por tipo - implementaciones
constexpr int8_t get_power_of_2_int8(int exp) {
  return (exp >= 0 && exp < 7) ? power_of_2_int8[exp] : 0;
}

constexpr uint8_t get_power_of_2_uint8(int exp) {
  return (exp >= 0 && exp < 8) ? power_of_2_uint8[exp] : 0;
}

constexpr int16_t get_power_of_2_int16(int exp) {
  return (exp >= 0 && exp < 15) ? power_of_2_int16[exp] : 0;
}

constexpr uint16_t get_power_of_2_uint16(int exp) {
  return (exp >= 0 && exp < 16) ? power_of_2_uint16[exp] : 0;
}

constexpr int32_t get_power_of_2_int32(int exp) {
  return (exp >= 0 && exp < 31) ? power_of_2_int32[exp] : 0;
}

constexpr uint32_t get_power_of_2_uint32(int exp) {
  return (exp >= 0 && exp < 32) ? power_of_2_uint32[exp] : 0;
}

// Template unificado con especialización
template <typename T> constexpr T get_power_of_2_from_table(int exp);

// Funciones auxiliares para verificación de rangos - declaración primaria
template <typename T> constexpr int get_max_power_of_2_exponent();

// Especializaciones explícitas
template <> constexpr int get_max_power_of_2_exponent<int8_t>() { return 6; }

template <> constexpr int get_max_power_of_2_exponent<uint8_t>() { return 7; }

template <> constexpr int get_max_power_of_2_exponent<int16_t>() { return 14; }

template <> constexpr int get_max_power_of_2_exponent<uint16_t>() { return 15; }

template <> constexpr int get_max_power_of_2_exponent<int32_t>() { return 30; }

template <> constexpr int get_max_power_of_2_exponent<uint32_t>() { return 31; }

// Especializaciones de template para acceso unificado
template <> constexpr int8_t get_power_of_2_from_table<int8_t>(int exp) {
  return get_power_of_2_int8(exp);
}

template <> constexpr uint8_t get_power_of_2_from_table<uint8_t>(int exp) {
  return get_power_of_2_uint8(exp);
}

template <> constexpr int16_t get_power_of_2_from_table<int16_t>(int exp) {
  return get_power_of_2_int16(exp);
}

template <> constexpr uint16_t get_power_of_2_from_table<uint16_t>(int exp) {
  return get_power_of_2_uint16(exp);
}

template <> constexpr int32_t get_power_of_2_from_table<int32_t>(int exp) {
  return get_power_of_2_int32(exp);
}

template <> constexpr uint32_t get_power_of_2_from_table<uint32_t>(int exp) {
  return get_power_of_2_uint32(exp);
}

template <typename T>
constexpr bool is_valid_power_of_2_exponent(int exponent) {
  return exponent >= 0 && exponent <= get_max_power_of_2_exponent<T>();
}

// Ejemplo de uso constexpr (comentado para evitar problemas de linkado)
// constexpr int8_t val1 = get_power_of_2_int8(5);      // = 32
// constexpr uint16_t val2 = get_power_of_2_uint16(10); // = 1024

} // namespace lookup_tables
} // namespace integral_power_functions
} // namespace number_calc