/**
 * @file number_type_traits.hpp
 * @brief Traits y concepts avanzados para tipos enteros con soporte para
 * precisión arbitraria
 * @author AlgoritmiaCombinatoria Project
 * @date 2024
 * @version 1.0
 *
 * Este archivo define concepts de C++20 para categorizar diferentes tipos de
 * enteros, incluyendo tipos estándar, extensiones de compilador, y tipos de
 * precisión arbitraria de Boost.Multiprecision.
 *
 * @section features Características Principales
 * - Soporte para enteros estándar de C++ (int, long, etc.)
 * - Soporte para extensiones de compilador (__int128)
 * - Soporte completo para Boost.Multiprecision (GMP, cpp_int, tom_int)
 * - Concepts para clasificar tipos por tamaño (finito vs. arbitrario)
 * - Concepts para clasificar tipos por signo (signed vs. unsigned)
 *
 * @section usage Uso Básico
 * @code
 * using namespace integral_type_traits;
 *
 * template<integral T>
 * void process_integer(T value) {
 *     if constexpr (infty_integral<T>) {
 *         // Lógica para enteros de precisión arbitraria
 *     } else {
 *         // Lógica para enteros de tamaño fijo
 *     }
 * }
 * @endcode
 */

#pragma once
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/gmp.hpp>
#include <boost/multiprecision/tommath.hpp>
#include <type_traits>

/**
 * @namespace integral_type_traits_impl
 * @brief Namespace interno con implementaciones de concepts para tipos enteros
 *
 * Este namespace contiene la implementación interna de los concepts que
 * clasifican diferentes categorías de tipos enteros. No debe usarse
 * directamente; en su lugar use el namespace integral_type_traits.
 */
namespace integral_type_traits_impl {

//==============================================================================
// CONCEPTS PARA TIPOS ENTEROS ESTÁNDAR DE C++
//==============================================================================

/**
 * @brief Concept que identifica tipos enteros estándar de C++
 * @tparam T Tipo a evaluar
 *
 * Verdadero para tipos como int, long, char, bool, etc.
 * Utiliza std::is_integral_v internamente.
 *
 * @note Incluye char y bool que técnicamente son enteros en C++
 */
template <typename T>
concept std_integral = std::is_integral_v<T>;

/**
 * @brief Concept para tipos enteros estándar con signo
 * @tparam T Tipo a evaluar
 *
 * Verdadero para tipos como int, long, short, signed char, etc.
 * Combina std_integral con std::is_signed_v.
 */
template <typename T>
concept std_signed_integral = std_integral<T> && std::is_signed_v<T>;

/**
 * @brief Concept para tipos enteros estándar sin signo
 * @tparam T Tipo a evaluar
 *
 * Verdadero para tipos como unsigned int, size_t, uint32_t, etc.
 * Combina std_integral con std::is_unsigned_v.
 */
template <typename T>
concept std_unsigned_integral = std_integral<T> && std::is_unsigned_v<T>;

/**
 * @brief Concept para tipos enteros estándar de tamaño finito/fijo
 * @tparam T Tipo a evaluar
 *
 * Verdadero para todos los tipos enteros estándar ya que tienen tamaño fijo.
 * Limita a tipos no mayores que long long para compatibilidad.
 *
 * @note En la práctica, todos los tipos std_integral son finite_integral
 */
template <typename T>
concept std_finite_integral =
    std_integral<T> && (sizeof(T) <= sizeof(long long));

/**
 * @brief Concept para tipos enteros estándar de precisión arbitraria
 * @tparam T Tipo a evaluar
 *
 * Siempre falso - ningún tipo estándar de C++ tiene precisión arbitraria.
 * Existe por completitud y consistencia del sistema de concepts.
 */
template <typename T>
concept std_infty_integral = false;

//==============================================================================
// CONCEPTS PARA TIPOS ENTEROS NO ESTÁNDAR (EXTENSIONES DE COMPILADOR)
//==============================================================================

/**
 * @brief Concept para enteros extendidos no estándar del compilador
 * @tparam T Tipo a evaluar
 *
 * Verdadero para tipos como __int128 y unsigned __int128 que son extensiones
 * específicas de GCC y Clang, no parte del estándar C++.
 *
 * @warning Estos tipos no están disponibles en todos los compiladores
 * @note MSVC no soporta __int128, usa __int64 como tipo más grande
 */
template <typename T>
concept nonstd_integral =
#ifdef __SIZEOF_INT128__
    std::is_same_v<T, __int128> || std::is_same_v<T, unsigned __int128>;
#else
    false; // No hay tipos no-estándar disponibles en este compilador
#endif

/**
 * @brief Concept para enteros no estándar con signo
 * @tparam T Tipo a evaluar
 *
 * Verdadero únicamente para __int128 (128 bits con signo).
 */
template <typename T>
concept nonstd_signed_integral =
#ifdef __SIZEOF_INT128__
    std::is_same_v<T, __int128>;
#else
    false; // No hay tipos no-estándar disponibles en este compilador
#endif

/**
 * @brief Concept para enteros no estándar sin signo
 * @tparam T Tipo a evaluar
 *
 * Verdadero únicamente para unsigned __int128 (128 bits sin signo).
 */
template <typename T>
concept nonstd_unsigned_integral =
#ifdef __SIZEOF_INT128__
    std::is_same_v<T, unsigned __int128>;
#else
    false; // No hay tipos no-estándar disponibles en este compilador
#endif

/**
 * @brief Concept para enteros no estándar de tamaño finito
 * @tparam T Tipo a evaluar
 *
 * Verdadero para todos los tipos nonstd_integral ya que __int128 tiene tamaño
 * fijo. Aunque es muy grande (128 bits), sigue siendo de tamaño finito y
 * determinado.
 */
template <typename T>
concept nonstd_finite_integral = nonstd_integral<T>;

/**
 * @brief Concept para enteros no estándar de precisión arbitraria
 * @tparam T Tipo a evaluar
 *
 * Siempre falso - los tipos __int128 tienen tamaño fijo, no arbitrario.
 * Existe por completitud del sistema de concepts.
 */
template <typename T>
concept nonstd_infty_integral = false;

// Conceptos para enteros no estándar
// propios de la librería Boost.Multiprecision
//==============================================================================
// ALIASES PARA TIPOS BOOST.MULTIPRECISION
//==============================================================================

/**
 * @brief Namespace alias para abreviar Boost.Multiprecision
 *
 * Facilita el acceso a los tipos de Boost::multiprecision sin repetir
 * todo el namespace completo.
 */
namespace mp =
    boost::multiprecision; /**
                            * @brief Alias legible para enteros de precisión
                            * arbitraria con backend C++
                            *
                            * cpp_int: Implementación pura C++ de enteros de
                            * precisión arbitraria. Es portable pero puede ser
                            * más lenta que GMP/tommath.
                            */
using cpp_int_t = mp::cpp_int;

/**
 * @brief Alias legible para enteros de precisión arbitraria con backend tommath
 *
 * tom_int: Usa la biblioteca libtommath como backend. Más rápida que cpp_int
 * pero requiere enlazar con la biblioteca externa.
 */
using tom_int_t = mp::tom_int;

/**
 * @brief Alias legible para enteros de precisión arbitraria con backend GMP
 *
 * mpz_int: Usa la biblioteca GMP (GNU Multiple Precision). La más rápida
 * para operaciones complejas pero requiere enlazar con GMP.
 */
using mpz_int_t = mp::mpz_int;

//==============================================================================
// CONCEPTS PARA TIPOS BOOST.MULTIPRECISION
//==============================================================================

/**
 * @brief Concept para todos los tipos enteros de Boost.Multiprecision
 * @tparam T Tipo a evaluar
 *
 * Verdadero para todos los tipos enteros de Boost::multiprecision incluyendo:
 * - Tipos de tamaño fijo: int128_t, int256_t, int512_t, int1024_t y sus
 * versiones unsigned
 * - Tipos de precisión arbitraria: cpp_int, tom_int, mpz_int
 *
 * @note Los tipos de precisión arbitraria pueden crecer dinámicamente según sea
 * necesario
 *
 * @code
 * static_assert(boost_integral<boost::multiprecision::cpp_int>);
 * static_assert(boost_integral<boost::multiprecision::int1024_t>);
 * static_assert(!boost_integral<int>); // int estándar no es de Boost
 * @endcode
 */
template <typename T>
concept boost_integral =
    std::is_same_v<T, mp::int1024_t> || std::is_same_v<T, mp::uint1024_t> ||
    std::is_same_v<T, mp::int512_t> || std::is_same_v<T, mp::uint512_t> ||
    std::is_same_v<T, mp::int256_t> || std::is_same_v<T, mp::uint256_t> ||
    std::is_same_v<T, mp::int128_t> || std::is_same_v<T, mp::uint128_t> ||
    std::is_same_v<T, mp::mpz_int> || std::is_same_v<T, mp::cpp_int> ||
    std::is_same_v<T, mp::tom_int>;

/**
 * @brief Concept para tipos enteros con signo de Boost.Multiprecision
 * @tparam T Tipo a evaluar
 *
 * Incluye tanto tipos de tamaño fijo como de precisión arbitraria con signo.
 * Los tipos de precisión arbitraria pueden representar números negativos
 * de cualquier magnitud.
 */
template <typename T>
concept boost_signed_integral =
    std::is_same_v<T, mp::int1024_t> || std::is_same_v<T, mp::int512_t> ||
    std::is_same_v<T, mp::int256_t> || std::is_same_v<T, mp::int128_t> ||
    std::is_same_v<T, mp::mpz_int> || std::is_same_v<T, mp::cpp_int> ||
    std::is_same_v<T, mp::tom_int>;

/**
 * @brief Concept para tipos enteros sin signo de Boost.Multiprecision
 * @tparam T Tipo a evaluar
 *
 * Solo incluye tipos de tamaño fijo sin signo. Los tipos de precisión
 * arbitraria cpp_int, tom_int y mpz_int son siempre con signo.
 *
 * @note Implementado como boost_integral && !boost_signed_integral para
 * mantenerlo DRY
 */
template <typename T>
concept boost_unsigned_integral =
    boost_integral<T> && !boost_signed_integral<T>;

/**
 * @brief Concept para tipos Boost de tamaño fijo (no arbitrario)
 * @tparam T Tipo a evaluar
 *
 * Verdadero para tipos con número fijo de bits (128, 256, 512, 1024).
 * Estos tipos tienen límites máximos definidos y no pueden crecer
 * dinámicamente.
 *
 * @note Implementado excluyendo los tipos de precisión arbitraria
 */
template <typename T>
concept boost_finite_integral =
    boost_integral<T> && !std::is_same_v<T, mp::mpz_int> &&
    !std::is_same_v<T, mp::cpp_int> && !std::is_same_v<T, mp::tom_int>;

/**
 * @brief Concept para tipos Boost de precisión arbitraria (infinita)
 * @tparam T Tipo a evaluar
 *
 * Verdadero para tipos que pueden representar enteros de cualquier tamaño
 * limitado solo por la memoria disponible. Estos son los más adecuados
 * para cálculos combinatorios que producen números muy grandes.
 *
 * @note cpp_int es el más portable, mpz_int el más rápido (requiere GMP)
 *
 * @code
 * // Ejemplo de uso con factorial de números muy grandes
 * template<boost_infty_integral T>
 * T big_factorial(int n) {
 *     T result = 1;
 *     for(int i = 2; i <= n; ++i) {
 *         result *= i;
 *     }
 *     return result;
 * }
 * @endcode
 */
template <typename T>
concept boost_infty_integral =
    std::is_same_v<T, mp::mpz_int> || std::is_same_v<T, mp::cpp_int> ||
    std::is_same_v<T, mp::tom_int>;

} // namespace integral_type_traits_impl

//==============================================================================
// INTERFAZ PÚBLICA - NAMESPACE INTEGRAL_TYPE_TRAITS
//==============================================================================

/**
 * @brief Namespace público que expone los concepts de clasificación de tipos
 * enteros
 *
 * Este namespace proporciona una interfaz unificada para trabajar con
 * diferentes tipos enteros: estándar (int, long, etc.), extensiones de
 * compilador (__int128) y tipos de Boost.Multiprecision.
 *
 * @section concepts_usage Uso de los Concepts
 *
 * Los concepts se pueden usar en templates para restringir tipos:
 * @code
 * template<integral_type_traits::integral T>
 * T my_function(T value) {
 *     // Función que acepta cualquier tipo entero
 *     return value * 2;
 * }
 *
 * template<integral_type_traits::infty_integral T>
 * T big_calculation(int n) {
 *     // Solo acepta tipos de precisión arbitraria
 *     T result = 1;
 *     for(int i = 1; i <= n; ++i) {
 *         result *= i;
 *     }
 *     return result;
 * }
 * @endcode
 */
namespace integral_type_traits {

using namespace integral_type_traits_impl;

/**
 * @brief Concept unificado para cualquier tipo entero soportado
 * @tparam T Tipo a evaluar
 *
 * Verdadero para cualquier tipo entero: estándar C++, extensiones de compilador
 * o tipos de Boost.Multiprecision.
 *
 * @note Este es el concept más general y útil para templates que deben
 *       aceptar cualquier tipo de entero
 */
template <typename T>
concept integral = std_integral<T> || nonstd_integral<T> || boost_integral<T>;

/**
 * @brief Concept unificado para tipos enteros con signo
 * @tparam T Tipo a evaluar
 *
 * Verdadero para tipos enteros que pueden representar números negativos.
 */
template <typename T>
concept signed_integral = std_signed_integral<T> || nonstd_signed_integral<T> ||
                          boost_signed_integral<T>;

/**
 * @brief Concept unificado para tipos enteros sin signo
 * @tparam T Tipo a evaluar
 *
 * Verdadero para tipos enteros que solo representan números no negativos.
 */
template <typename T>
concept unsigned_integral =
    std_unsigned_integral<T> || nonstd_unsigned_integral<T> ||
    boost_unsigned_integral<T>;

/**
 * @brief Concept unificado para tipos enteros de tamaño fijo
 * @tparam T Tipo a evaluar
 *
 * Verdadero para tipos con límites máximos fijos (no pueden crecer
 * arbitrariamente). Útil cuando se necesita garantizar límites de memoria
 * predecibles.
 */
template <typename T>
concept finite_integral = std_finite_integral<T> || nonstd_finite_integral<T> ||
                          boost_finite_integral<T>;

/**
 * @brief Concept unificado para tipos enteros de precisión arbitraria
 * @tparam T Tipo a evaluar
 *
 * Verdadero para tipos que pueden representar enteros de cualquier tamaño.
 * Estos son ideales para cálculos matemáticos que producen números muy grandes
 * como factoriales, combinaciones o números de Fibonacci grandes.
 *
 * @warning Solo tipos de Boost.Multiprecision soportan precisión arbitraria
 * @see boost_infty_integral Para los tipos específicos disponibles
 */
template <typename T>
concept infty_integral = std_infty_integral<T> || nonstd_infty_integral<T> ||
                         boost_infty_integral<T>;

} // namespace integral_type_traits

//==============================================================================
// FIN DEL ARCHIVO NUMBER_TYPE_TRAITS.HPP
//==============================================================================
