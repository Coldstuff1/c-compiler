#include "../lexer/scanner.hpp"
#include "../lexer/tokeniser.hpp"
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <ostream>

enum class Mode {
  LEXER,
  PARSER,
};

void usage() {
  std::cout << std::format(
      "Usage: bazel run //main:c-compiler -- <mode> <inputfile>");
}

int main(int argc, char *argv[]) {

  if (argc != 3) {
    usage();
    return -1;
  }

  Mode mode;
  std::string_view pass{argv[1]};

  if (pass == "-lexer")
    mode = Mode::LEXER;
  else if (pass == "-parser")
    mode = Mode::PARSER;
  else {
    usage();
    return -1;
  }

  std::filesystem::path inputPath = std::filesystem::path(argv[2]);
  std::ifstream inputFile(inputPath);

  if (!inputFile.is_open()) {
    std::cout << "File not found!" << std::endl;
    return -1;
  }

  lexer::Scanner scanner{inputFile};

  lexer::Tokeniser tokeniser{scanner};

  if (mode == Mode::LEXER) {
    bool eof = false;
    while (!eof) {
      lexer::Token token{tokeniser.nextToken()};
      if (token.type == lexer::TokenClass::END)
        eof = true;
      std::cout << token.toString() << std::endl;
    }
    if (tokeniser.getErrorCount() == 0)
      std::cout << "Lexing: pass" << std::endl;
    else
      std::cout << std::format("Lexing: failed ({} errors)",
                               tokeniser.getErrorCount());
    return tokeniser.getErrorCount() == 0 ? 0 : -1;
  }

  return 0;
}
