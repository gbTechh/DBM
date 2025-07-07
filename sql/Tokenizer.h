#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <vector>

class Tokenizer {
public:
  explicit Tokenizer(const std::string &input);
  std::vector<std::string> tokenize();

private:
  std::string input;
  size_t pos;

  void skipWhitespace();
  std::string nextToken();
};

#endif
