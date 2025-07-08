#ifndef SQL_H
#define SQL_H

#include "../manager/DatabaseManager.h"
#include "Parser.h"
#include "Tokenizer.h"
#include <string>
#include <vector>

class SQL {
private:
  Tokenizer tokenizer;
  Parser parser;
  DatabaseManager &dbManager;

  bool cumpleCondiciones(const std::vector<std::string> &registro);
  std::vector<unsigned int> buscarAttr(const std::string columna,
                                       const std::string valor);
  bool evaluarCondicion(const std::string &valor, const std::string &esperado,
                        const std::string &tipo, const std::string &operador);

public:
  std::vector<std::pair<int, std::vector<std::string>>>
  getRegistros(std::vector<std::tuple<int, int, int>> &bitesVector);
  Consulta consulta;
  Consulta getConsulta(){ return consulta;}
  SQL(const std::string &query, DatabaseManager &dbManager);
  std::vector<std::pair<int, std::vector<std::string>>> getRegistrosConUbicacion(std::vector<std::pair<UbicacionFisica, UbicacionFisica>> &ubicacionesVector) ;
};

#endif