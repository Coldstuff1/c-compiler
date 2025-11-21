#ifndef SCANNER_H
#define SCANNER_H

#include <fstream>
#include <vector>

namespace lexer {
class Scanner {
private:
  std::vector<char> buffer;
  char peeked{-1};
  int line{1};
  int column{1};
  long unsigned int pos{0};

public:
  Scanner(std::ifstream &file) {
    std::istreambuf_iterator<char> begin(file), end;
    buffer = std::vector<char>(begin, end);
  }

  int getColumn();
  int getLine();
  char peek();
  char next();
  bool hasNext();
};
} // namespace lexer
#endif
