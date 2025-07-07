#ifndef REGISTROS_H
#define REGISTROS_H

#include "../utils/Enum.h"
#include <string>
#include <vector>

struct Head {
  std::string name;
  std::string type;
  int size;
};

class Registro {
public:
  void AddColumn(std::string name, EnumType type, int size);
  Head GetHeaderByIndex(int index);
  void AddHeader(std::string name, std::string type, int size);

  void printHeader();

  void setCamposAndDatos();

  void addRegistro(std::vector<std::string> r);
  std::vector<std::vector<std::string>> getAllRegistros();

  std::string getHeaderLine();
  std::string getHeaderLineWithoutID();
  void setHeaderLine(std::string h);
  void setHeaderLineWithoutID(std::string h);

  std::vector<std::string> getCamposNombre() const; // Nuevo método
  std::vector<int> getCamposSize() const;           // Nuevo método
  std::vector<std::string> getCamposTipo() const;   // Nuevo método
  std::vector<std::string> getTipoDato() const;     // Nuevo método

private:
  std::string headerLine;
  std::string headerLineWithouID;
  std::vector<Head> header;
  std::vector<std::string> camposNombre;
  std::vector<std::string> camposTipo;
  std::vector<std::string> tipoDato;
  std::vector<std::vector<std::string>> registros;
};

#endif // REGISTROS_H