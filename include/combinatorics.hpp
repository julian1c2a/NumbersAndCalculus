#ifndef COMBINATORICS_HPP
#define COMBINATORICS_HPP

#include "number_type_traits.hpp"
#include "numeric_limits.hpp"
#include <boost/multiprecision/cpp_dec_float.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/rational.hpp>
#include <concepts>
#include <vector>

namespace algo_comb {

// Alias para tipos de multiprecisión
using BigInt = boost::multiprecision::cpp_int;
using BigFloat = boost::multiprecision::cpp_dec_float_100;
using Rational = boost::rational<BigInt>;

// Concepto para tipos enteros
template <typename T>
concept IntegerType = std::integral<T> || std::same_as<T, BigInt>;

// Concepto para tipos numéricos
template <typename T>
concept NumericType =
    std::integral<T> || std::floating_point<T> || std::same_as<T, BigInt> ||
    std::same_as<T, BigFloat> || std::same_as<T, Rational>;

/**
 * @brief Calcula el factorial de n
 * @param n Número para calcular el factorial
 * @return El factorial de n
 */
template <IntegerType T> BigInt factorial(const T &n);

/**
 * @brief Calcula las combinaciones C(n, k) = n! / (k! * (n-k)!)
 * @param n Número total de elementos
 * @param k Número de elementos a seleccionar
 * @return Número de combinaciones
 */
template <IntegerType T> BigInt combinations(const T &n, const T &k);

/**
 * @brief Calcula las permutaciones P(n, k) = n! / (n-k)!
 * @param n Número total de elementos
 * @param k Número de elementos a permutar
 * @return Número de permutaciones
 */
template <IntegerType T> BigInt permutations(const T &n, const T &k);

/**
 * @brief Calcula los números de Fibonacci
 * @param n Posición en la secuencia
 * @return El n-ésimo número de Fibonacci
 */
template <IntegerType T> BigInt fibonacci(const T &n);

/**
 * @brief Calcula los números de Catalan
 * @param n Posición en la secuencia
 * @return El n-ésimo número de Catalan
 */
template <IntegerType T> BigInt catalan(const T &n);

/**
 * @brief Genera todas las permutaciones de un vector
 * @param elements Vector de elementos
 * @return Vector con todas las permutaciones
 */
template <typename T>
std::vector<std::vector<T>>
generate_permutations(const std::vector<T> &elements);

/**
 * @brief Genera todas las combinaciones de k elementos de un vector
 * @param elements Vector de elementos
 * @param k Número de elementos por combinación
 * @return Vector con todas las combinaciones
 */
template <typename T>
std::vector<std::vector<T>>
generate_combinations(const std::vector<T> &elements, size_t k);

/**
 * @brief Calcula el coeficiente binomial usando racionales para mayor precisión
 * @param n Número total
 * @param k Número a seleccionar
 * @return Coeficiente binomial como número racional
 */
template <IntegerType T>
Rational binomial_coefficient_rational(const T &n, const T &k);

/**
 * @brief Calcula la función gamma usando multiprecisión
 * @param x Valor de entrada
 * @return Valor de la función gamma
 */
BigFloat gamma_function(const BigFloat &x);

} // namespace algo_comb

// Incluir implementaciones de templates
#include "combinatorics_impl.hpp"

#endif // COMBINATORICS_HPP