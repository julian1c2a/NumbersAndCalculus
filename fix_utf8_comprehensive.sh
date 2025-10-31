#!/bin/bash

# Script para arreglar caracteres UTF-8 problemáticos en archivos C++ y HPP
# y añadir soporte completo para UTF-8 IO

echo "=== Arreglando caracteres UTF-8 problemáticos ==="

# Función para arreglar caracteres en un archivo
fix_file_encoding() {
    local file="$1"
    echo "Procesando: $file"
    
    # Crear backup
    cp "$file" "$file.backup"
    
    # Reemplazos de caracteres problemáticos comunes
    sed -i 's/ó/o/g' "$file"          # ó -> o
    sed -i 's/á/a/g' "$file"          # á -> a
    sed -i 's/é/e/g' "$file"          # é -> e
    sed -i 's/í/i/g' "$file"          # í -> i
    sed -i 's/ú/u/g' "$file"          # ú -> u
    sed -i 's/ñ/n/g' "$file"          # ñ -> n
    sed -i 's/Ñ/N/g' "$file"          # Ñ -> N
    sed -i 's/ü/u/g' "$file"          # ü -> u
    sed -i 's/Ü/U/g' "$file"          # Ü -> U
    sed -i 's/¿//g' "$file"           # Eliminar ¿
    sed -i 's/¡//g' "$file"           # Eliminar ¡
    sed -i 's/–/-/g' "$file"          # En dash -> hyphen
    sed -i 's/—/-/g' "$file"          # Em dash -> hyphen  
    sed -i 's/"/"/g' "$file"          # Smart quote izq -> normal
    sed -i 's/"/"/g' "$file"          # Smart quote der -> normal
    sed -i 's/'/'"'"'/g' "$file"      # Smart apostrophe -> normal
    sed -i 's/'/'"'"'/g' "$file"      # Smart apostrophe -> normal
    sed -i 's/…/.../g' "$file"        # Ellipsis -> triple dot
    sed -i 's/•/*/g' "$file"          # Bullet -> asterisk
    sed -i 's/✓/[OK]/g' "$file"       # Check mark -> [OK]
    sed -i 's/✗/[ERROR]/g' "$file"    # X mark -> [ERROR]
    sed -i 's/←/<-/g' "$file"         # Left arrow -> <-
    sed -i 's/→/->/g' "$file"         # Right arrow -> ->
    sed -i 's/↑/^/g' "$file"          # Up arrow -> ^
    sed -i 's/↓/v/g' "$file"          # Down arrow -> v
    sed -i 's/°/deg/g' "$file"        # Degree symbol -> deg
    sed -i 's/±/+\/-/g' "$file"       # Plus minus -> +/-
    sed -i 's/×/x/g' "$file"          # Multiplication -> x
    sed -i 's/÷/\//g' "$file"         # Division -> /
    sed -i 's/≤/<=/g' "$file"         # Less equal -> <=
    sed -i 's/≥/>=/g' "$file"         # Greater equal -> >=
    sed -i 's/≠/!=/g' "$file"         # Not equal -> !=
    sed -i 's/≈/~=/g' "$file"         # Approximately -> ~=
    sed -i 's/∞/INF/g' "$file"        # Infinity -> INF
    sed -i 's/∑/SUM/g' "$file"        # Summation -> SUM
    sed -i 's/∏/PROD/g' "$file"       # Product -> PROD
    sed -i 's/∫/INTEGRAL/g' "$file"   # Integral -> INTEGRAL
    sed -i 's/∂/PARTIAL/g' "$file"    # Partial -> PARTIAL
    sed -i 's/∇/NABLA/g' "$file"      # Nabla -> NABLA
    sed -i 's/√/sqrt/g' "$file"       # Square root -> sqrt
    sed -i 's/∛/cbrt/g' "$file"       # Cube root -> cbrt
    sed -i 's/∜/fourthrt/g' "$file"   # Fourth root -> fourthrt
    sed -i 's/π/PI/g' "$file"         # Pi -> PI
    sed -i 's/θ/theta/g' "$file"      # Theta -> theta
    sed -i 's/φ/phi/g' "$file"        # Phi -> phi
    sed -i 's/λ/lambda/g' "$file"     # Lambda -> lambda
    sed -i 's/μ/mu/g' "$file"         # Mu -> mu
    sed -i 's/σ/sigma/g' "$file"      # Sigma -> sigma
    sed -i 's/Ω/OMEGA/g' "$file"      # Omega -> OMEGA
    sed -i 's/α/alpha/g' "$file"      # Alpha -> alpha
    sed -i 's/β/beta/g' "$file"       # Beta -> beta
    sed -i 's/γ/gamma/g' "$file"      # Gamma -> gamma
    sed -i 's/δ/delta/g' "$file"      # Delta -> delta
    sed -i 's/ε/epsilon/g' "$file"    # Epsilon -> epsilon
    sed -i 's/ζ/zeta/g' "$file"       # Zeta -> zeta
    sed -i 's/η/eta/g' "$file"        # Eta -> eta
    sed -i 's/κ/kappa/g' "$file"      # Kappa -> kappa
    sed -i 's/ν/nu/g' "$file"         # Nu -> nu
    sed -i 's/ξ/xi/g' "$file"         # Xi -> xi
    sed -i 's/ο/omicron/g' "$file"    # Omicron -> omicron
    sed -i 's/ρ/rho/g' "$file"        # Rho -> rho
    sed -i 's/τ/tau/g' "$file"        # Tau -> tau
    sed -i 's/υ/upsilon/g' "$file"    # Upsilon -> upsilon
    sed -i 's/χ/chi/g' "$file"        # Chi -> chi
    sed -i 's/ψ/psi/g' "$file"        # Psi -> psi
    sed -i 's/ω/omega/g' "$file"      # Omega lowercase -> omega
    
    # Caracteres de box drawing (comunes en outputs de consola)
    sed -i 's/┌/+/g' "$file"         # Box top-left -> +
    sed -i 's/┐/+/g' "$file"         # Box top-right -> +  
    sed -i 's/└/+/g' "$file"         # Box bottom-left -> +
    sed -i 's/┘/+/g' "$file"         # Box bottom-right -> +
    sed -i 's/├/+/g' "$file"         # Box left T -> +
    sed -i 's/┤/+/g' "$file"         # Box right T -> +
    sed -i 's/┬/+/g' "$file"         # Box top T -> +
    sed -i 's/┴/+/g' "$file"         # Box bottom T -> +
    sed -i 's/┼/+/g' "$file"         # Box cross -> +
    sed -i 's/│/|/g' "$file"         # Box vertical -> |
    sed -i 's/─/-/g' "$file"         # Box horizontal -> -
    sed -i 's/━/-/g' "$file"         # Heavy horizontal -> -
    sed -i 's/┃/|/g' "$file"         # Heavy vertical -> |
    sed -i 's/╋/+/g' "$file"         # Heavy cross -> +
    sed -i 's/╔/+/g' "$file"         # Double top-left -> +
    sed -i 's/╗/+/g' "$file"         # Double top-right -> +
    sed -i 's/╚/+/g' "$file"         # Double bottom-left -> +
    sed -i 's/╝/+/g' "$file"         # Double bottom-right -> +
    sed -i 's/╠/+/g' "$file"         # Double left T -> +
    sed -i 's/╣/+/g' "$file"         # Double right T -> +
    sed -i 's/╦/+/g' "$file"         # Double top T -> +
    sed -i 's/╩/+/g' "$file"         # Double bottom T -> +
    sed -i 's/╬/+/g' "$file"         # Double cross -> +
    sed -i 's/║/|/g' "$file"         # Double vertical -> |
    sed -i 's/═/-/g' "$file"         # Double horizontal -> -
    
    echo "  -> Caracteres arreglados en $file"
}

# Encontrar y procesar todos los archivos .cpp y .hpp
find . -name "*.cpp" -o -name "*.hpp" | while read -r file; do
    if [[ -f "$file" ]]; then
        fix_file_encoding "$file"
    fi
done

echo ""
echo "=== Creando soporte UTF-8 mejorado para IO ==="

# Crear un header mejorado para UTF-8 IO
cat > include/utf8_io.hpp << 'EOF'
#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <locale>
#include <codecvt>
#include <type_traits>

#ifdef _WIN32
#include <windows.h>
#include <fcntl.h>
#include <io.h>
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
template<typename CharT = char>
inline std::basic_ostream<CharT>& write_utf8(std::basic_ostream<CharT>& os, const std::string& utf8_str) {
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
template<typename CharT = char>
inline std::string read_utf8_line(std::basic_istream<CharT>& is) {
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
    explicit utf8_ifstream(const std::string& filename) 
        : std::ifstream(filename, std::ios::binary) {
        // Configurar para UTF-8
        this->imbue(std::locale());
    }
    
    explicit utf8_ifstream(const char* filename) 
        : utf8_ifstream(std::string(filename)) {}
};

/**
 * @brief Escribir archivo con soporte UTF-8
 */
class utf8_ofstream : public std::ofstream {
public:
    explicit utf8_ofstream(const std::string& filename) 
        : std::ofstream(filename, std::ios::binary) {
        // Configurar para UTF-8
        this->imbue(std::locale());
    }
    
    explicit utf8_ofstream(const char* filename) 
        : utf8_ofstream(std::string(filename)) {}
};

/**
 * @brief Operadores de conveniencia para UTF-8
 */
struct utf8_output_wrapper {
    std::ostream& os;
    explicit utf8_output_wrapper(std::ostream& stream) : os(stream) {}
};

inline utf8_output_wrapper utf8(std::ostream& os) {
    return utf8_output_wrapper(os);
}

template<typename T>
inline std::ostream& operator<<(utf8_output_wrapper wrapper, const T& value) {
    return wrapper.os << value;
}

// Especialización para strings UTF-8
inline std::ostream& operator<<(utf8_output_wrapper wrapper, const std::string& utf8_str) {
    return write_utf8(wrapper.os, utf8_str);
}

/**
 * @brief Utilidades para validación UTF-8
 */
inline bool is_valid_utf8(const std::string& str) {
    const unsigned char* bytes = reinterpret_cast<const unsigned char*>(str.c_str());
    size_t len = str.length();
    
    for (size_t i = 0; i < len; ++i) {
        if (bytes[i] <= 0x7F) {
            // ASCII
            continue;
        } else if ((bytes[i] >> 5) == 0x06) {
            // 110xxxxx - 2 bytes
            if (++i >= len || (bytes[i] >> 6) != 0x02) return false;
        } else if ((bytes[i] >> 4) == 0x0E) {
            // 1110xxxx - 3 bytes
            if (++i >= len || (bytes[i] >> 6) != 0x02) return false;
            if (++i >= len || (bytes[i] >> 6) != 0x02) return false;
        } else if ((bytes[i] >> 3) == 0x1E) {
            // 11110xxx - 4 bytes
            if (++i >= len || (bytes[i] >> 6) != 0x02) return false;
            if (++i >= len || (bytes[i] >> 6) != 0x02) return false;
            if (++i >= len || (bytes[i] >> 6) != 0x02) return false;
        } else {
            return false;
        }
    }
    return true;
}

/**
 * @brief Sanitizar string para ASCII seguro
 */
inline std::string sanitize_to_ascii(const std::string& input) {
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
EOF

echo "  -> Creado include/utf8_io.hpp con soporte UTF-8 completo"

# Crear un archivo de prueba para UTF-8
cat > test_utf8_io.cpp << 'EOF'
#include "include/utf8_io.hpp"
#include <iostream>
#include <string>

int main() {
    // Inicializar soporte UTF-8
    utf8_io::initialize_utf8_support();
    
    std::cout << "=== Prueba de UTF-8 IO ===" << std::endl;
    
    // Pruebas básicas
    std::string test_ascii = "Hello, World! Basic ASCII test.";
    std::string test_mixed = "Mixed content: ASCII + symbols [OK] -> safe";
    
    std::cout << "ASCII test: " << test_ascii << std::endl;
    std::cout << "Mixed test: " << test_mixed << std::endl;
    
    // Validación UTF-8
    std::cout << "ASCII is valid UTF-8: " << (utf8_io::is_valid_utf8(test_ascii) ? "true" : "false") << std::endl;
    std::cout << "Mixed is valid UTF-8: " << (utf8_io::is_valid_utf8(test_mixed) ? "true" : "false") << std::endl;
    
    // Prueba de sanitización
    std::string potentially_problematic = "Test with potential issues";
    std::string sanitized = utf8_io::sanitize_to_ascii(potentially_problematic);
    std::cout << "Sanitized: " << sanitized << std::endl;
    
    // Prueba de escritura de archivo UTF-8
    {
        utf8_io::utf8_ofstream file("test_utf8_output.txt");
        file << "UTF-8 file test" << std::endl;
        file << "Line 2 with safe characters: [OK] -> success" << std::endl;
        file << "Mathematical symbols converted: PI = " << "PI" << std::endl;
    }
    
    // Prueba de lectura de archivo UTF-8
    {
        utf8_io::utf8_ifstream file("test_utf8_output.txt");
        std::string line;
        int line_num = 1;
        while (std::getline(file, line)) {
            std::cout << "File line " << line_num++ << ": " << line << std::endl;
        }
    }
    
    std::cout << "=== UTF-8 IO test completed ===" << std::endl;
    return 0;
}
EOF

echo "  -> Creado test_utf8_io.cpp para probar el soporte UTF-8"

echo ""
echo "=== Actualizando archivos principales para usar UTF-8 ==="

# Actualizar el archivo int128_io.hpp para incluir UTF-8
if [[ -f "include/int128_io.hpp" ]]; then
    echo "  -> Actualizando include/int128_io.hpp con soporte UTF-8"
    
    # Añadir include de UTF-8 al principio
    sed -i '1i #include "utf8_io.hpp"' include/int128_io.hpp
    
    # Añadir función de inicialización UTF-8 en el namespace
    sed -i '/namespace int128_io {/a\\n// Initialize UTF-8 support automatically\nstatic bool utf8_initialized = []() {\n    utf8_io::initialize_utf8_support();\n    return true;\n}();' include/int128_io.hpp
fi

echo ""
echo "=== Resumen de cambios realizados ==="
echo "1. Corregidos caracteres problemáticos en todos los archivos .cpp y .hpp"
echo "2. Creado include/utf8_io.hpp con soporte completo para UTF-8"
echo "3. Creado test_utf8_io.cpp para probar la funcionalidad"
echo "4. Actualizado int128_io.hpp para incluir soporte UTF-8 automático"
echo ""
echo "Para compilar y probar:"
echo "  g++ -std=c++17 test_utf8_io.cpp -o test_utf8_io"
echo "  ./test_utf8_io"
echo ""
echo "Los archivos originales se guardaron con extensión .backup"