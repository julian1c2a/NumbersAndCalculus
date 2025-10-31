// CÁLCULO: noexcept + CalculationResult
auto result = get_power_of_2_int8_safe(5);
if (result.is_valid()) {
  std::cout << "Resultado: " << result.get_value() << std::endl;
} else {
  std::cout << "Error: " << error_to_string(result.get_error()) << std::endl;
}

// I/O: excepciones
try {
  print_power_result_io(5);
} catch (const std::exception &e) {
  std::cout << "Error I/O: " << e.what() << std::endl;
}

// PURAS: noexcept
constexpr bool is_valid = is_valid_integral_type<int8_t>(); // noexcept