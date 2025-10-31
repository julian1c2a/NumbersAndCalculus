#include <iostream>
#include <optional>

int main() {
  std::cout << "Testing std::optional in C++17..." << std::endl;

  std::optional<int> value = 42;
  if (value.has_value()) {
    std::cout << "Value: " << value.value() << std::endl;
  }

  std::optional<int> empty;
  if (!empty.has_value()) {
    std::cout << "Empty optional handled correctly" << std::endl;
  }

  std::cout << "std::optional works!" << std::endl;
  return 0;
}