#include <iostream>
#include <optional>


// Test simple para verificar std::optional en C++17
std::optional<int> safe_divide(int a, int b) noexcept {
  if (b == 0) {
    return std::nullopt;
  }
  return a / b;
}

int main() {
  std::cout << "Testing std::optional in C++17...\n";

  auto result1 = safe_divide(10, 2);
  if (result1) {
    std::cout << "10 / 2 = " << *result1 << "\n";
  }

  auto result2 = safe_divide(10, 0);
  if (!result2) {
    std::cout << "Division by zero handled safely\n";
  }

  std::cout << "std::optional works correctly!\n";
  return 0;
}