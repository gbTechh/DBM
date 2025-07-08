#include "Tokenizer.h"
#include <algorithm>
#include <cctype>
#include <cstring>

Tokenizer::Tokenizer(const std::string &input) : input(input), pos(0) {}

void Tokenizer::skipWhitespace() {
  while (pos < input.length() && std::isspace(input[pos]))
    ++pos;
}

std::string Tokenizer::parseNumber() {
  size_t start = pos;
  while (pos < input.length() && std::isdigit(input[pos]))
    ++pos;

  if (pos < input.length() && input[pos] == '.') {
    ++pos;
    while (pos < input.length() && std::isdigit(input[pos]))
      ++pos;
  }

  return input.substr(start, pos - start);
}

std::string Tokenizer::parseQuotedString() {
  char quote = input[pos];
  size_t start = pos;
  ++pos;

  while (pos < input.length() && input[pos] != quote) {
    if (input[pos] == '\\' && pos + 1 < input.length()) // Handle escape
      ++pos;
    ++pos;
  }

  if (pos < input.length())
    ++pos; // Skip closing quote

  return input.substr(start, pos - start);
}

std::string Tokenizer::parseIdentifier() {
  size_t start = pos;
  while (pos < input.length() &&
         (std::isalnum(input[pos]) || input[pos] == '_'))
    ++pos;
  return input.substr(start, pos - start);
}

std::string Tokenizer::parseOperator() {
  // Multi-character operators
  if (pos + 1 < input.length()) {
    std::string twoChar = input.substr(pos, 2);
    if (twoChar == ">=" || twoChar == "<=" || twoChar == "!=" ||
        twoChar == "<>") {
      pos += 2;
      return twoChar;
    }
  }

  // Single-character operators
  std::string op(1, input[pos++]);
  return op;
}

std::string Tokenizer::nextToken() {
  skipWhitespace();
  if (pos >= input.length())
    return "";

  char c = input[pos];

  if (std::isdigit(c))
    return parseNumber();

  if (c == '\'' || c == '"')
    return parseQuotedString();

  if (std::isalpha(c) || c == '_')
    return parseIdentifier();

  if (strchr("=><,()", c))
    return parseOperator();

  // Default case (shouldn't normally reach here)
  return std::string(1, input[pos++]);
}

std::vector<std::string> Tokenizer::tokenize() {
  std::vector<std::string> tokens;
  std::string token;

  while (!(token = nextToken()).empty()) {
    // Convert SQL keywords to uppercase
    std::string upperToken = token;
    std::transform(upperToken.begin(), upperToken.end(), upperToken.begin(),
                   ::toupper);

    if (upperToken == "SELECT" || upperToken == "FROM" ||
        upperToken == "WHERE" || upperToken == "AND" || upperToken == "OR") {
      tokens.push_back(upperToken);
    } else {
      tokens.push_back(token);
    }
  }

  return tokens;
}