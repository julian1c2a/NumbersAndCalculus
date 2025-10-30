#include "combinatorics.hpp"
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <vector>

TEST_CASE("Factorial calculations", "[factorial]") {
  SECTION("Basic factorial calculations") {
    REQUIRE(algo_comb::factorial(0) == 1);
    REQUIRE(algo_comb::factorial(1) == 1);
    REQUIRE(algo_comb::factorial(2) == 2);
    REQUIRE(algo_comb::factorial(3) == 6);
    REQUIRE(algo_comb::factorial(4) == 24);
    REQUIRE(algo_comb::factorial(5) == 120);
  }

  SECTION("Large factorial with BigInt") {
    auto result = algo_comb::factorial(algo_comb::BigInt(10));
    REQUIRE(result == 3628800);
  }

  SECTION("Negative input throws exception") {
    REQUIRE_THROWS_AS(algo_comb::factorial(-1), std::invalid_argument);
  }
}

TEST_CASE("Combination calculations", "[combinations]") {
  SECTION("Basic combination calculations") {
    REQUIRE(algo_comb::combinations(5, 0) == 1);
    REQUIRE(algo_comb::combinations(5, 1) == 5);
    REQUIRE(algo_comb::combinations(5, 2) == 10);
    REQUIRE(algo_comb::combinations(5, 3) == 10);
    REQUIRE(algo_comb::combinations(5, 4) == 5);
    REQUIRE(algo_comb::combinations(5, 5) == 1);
  }

  SECTION("Edge cases") {
    REQUIRE(algo_comb::combinations(10, 0) == 1);
    REQUIRE(algo_comb::combinations(10, 10) == 1);
    REQUIRE(algo_comb::combinations(5, 6) == 0);  // k > n
    REQUIRE(algo_comb::combinations(-1, 2) == 0); // negative n
  }

  SECTION("Large combinations") {
    auto result = algo_comb::combinations(20, 10);
    REQUIRE(result == 184756);
  }
}

TEST_CASE("Permutation calculations", "[permutations]") {
  SECTION("Basic permutation calculations") {
    REQUIRE(algo_comb::permutations(5, 0) == 1);
    REQUIRE(algo_comb::permutations(5, 1) == 5);
    REQUIRE(algo_comb::permutations(5, 2) == 20);
    REQUIRE(algo_comb::permutations(5, 3) == 60);
    REQUIRE(algo_comb::permutations(5, 5) == 120);
  }

  SECTION("Edge cases") {
    REQUIRE(algo_comb::permutations(5, 6) == 0);  // k > n
    REQUIRE(algo_comb::permutations(-1, 2) == 0); // negative n
  }
}

TEST_CASE("Fibonacci sequence", "[fibonacci]") {
  SECTION("Basic Fibonacci numbers") {
    REQUIRE(algo_comb::fibonacci(0) == 0);
    REQUIRE(algo_comb::fibonacci(1) == 1);
    REQUIRE(algo_comb::fibonacci(2) == 1);
    REQUIRE(algo_comb::fibonacci(3) == 2);
    REQUIRE(algo_comb::fibonacci(4) == 3);
    REQUIRE(algo_comb::fibonacci(5) == 5);
    REQUIRE(algo_comb::fibonacci(6) == 8);
    REQUIRE(algo_comb::fibonacci(7) == 13);
  }

  SECTION("Large Fibonacci numbers") {
    auto result = algo_comb::fibonacci(20);
    REQUIRE(result == 6765);
  }

  SECTION("Negative input throws exception") {
    REQUIRE_THROWS_AS(algo_comb::fibonacci(-1), std::invalid_argument);
  }
}

TEST_CASE("Catalan numbers", "[catalan]") {
  SECTION("Basic Catalan numbers") {
    REQUIRE(algo_comb::catalan(0) == 1);
    REQUIRE(algo_comb::catalan(1) == 1);
    REQUIRE(algo_comb::catalan(2) == 2);
    REQUIRE(algo_comb::catalan(3) == 5);
    REQUIRE(algo_comb::catalan(4) == 14);
    REQUIRE(algo_comb::catalan(5) == 42);
  }

  SECTION("Negative input throws exception") {
    REQUIRE_THROWS_AS(algo_comb::catalan(-1), std::invalid_argument);
  }
}

TEST_CASE("Generate permutations", "[generate_permutations]") {
  SECTION("Small vector permutations") {
    std::vector<int> input = {1, 2, 3};
    auto result = algo_comb::generate_permutations(input);

    REQUIRE(result.size() == 6); // 3! = 6

    // Verificar que todas las permutaciones están presentes
    std::vector<std::vector<int>> expected = {{1, 2, 3}, {1, 3, 2}, {2, 1, 3},
                                              {2, 3, 1}, {3, 1, 2}, {3, 2, 1}};

    for (const auto &expected_perm : expected) {
      REQUIRE(std::find(result.begin(), result.end(), expected_perm) !=
              result.end());
    }
  }

  SECTION("Empty vector") {
    std::vector<int> input = {};
    auto result = algo_comb::generate_permutations(input);
    REQUIRE(result.size() == 1); // Una permutación vacía
    REQUIRE(result[0].empty());
  }
}

TEST_CASE("Generate combinations", "[generate_combinations]") {
  SECTION("Basic combinations") {
    std::vector<int> input = {1, 2, 3, 4};
    auto result = algo_comb::generate_combinations(input, 2);

    REQUIRE(result.size() == 6); // C(4,2) = 6
  }

  SECTION("k = 0") {
    std::vector<int> input = {1, 2, 3};
    auto result = algo_comb::generate_combinations(input, 0);
    REQUIRE(result.empty());
  }

  SECTION("k > n") {
    std::vector<int> input = {1, 2};
    auto result = algo_comb::generate_combinations(input, 5);
    REQUIRE(result.empty());
  }
}

TEST_CASE("Rational binomial coefficients", "[rational]") {
  SECTION("Basic rational coefficients") {
    auto result = algo_comb::binomial_coefficient_rational(5, 2);
    REQUIRE(result.numerator() == 10);
    REQUIRE(result.denominator() == 1);
  }

  SECTION("Edge cases") {
    auto result_zero = algo_comb::binomial_coefficient_rational(5, 6);
    REQUIRE(result_zero.numerator() == 0);

    auto result_one = algo_comb::binomial_coefficient_rational(5, 0);
    REQUIRE(result_one.numerator() == 1);
    REQUIRE(result_one.denominator() == 1);
  }
}

TEST_CASE("BigInt operations", "[bigint]") {
  SECTION("Large factorial calculation") {
    auto result = algo_comb::factorial(algo_comb::BigInt(50));
    // 50! es un número muy grande, verificamos que no es cero
    REQUIRE(result > 0);
  }

  SECTION("BigInt combinations") {
    auto result =
        algo_comb::combinations(algo_comb::BigInt(100), algo_comb::BigInt(50));
    REQUIRE(result > 0);
  }
}