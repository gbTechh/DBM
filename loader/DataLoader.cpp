#include "DataLoader.h"
#include "../file/File.h"
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

char separador = '|';
DataLoader::DataLoader() {};

void DataLoader::loadFromFile(const std::string &filename,
                              DatabaseManager &dbManager) {
  File file(filename); // Abre el archivo
  // std::vector<Registro> registros;

  // Leer cabecera (tipos o nombres de campos)
  std::string header;
  if (!file.ReadLine(header)) {
    throw std::runtime_error(
        "El archivo está vacío o no se pudo leer la cabecera.");
  } else {
    std::vector<std::string> campos = split(header, separador);
    for (const auto &campo : campos) {
      std::vector<std::string> partes = split(campo, ':');
      int size = std::stoi(partes[2]);
      dbManager.AddHeaderInRegistro(partes[0], partes[1], size);
      dbManager.setHeaderLine(header);
      // dbManager.setHeaderLineWithoutID(header);
      //  dbManager.getIndexAttr().setCampos()
    }
  }

  dbManager.setHeadersRegistros();
  // Leer registros línea por línea
  std::string line;
  while (file.ReadLine(line)) {
    std::vector<std::string> lineSplited = split(line, separador);
    dbManager.insertRegistro(lineSplited);
  }
}

void DataLoader::getColumnHead(std::string line) {}

std::vector<std::string> DataLoader::split(const std::string &str,
                                           char delimiter) {
  std::vector<std::string> tokens;
  std::istringstream iss(str);
  std::string token;

  while (std::getline(iss, token, delimiter)) {
    if (!token.empty())
      tokens.push_back(token);
  }

  return tokens;
}
