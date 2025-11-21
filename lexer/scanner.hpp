#include <fstream>
#include <vector>

namespace lexer {
class Scanner {
private:
  std::vector<char> buffer;
  char peeked{-1};
  int line{1};
  int column{1};
  int pos{0};

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
