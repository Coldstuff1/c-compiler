#include <string>
namespace lexer {

struct Position {
  int x;
  int y;
};

enum class TokenClass {
  IDENTIFIER,
  ASSIGN,
  LBRA,
  RBRA,
  LPAR,
  RPAR,
  LSBR,
  RSBR,
  SC,
  COMMA,
  INT,
  VOID,
  CHAR,
  IF,
  ELSE,
  WHILE,
  RETURN,
  STRUCT,
  SIZEOF,
  INCLUDE,
  STRING_LITERAL,
  INT_LITERAL,
  CHAR_LITERAL,
  LOGAND,
  LOGOR,
  EQ,
  NE,
  LT,
  GT,
  LE,
  GE,
  PLUS,
  MINUS,
  ASTERIX,
  DIV,
  REM,
  AND,
  DOT,
  END, // EOF
  INVALID
};

class Token {
public:
  const TokenClass type;
  const std::string str;
  const Position position;

  Token(TokenClass type, std::string str, int lineNum, int colNum)
      : type(type), str(str), position({lineNum, colNum}) {}

  std::string toString();
};

} // namespace lexer
