#ifndef COMBINATORICS_IMPL_H
#define COMBINATORICS_IMPL_H

#include <algorithm>
#include <cmath>
#include <stdexcept>


namespace algo_comb {

template <IntegerType T> BigInt factorial(const T &n) {
  if (n < 0) {
    throw std::invalid_argument(
        "Factorial is not defined for negative numbers");
  }

  BigInt result = 1;
  BigInt num = static_cast<BigInt>(n);

  for (BigInt i = 2; i <= num; ++i) {
    result *= i;
  }

  return result;
}

template <IntegerType T> BigInt combinations(const T &n, const T &k) {
  BigInt big_n = static_cast<BigInt>(n);
  BigInt big_k = static_cast<BigInt>(k);

  if (big_k < 0 || big_n < 0 || big_k > big_n) {
    return 0;
  }

  if (big_k == 0 || big_k == big_n) {
    return 1;
  }

  // Optimización: C(n,k) = C(n,n-k)
  if (big_k > big_n - big_k) {
    big_k = big_n - big_k;
  }

  BigInt result = 1;
  for (BigInt i = 0; i < big_k; ++i) {
    result = result * (big_n - i) / (i + 1);
  }

  return result;
}

template <IntegerType T> BigInt permutations(const T &n, const T &k) {
  BigInt big_n = static_cast<BigInt>(n);
  BigInt big_k = static_cast<BigInt>(k);

  if (big_k < 0 || big_n < 0 || big_k > big_n) {
    return 0;
  }

  BigInt result = 1;
  for (BigInt i = big_n; i > big_n - big_k; --i) {
    result *= i;
  }

  return result;
}

template <IntegerType T> BigInt fibonacci(const T &n) {
  BigInt big_n = static_cast<BigInt>(n);

  if (big_n < 0) {
    throw std::invalid_argument(
        "Fibonacci is not defined for negative numbers");
  }

  if (big_n <= 1) {
    return big_n;
  }

  BigInt a = 0, b = 1;
  for (BigInt i = 2; i <= big_n; ++i) {
    BigInt temp = a + b;
    a = b;
    b = temp;
  }

  return b;
}

template <IntegerType T> BigInt catalan(const T &n) {
  BigInt big_n = static_cast<BigInt>(n);

  if (big_n < 0) {
    throw std::invalid_argument(
        "Catalan numbers are not defined for negative numbers");
  }

  if (big_n <= 1) {
    return 1;
  }

  // C_n = (2n)! / ((n+1)! * n!)
  // o usando la fórmula recursiva más eficiente
  BigInt two_n = 2 * big_n;
  return combinations(two_n, big_n) / (big_n + 1);
}

template <typename T>
std::vector<std::vector<T>>
generate_permutations(const std::vector<T> &elements) {
  std::vector<std::vector<T>> result;
  std::vector<T> perm = elements;

  std::sort(perm.begin(), perm.end());

  do {
    result.push_back(perm);
  } while (std::next_permutation(perm.begin(), perm.end()));

  return result;
}

template <typename T>
std::vector<std::vector<T>>
generate_combinations(const std::vector<T> &elements, size_t k) {
  std::vector<std::vector<T>> result;

  if (k > elements.size() || k == 0) {
    return result;
  }

  std::vector<bool> selector(elements.size());
  std::fill(selector.end() - k, selector.end(), true);

  do {
    std::vector<T> combination;
    for (size_t i = 0; i < elements.size(); ++i) {
      if (selector[i]) {
        combination.push_back(elements[i]);
      }
    }
    result.push_back(combination);
  } while (std::next_permutation(selector.begin(), selector.end()));

  return result;
}

template <IntegerType T>
Rational binomial_coefficient_rational(const T &n, const T &k) {
  BigInt big_n = static_cast<BigInt>(n);
  BigInt big_k = static_cast<BigInt>(k);

  if (big_k < 0 || big_n < 0 || big_k > big_n) {
    return Rational(0);
  }

  BigInt numerator = 1;
  BigInt denominator = 1;

  for (BigInt i = 0; i < big_k; ++i) {
    numerator *= (big_n - i);
    denominator *= (i + 1);
  }

  return Rational(numerator, denominator);
}

} // namespace algo_comb

#endif // COMBINATORICS_IMPL_H