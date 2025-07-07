#include "Tokenizer.h"
#include <cctype>
#include <cstring>

Tokenizer::Tokenizer(const std::string &input) : input(input), pos(0) {}

void Tokenizer::skipWhitespace() {
  while (pos < input.length() && std::isspace(input[pos]))
    ++pos;
}

std::string Tokenizer::nextToken() {
  skipWhitespace();
  if (pos >= input.size())
    return "";

  // Operadores multi-char
  if (input.compare(pos, 2, ">=") == 0 || input.compare(pos, 2, "<=") == 0 ||
      input.compare(pos, 2, "!=") == 0 || input.compare(pos, 2, "<>") == 0) {
    pos += 2;
    return input.substr(pos - 2, 2);
  }

  // Operadores de un solo caracter
  if (strchr("=><,", input[pos])) {
    return std::string(1, input[pos++]);
  }

  // Cadenas entre comillas
  if (input[pos] == '\'') {
    size_t end = input.find('\'', pos + 1);
    std::string val = input.substr(pos, end - pos + 1);
    pos = end + 1;
    return val;
  }

  // Palabras o identificadores
  size_t start = pos;
  while (pos < input.length() &&
         (std::isalnum(input[pos]) || input[pos] == '_')) {
    ++pos;
  }
  return input.substr(start, pos - start);
}

std::vector<std::string> Tokenizer::tokenize() {
  std::vector<std::string> tokens;
  std::string token;
  while (!(token = nextToken()).empty()) {
    tokens.push_back(token);
  }
  return tokens;
}
