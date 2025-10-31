#include <fstream>
#include <iostream>
#include <map>
#include <string>


int main() {
  // Map of problematic characters to ASCII replacements
  std::map<std::string, std::string> replacements = {
      {"├│", "o"},
      {"├¡", "i"},
      {"├®", "e"},
      {"├▒", "n"},
      {"Tama├▒os", "Tamanos"},
      {"Alineaci├│n", "Alineacion"},
      {"PREDICCI├ôN", "PREDICCION"},
      {"├¡ndices", "indices"},
      {"An├ílisis", "Analisis"},
      {"operaci├│n", "operacion"},
      {"comparaci├│n", "comparacion"}};

  std::string filename = "comprehensive_benchmarks.cpp";
  std::ifstream infile(filename);
  std::string content((std::istreambuf_iterator<char>(infile)),
                      std::istreambuf_iterator<char>());
  infile.close();

  // Apply replacements
  for (const auto &pair : replacements) {
    std::string::size_type pos = 0;
    while ((pos = content.find(pair.first, pos)) != std::string::npos) {
      content.replace(pos, pair.first.length(), pair.second);
      pos += pair.second.length();
    }
  }

  std::ofstream outfile(filename);
  outfile << content;
  outfile.close();

  std::cout << "UTF-8 corrections applied to " << filename << std::endl;
  return 0;
}