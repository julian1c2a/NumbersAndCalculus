#include "combinatorics.hpp"
#include <boost/math/special_functions/gamma.hpp>
#include <iostream>

namespace algo_comb {

BigFloat gamma_function(const BigFloat &x) {
  // Implementación básica usando la librería Boost Math
  // Para valores de entrada válidos
  if (x <= 0) {
    throw std::invalid_argument(
        "Gamma function is not defined for non-positive integers");
  }

  // Convertir a double para usar boost::math::tgamma, luego convertir de vuelta
  double x_double = static_cast<double>(x);
  double result_double = boost::math::tgamma(x_double);

  return BigFloat(result_double);
}

} // namespace algo_comb