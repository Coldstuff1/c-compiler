#include "scanner.hpp"
#include "token.hpp"
#include <iostream>
#include <string_view>
namespace lexer {
class Tokeniser {
private:
  Scanner scanner;
  int errors{0};

  void error(std::string_view msg) {
    errors++;
    std::cout << msg << std::endl;
  }

public:
  Tokeniser(Scanner scanner) : scanner(scanner) {}

  Token nextToken();
};
} // namespace lexer
