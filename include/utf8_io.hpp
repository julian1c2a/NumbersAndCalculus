#pragma once

#include <codecvt>
#include <fstream>
#include <iostream>
#include <locale>
#include <string>
#include <type_traits>


#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
#include <windows.h>

#endif

namespace utf8_io {

/**
 * @brief Inicializar soporte UTF-8 para la consola y streams
 */
inline void initialize_utf8_support() {
#ifdef _WIN32
  // Configurar página de código UTF-8 para Windows
  SetConsoleOutputCP(CP_UTF8);
  SetConsoleCP(CP_UTF8);

  // Configurar modo UTF-8 para stdout/stderr
  _setmode(_fileno(stdout), _O_U8TEXT);
  _setmode(_fileno(stderr), _O_U8TEXT);
  _setmode(_fileno(stdin), _O_U8TEXT);
#endif

  // Configurar locale UTF-8
  try {
    std::locale::global(std::locale(""));
  } catch (...) {
    // Fallback si el locale no está disponible
    std::locale::global(std::locale("C"));
  }

  // Configurar streams para UTF-8
  std::cout.imbue(std::locale());
  std::cerr.imbue(std::locale());
  std::cin.imbue(std::locale());
}

/**
 * @brief Escribir string UTF-8 a stream de salida
 */
template <typename CharT = char>
inline std::basic_ostream<CharT> &write_utf8(std::basic_ostream<CharT> &os,
                                             const std::string &utf8_str) {
  if constexpr (std::is_same_v<CharT, char>) {
    // Para char streams, escribir directamente
    return os << utf8_str;
  } else {
    // Para wchar_t streams, convertir
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::wstring wide_str = converter.from_bytes(utf8_str);
    return os << wide_str;
  }
}

/**
 * @brief Leer string UTF-8 desde stream de entrada
 */
template <typename CharT = char>
inline std::string read_utf8_line(std::basic_istream<CharT> &is) {
  if constexpr (std::is_same_v<CharT, char>) {
    // Para char streams, leer directamente
    std::string line;
    std::getline(is, line);
    return line;
  } else {
    // Para wchar_t streams, convertir
    std::wstring wide_line;
    std::getline(is, wide_line);
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.to_bytes(wide_line);
  }
}

/**
 * @brief Abrir archivo con soporte UTF-8
 */
class utf8_ifstream : public std::ifstream {
public:
  explicit utf8_ifstream(const std::string &filename)
      : std::ifstream(filename, std::ios::binary) {
    // Configurar para UTF-8
    this->imbue(std::locale());
  }

  explicit utf8_ifstream(const char *filename)
      : utf8_ifstream(std::string(filename)) {}
};

/**
 * @brief Escribir archivo con soporte UTF-8
 */
class utf8_ofstream : public std::ofstream {
public:
  explicit utf8_ofstream(const std::string &filename)
      : std::ofstream(filename, std::ios::binary) {
    // Configurar para UTF-8
    this->imbue(std::locale());
  }

  explicit utf8_ofstream(const char *filename)
      : utf8_ofstream(std::string(filename)) {}
};

/**
 * @brief Operadores de conveniencia para UTF-8
 */
struct utf8_output_wrapper {
  std::ostream &os;
  explicit utf8_output_wrapper(std::ostream &stream) : os(stream) {}
};

inline utf8_output_wrapper utf8(std::ostream &os) {
  return utf8_output_wrapper(os);
}

template <typename T>
inline std::ostream &operator<<(utf8_output_wrapper wrapper, const T &value) {
  return wrapper.os << value;
}

// Especialización para strings UTF-8
inline std::ostream &operator<<(utf8_output_wrapper wrapper,
                                const std::string &utf8_str) {
  return write_utf8(wrapper.os, utf8_str);
}

/**
 * @brief Utilidades para validación UTF-8
 */
inline bool is_valid_utf8(const std::string &str) {
  const unsigned char *bytes =
      reinterpret_cast<const unsigned char *>(str.c_str());
  size_t len = str.length();

  for (size_t i = 0; i < len; ++i) {
    if (bytes[i] <= 0x7F) {
      // ASCII
      continue;
    } else if ((bytes[i] >> 5) == 0x06) {
      // 110xxxxx - 2 bytes
      if (++i >= len || (bytes[i] >> 6) != 0x02)
        return false;
    } else if ((bytes[i] >> 4) == 0x0E) {
      // 1110xxxx - 3 bytes
      if (++i >= len || (bytes[i] >> 6) != 0x02)
        return false;
      if (++i >= len || (bytes[i] >> 6) != 0x02)
        return false;
    } else if ((bytes[i] >> 3) == 0x1E) {
      // 11110xxx - 4 bytes
      if (++i >= len || (bytes[i] >> 6) != 0x02)
        return false;
      if (++i >= len || (bytes[i] >> 6) != 0x02)
        return false;
      if (++i >= len || (bytes[i] >> 6) != 0x02)
        return false;
    } else {
      return false;
    }
  }
  return true;
}

/**
 * @brief Sanitizar string para ASCII seguro
 */
inline std::string sanitize_to_ascii(const std::string &input) {
  std::string result;
  result.reserve(input.length());

  for (unsigned char c : input) {
    if (c <= 0x7F) {
      result.push_back(c);
    } else {
      // Reemplazar caracteres no-ASCII con '?'
      result.push_back('?');
    }
  }
  return result;
}

} // namespace utf8_io