#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>

struct Condicion {
  std::string campo;
  std::string operador;
  std::string valor;
};

struct Consulta {
  std::vector<std::string> camposSeleccionados;
  std::string tabla;
  std::vector<Condicion> condiciones;
  std::vector<std::string> conectores;
};

class Parser {
public:
  explicit Parser(const std::vector<std::string> &tokens);
  Consulta parse();

private:
  std::vector<std::string> tokens;
  size_t pos;

  std::string current();
  void advance();
  void expect(const std::string &expected);

  std::vector<std::string> parseCampos();
  Condicion parseCondicion();
};

#endif
