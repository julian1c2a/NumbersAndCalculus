#pragma once

/**
 * @file simplified_power_tables.hpp
 * @brief Versión simplificada de tablas constexpr para pruebas
 */

#include <array>
#include <cstdint>


namespace number_calc {
namespace integral_power_functions {
namespace lookup_tables {

// Tablas constexpr para potencias de 2
constexpr std::array<int8_t, 7> power_of_2_int8 = {
    1, 2, 4, 8, 16, 32, 64 // 2^0 a 2^6
};

constexpr std::array<uint8_t, 8> power_of_2_uint8 = {
    1, 2, 4, 8, 16, 32, 64, 128 // 2^0 a 2^7
};

constexpr std::array<int16_t, 15> power_of_2_int16 = {
    1,   2,   4,    8,    16,   32,   64,   128,
    256, 512, 1024, 2048, 4096, 8192, 16384 // 2^0 a 2^14
};

constexpr std::array<uint16_t, 16> power_of_2_uint16 = {
    1,   2,   4,    8,    16,   32,   64,    128,
    256, 512, 1024, 2048, 4096, 8192, 16384, 32768 // 2^0 a 2^15
};

// Funciones de acceso específicas por tipo
constexpr int8_t get_power_of_2_int8(int exponent) {
  return (exponent >= 0 && exponent < 7) ? power_of_2_int8[exponent] : 0;
}

constexpr uint8_t get_power_of_2_uint8(int exponent) {
  return (exponent >= 0 && exponent < 8) ? power_of_2_uint8[exponent] : 0;
}

constexpr int16_t get_power_of_2_int16(int exponent) {
  return (exponent >= 0 && exponent < 15) ? power_of_2_int16[exponent] : 0;
}

constexpr uint16_t get_power_of_2_uint16(int exponent) {
  return (exponent >= 0 && exponent < 16) ? power_of_2_uint16[exponent] : 0;
}

// Función template genérica
template <typename T> constexpr T get_power_of_2_from_table(int exponent) {
  if constexpr (std::is_same_v<T, int8_t>) {
    return get_power_of_2_int8(exponent);
  } else if constexpr (std::is_same_v<T, uint8_t>) {
    return get_power_of_2_uint8(exponent);
  } else if constexpr (std::is_same_v<T, int16_t>) {
    return get_power_of_2_int16(exponent);
  } else if constexpr (std::is_same_v<T, uint16_t>) {
    return get_power_of_2_uint16(exponent);
  } else {
    return T{0}; // Tipo no soportado
  }
}

// Funciones auxiliares para verificación de rangos
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
    return -1;
  }
}

template <typename T>
constexpr bool is_valid_power_of_2_exponent(int exponent) {
  return exponent >= 0 && exponent <= get_max_power_of_2_exponent<T>();
}

} // namespace lookup_tables
} // namespace integral_power_functions
} // namespace number_calc