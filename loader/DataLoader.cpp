#include "DataLoader.h"
#include "../file/File.h"
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

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
    std::vector<std::string> campos = split(header, ',');
    for (const auto &campo : campos) {
      std::vector<std::string> partes = split(campo, ':');
      int size = std::stoi(partes[2]);
      dbManager.AddHeaderInRegistro(partes[0], partes[1], size);
      dbManager.setHeaderLine(header);
    }
  }

  dbManager.setHeadersRegistros();
  // Leer registros línea por línea
  std::string line;
  while (file.ReadLine(line)) {
    std::vector<std::string> lineSplited = split(line, ',');
    dbManager.insertRegistro(lineSplited);

    // buscador(campos_nombre, tipoDato, registros);
    //  Registro registro = parseLineToRecord(line);
    //  dbManager.insertRegistro(registro); // Insertar en disco e índice
    //   registros.push_back(registro);
  }

  // return registros;
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

// Registro DataLoader::parseLineToRecord(const std::string &line) {
//   std::istringstream iss(line);
//   Registro registro;
//   std::string value;
//   int fieldIndex = 0;

//   while (iss >> value) {
//     // Aquí puedes mejorar usando nombres de campo leídos de la cabecera si
//     lo
//     // deseas
//     std::string fieldName = "campo" + std::to_string(fieldIndex++);
//     registro.atributos[fieldName] = value;
//   }

//   // Asignamos un ID básico basado en hash simple del contenido
//   // OJO: esto es solo un ejemplo. Puedes tener un generador de IDs único si
//   lo
//   // prefieres.
//   registro.id = std::hash<std::string>{}(line);

//   return registro;
// }