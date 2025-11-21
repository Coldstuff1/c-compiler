#include "scanner.hpp"

namespace lexer {

int Scanner::getColumn() { return column; }

int Scanner::getLine() { return line; }

char Scanner::peek() {
  if (peeked != -1)
    return peeked;

  if (pos == (buffer.size()))
    return -1;

  peeked = buffer[pos++];

  return peeked;
}

char Scanner::next() {
  char nextChar;

  if (peeked != -1) {
    nextChar = peeked;
    peeked = -1;
  } else {

    if (pos == (buffer.size()))
      return -1;

    nextChar = buffer[pos++];
  }

  if (nextChar == '\n') {
    line++;
    column = 1;
  } else
    column++;
  return nextChar;
}

bool Scanner::hasNext() {
  if (peeked != -1)
    return true;

  if (pos == (buffer.size()))
    return false;

  peeked = buffer[pos++];

  return true;
}
} // namespace lexer
