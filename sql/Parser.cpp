#include "Parser.h"
#include <algorithm>

Parser::Parser(const std::vector<std::string> &tokens)
    : tokens(tokens), pos(0) {}

std::string Parser::current() {
  return (pos < tokens.size()) ? tokens[pos] : "";
}

void Parser::advance() {
  if (pos < tokens.size())
    ++pos;
}

void Parser::expect(const std::string &expected) {
  if (current() != expected) {
    throw std::runtime_error("Se esperaba '" + expected +
                             "' pero se encontró '" + current() + "'");
  }
  advance();
}

std::vector<std::string> Parser::parseCampos() {
  std::vector<std::string> campos;

  while (true) {
    if (current().empty() || current() == "FROM")
      break;

    if (current() == ",") {
      advance();
      continue;
    }

    campos.push_back(current());
    advance();
  }

  return campos;
}

std::string Parser::parseValor() {
  std::string val = current();

  // Remove quotes from quoted strings
  if ((val.front() == '\'' || val.front() == '"') &&
      val.back() == val.front() && val.size() >= 2) {
    return val.substr(1, val.size() - 2);
  }

  return val;
}

Condicion Parser::parseCondicion() {
  Condicion c;

  c.campo = current();
  advance();

  c.operador = current();
  advance();

  c.valor = parseValor();
  advance();

  return c;
}

Consulta Parser::parse() {
  Consulta consulta;

  expect("SELECT");
  consulta.camposSeleccionados = parseCampos();

  expect("FROM");
  consulta.tabla = current();
  advance();

  if (current() == "WHERE") {
    advance();
    consulta.condiciones.push_back(parseCondicion());

    while (current() == "AND" || current() == "OR") {
      consulta.conectores.push_back(current());
      advance();
      consulta.condiciones.push_back(parseCondicion());
    }
  }

  return consulta;
}