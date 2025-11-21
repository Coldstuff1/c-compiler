#include "tokeniser.hpp"
#include <cctype>
#include <format>
#include <string>

namespace lexer {

// keywords
constexpr std::string_view IF = "if";
constexpr std::string_view ELSE = "else";
constexpr std::string_view WHILE = "while";
constexpr std::string_view RETURN = "return";
constexpr std::string_view STRUCT = "struct";
constexpr std::string_view SIZEOF = "sizeof";
constexpr std::string_view INT = "int";
constexpr std::string_view VOID = "void";
constexpr std::string_view CHAR = "char";

// escape codes
constexpr char BACKSPACE = 'b';
constexpr char FORMFEED = 'f';
constexpr char NEWLINE = 'n';
constexpr char CARRIAGE_RETURN = 'r';
constexpr char HORIZONTAL_TAB = 't';
constexpr char BACKSLASH = '\\';
constexpr char SINGLE_QUOTE = '\'';
constexpr char NULL_TERMINATOR = '0';

// lexing business logic functions
static Token lexKeywordOrIdent(Scanner &scanner, std::string str, int line,
                               int column, ErrorFunction error);
static Token lexCharLiteral(Scanner &scanner, int line, int column,
                            ErrorFunction error);
static Token lexStringLiteral(Scanner &scanner, int line, int column,
                              ErrorFunction error);
static Token lexIntLiteral(Scanner &scanner, std::string str, int line,
                           int column, ErrorFunction error);
static Token lexInclude(Scanner &scanner, int line, int column,
                        ErrorFunction error);

// helper functions
static Token possibleKeywordToToken(const std::string_view str, const int line,
                                    const int column);
static bool isEscapeCharacter(const char curChar);
static char toEscapeCharacter(const char curChar);

int Tokeniser::getErrorCount() { return errors; }

Token Tokeniser::nextToken() {
  char nextChar;

  int line{scanner.getLine()};
  int column{scanner.getColumn()};

  ErrorFunction error_func{[this](std::string_view msg) { this->error(msg); }};

  nextChar = scanner.next();

  if (nextChar == -1)
    return Token{TokenClass::END, "EOF", line, column};

  if (std::isspace(nextChar))
    return nextToken();

  if (std::isalpha(nextChar) || nextChar == '_')
    return lexKeywordOrIdent(scanner, std::string{nextChar}, line, column,
                             error_func);

  if (nextChar == '\'')
    return lexCharLiteral(scanner, line, column, error_func);

  if (nextChar == '"')
    return lexStringLiteral(scanner, line, column, error_func);

  if (std::isdigit(nextChar))
    return lexIntLiteral(scanner, std::string{nextChar}, line, column,
                         error_func);

  if (nextChar == '#')
    return lexInclude(scanner, line, column, error_func);

  if (nextChar == '+')
    return Token{TokenClass::PLUS, std::string{nextChar}, line, column};

  if (nextChar == '-')
    return Token{TokenClass::MINUS, std::string{nextChar}, line, column};

  if (nextChar == '*')
    return Token{TokenClass::ASTERIX, std::string{nextChar}, line, column};

  if (nextChar == '%')
    return Token{TokenClass::REM, std::string{nextChar}, line, column};

  if (nextChar == '{')
    return Token{TokenClass::LBRA, std::string{nextChar}, line, column};

  if (nextChar == '}')
    return Token{TokenClass::RBRA, std::string{nextChar}, line, column};

  if (nextChar == '(')
    return Token{TokenClass::LPAR, std::string{nextChar}, line, column};

  if (nextChar == ')')
    return Token{TokenClass::RPAR, std::string{nextChar}, line, column};

  if (nextChar == '[')
    return Token{TokenClass::LSBR, std::string{nextChar}, line, column};

  if (nextChar == ']')
    return Token{TokenClass::RSBR, std::string{nextChar}, line, column};

  if (nextChar == ';')
    return Token{TokenClass::SC, std::string{nextChar}, line, column};

  if (nextChar == ',')
    return Token{TokenClass::COMMA, std::string{nextChar}, line, column};

  if (nextChar == '.')
    return Token{TokenClass::DOT, std::string{nextChar}, line, column};

  if (nextChar == '&') {
    if (scanner.peek() != '&')
      return Token{TokenClass::AND, std::string{nextChar}, line, column};
    scanner.next();
    return Token{TokenClass::LOGAND, std::string{nextChar, '&'}, line, column};
  }

  if (nextChar == '=') {
    if (scanner.peek() != '=')
      return Token{TokenClass::ASSIGN, std::string{nextChar}, line, column};
    scanner.next();
    return Token{TokenClass::EQ, std::string{nextChar, '='}, line, column};
  }

  if (nextChar == '|' && scanner.peek() == '|') {
    scanner.next();
    return Token{TokenClass::LOGOR, std::string{nextChar, '|'}, line, column};
  }

  if (nextChar == '!' && scanner.peek() == '=') {
    scanner.next();
    return Token{TokenClass::NE, std::string{nextChar, '='}, line, column};
  }

  if (nextChar == '<') {
    if (scanner.peek() != '=')
      return Token{TokenClass::LT, std::string{nextChar}, line, column};
    scanner.next();
    return Token{TokenClass::LE, std::string{nextChar, '='}, line, column};
  }

  if (nextChar == '>') {
    if (scanner.peek() != '=')
      return Token{TokenClass::GT, std::string{nextChar}, line, column};
    scanner.next();
    return Token{TokenClass::GE, std::string{nextChar, '='}, line, column};
  }

  if (nextChar == '/') {
    nextChar = scanner.peek();
    if (nextChar != '/' && nextChar != '*')
      return Token{TokenClass::GT, std::string{'/'}, line, column};

    char lastChar{nextChar};
    scanner.next();
    nextChar = scanner.peek();
    if (lastChar == '/') { // single line comment
      while (nextChar != '\n')
        nextChar = scanner.next();
      return nextToken();
    }

    bool keepSkipping = true;

    while (keepSkipping) { // multi line comment
      nextChar = scanner.next();
      if (nextChar == -1)
        keepSkipping = false;
      if (nextChar == '*' && scanner.peek() == '/') {
        scanner.next();
        keepSkipping = false;
      }
    }
    return nextToken();
  }

  error(std::format("Lexing error: unrecognised character ({}) at {}:{}!",
                    nextChar, line, column));
  return Token{TokenClass::INVALID, std::string{nextChar}, line, column};
}

static Token lexKeywordOrIdent(Scanner &scanner, std::string str, int line,
                               int column, ErrorFunction error) {
  char nextChar{scanner.peek()};

  if (nextChar == -1)
    return Token{TokenClass::END, "EOF", line, column};

  while (std::isalpha(nextChar)) {
    str += nextChar;
    scanner.next();
    nextChar = scanner.peek();

    if (nextChar == -1) {
      error(std::format(
          "Lexing error: file ending cutoff keyword or identifier at {}:{}!",
          line, column));
      return Token{TokenClass::INVALID, std::string{str}, line, column};
    }
    if (!std::isalnum(nextChar) && nextChar != '_') {
      return possibleKeywordToToken(str, line, column);
    }
  }

  while (std::isalnum(nextChar) || nextChar == '_') { // ident
    str += nextChar;
    scanner.next();
    nextChar = scanner.peek();

    if (nextChar == -1) {
      error(std::format("Lexing error: file ending cutoff identifier at {}:{}!",
                        line, column));
      return Token{TokenClass::INVALID, std::string{str}, line, column};
    }
  }
  Token foundToken{possibleKeywordToToken(str, line, column)};
  if (foundToken.type != TokenClass::IDENTIFIER) {
    error(std::format(
        "Lexing error: identifier at {}:{} cannot be reserved keyword!", line,
        column));
    return Token{TokenClass::INVALID, str, line, column};
  }
  return foundToken;
}

static Token lexCharLiteral(Scanner &scanner, int line, int column,
                            ErrorFunction error) {

  char nextChar{scanner.peek()};

  if (nextChar == -1)
    return Token{TokenClass::END, "EOF", line, column};

  std::string str;

  if (nextChar == '\\') { // escape char
    scanner.next();
    nextChar = scanner.peek();

    if (nextChar == -1) {
      error(std::format("Lexing error: char at {}:{} must be enclosed "
                        "between apostrophes!",
                        line, column));
      return Token{TokenClass::INVALID, str, line, column};
    }

    if (isEscapeCharacter(nextChar)) {
      str = std::string{toEscapeCharacter(nextChar)};
      scanner.next();
      nextChar = scanner.peek();

      if (nextChar != '\'') {
        error(std::format("Lexing error: char at {}:{} must be enclosed "
                          "between apostrophes!",
                          line, column));
        return Token{TokenClass::INVALID, str, line, column};
      }
      scanner.next();
      return Token{TokenClass::CHAR_LITERAL, str, line, column};
    }
  }

  str = std::string{nextChar};
  scanner.next();
  nextChar = scanner.peek();

  if (nextChar != '\'') {
    error(std::format("Lexing error: char at {}:{} must be enclosed "
                      "between apostrophes!",
                      line, column));
    return Token{TokenClass::INVALID, str, line, column};
  }
  scanner.next();
  return Token{TokenClass::CHAR_LITERAL, str, line, column};
}

static Token lexStringLiteral(Scanner &scanner, int line, int column,
                              ErrorFunction error) {
  std::string str{""};
  char nextChar{scanner.peek()};

  if (nextChar == -1)
    return Token{TokenClass::END, "EOF", line, column};

  while (nextChar != '"') {
    if (nextChar == '\\') {
      scanner.next();
      nextChar = toEscapeCharacter(scanner.peek());
    }

    if (nextChar == -1) {
      error(std::format(
          "Lexing error: string at {}:{} must be enclosed between quotes!",
          line, column));
      return Token{TokenClass::INVALID, str, line, column};
    }
    str += nextChar;
    scanner.next();
    nextChar = scanner.peek();
  }
  scanner.next();

  return Token{TokenClass::STRING_LITERAL, str, line, column};
}

static Token lexIntLiteral(Scanner &scanner, std::string str, int line,
                           int column, ErrorFunction error) {
  char nextChar{scanner.peek()};
  while (std::isdigit(nextChar)) {
    str += nextChar;
    scanner.next();
    nextChar = scanner.peek();
  }
  return Token{TokenClass::INT_LITERAL, str, line, column};
}

static Token lexInclude(Scanner &scanner, int line, int column,
                        ErrorFunction error) {
  char nextChar{scanner.peek()};
  std::string str{""};

  while (std::isalpha(nextChar)) {
    str += nextChar;
    scanner.next();
    nextChar = scanner.peek();
  }
  if (str == "include")
    return Token{TokenClass::INCLUDE, "#" + str, line, column};
  error(std::format("Lexing error: invalid include token at {}:{}!", line,
                    column));
  return Token{TokenClass::INVALID, str, line, column};
}

static Token possibleKeywordToToken(const std::string_view str, const int line,
                                    const int column) {
  if (str == IF)
    return Token(TokenClass::IF, std::string(str), line, column);
  if (str == ELSE)
    return Token(TokenClass::ELSE, std::string(str), line, column);
  if (str == WHILE)
    return Token(TokenClass::WHILE, std::string(str), line, column);
  if (str == RETURN)
    return Token(TokenClass::RETURN, std::string(str), line, column);
  if (str == STRUCT)
    return Token(TokenClass::STRUCT, std::string(str), line, column);
  if (str == SIZEOF)
    return Token(TokenClass::SIZEOF, std::string(str), line, column);
  if (str == INT)
    return Token(TokenClass::INT, std::string(str), line, column);
  if (str == VOID)
    return Token(TokenClass::VOID, std::string(str), line, column);
  if (str == CHAR)
    return Token(TokenClass::CHAR, std::string(str), line, column);

  return Token(TokenClass::IDENTIFIER, std::string(str), line, column);
}

static bool isEscapeCharacter(const char curChar) {
  return curChar == BACKSPACE || curChar == FORMFEED || curChar == NEWLINE ||
         curChar == CARRIAGE_RETURN || curChar == HORIZONTAL_TAB ||
         curChar == BACKSLASH || curChar == SINGLE_QUOTE ||
         curChar == NULL_TERMINATOR;
}

static char toEscapeCharacter(const char curChar) {
  if (curChar == BACKSPACE)
    return '\b';
  if (curChar == FORMFEED)
    return '\f';
  if (curChar == NEWLINE)
    return '\n';
  if (curChar == CARRIAGE_RETURN)
    return '\r';
  if (curChar == HORIZONTAL_TAB)
    return '\t';
  if (curChar == BACKSLASH)
    return '\\';
  if (curChar == SINGLE_QUOTE)
    return '\'';
  if (curChar == NULL_TERMINATOR)
    return '\0';
  return curChar;
}

} // namespace lexer
