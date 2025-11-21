#include "tokeniser.hpp"
#include <cctype>
#include <format>
#include <string>
#include <string_view>
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

Token Tokeniser::nextToken() {
  char nextChar;

  int line = scanner.getLine();
  int column = scanner.getColumn();

  nextChar = scanner.next();

  if (nextChar == -1)
    return Token(TokenClass::END, "EOF", line, column);

  if (std::isspace(nextChar))
    return nextToken();

  if (std::isalpha(nextChar) || nextChar == '_') { // keyword or ident
    std::string str{nextChar};
    nextChar = scanner.peek();

    if (nextChar == -1)
      return Token(TokenClass::END, "EOF", line, column);

    while (std::isalpha(nextChar)) {
      str += nextChar;
      scanner.next();
      nextChar = scanner.peek();

      if (nextChar == -1)
        return Token(TokenClass::END, "EOF", line, column);

      if (!std::isalnum(nextChar) && nextChar != '_') {
        return possibleKeywordToToken(str, line, column);
      }
    }

    while (std::isalnum(nextChar) || nextChar == '_') { // ident
      str += nextChar;
      scanner.next();
      nextChar = scanner.peek();

      if (nextChar == -1)
        return Token(TokenClass::END, "EOF", line, column);
    }
    Token foundToken{possibleKeywordToToken(str, line, column)};
    if (foundToken.type != TokenClass::IDENTIFIER) {
      error(std::format(
          "Lexing error: identifier at {}:{} cannot be reserved keyword!", line,
          column));
      return Token(TokenClass::INVALID, str, line, column);
    }
    return foundToken;
  }

  if (nextChar == '\'') { // char literal
    nextChar = scanner.peek();

    if (nextChar == -1)
      return Token(TokenClass::END, "EOF", line, column);

    std::string str;

    if (nextChar == '\\') { // escape char
      scanner.next();
      nextChar = scanner.peek();

      if (nextChar == -1)
        return Token(TokenClass::END, "EOF", line, column);

      if (isEscapeCharacter(nextChar)) {
        str = std::string{toEscapeCharacter(nextChar)};
        scanner.next();
        nextChar = scanner.peek();

        if (nextChar == -1)
          return Token(TokenClass::END, "EOF", line, column);

        if (nextChar != '\'') {
          error(std::format("Lexing error: char at {}:{} must be enclosed "
                            "between apostrophes!",
                            line, column));
          return Token(TokenClass::INVALID, str, line, column);
        }
        scanner.next();
        return Token(TokenClass::CHAR_LITERAL, str, line, column);
      }
    }
    str = std::string{nextChar};
    scanner.next();
    nextChar = scanner.peek();

    if (nextChar == -1)
      return Token(TokenClass::END, "EOF", line, column);

    if (nextChar != '\'') {
      error(std::format("Lexing error: char at {}:{} must be enclosed "
                        "between apostrophes!",
                        line, column));
      return Token(TokenClass::INVALID, str, line, column);
    }
    scanner.next();
    return Token(TokenClass::CHAR_LITERAL, str, line, column);
  }
}
} // namespace lexer
