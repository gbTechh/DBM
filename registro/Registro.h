#ifndef REGISTROS_H
#define REGISTROS_H

#include <string>
#include <vector>

struct Head {
  std::string name;
  std::string type;
  int size;
};

class Registro {
public:
  void AddColumn(std::string name, std::string type, int size);
  Head GetHeaderByIndex(int index);
  void AddHeader(std::string name, std::string type, int size);

  void printHeader();

  void setCamposAndDatos();

  void addRegistro(std::vector<std::string> r);
  std::vector<std::vector<std::string>> getAllRegistros();

  std::string getHeaderLine();
  void setHeaderLine(std::string h);

private:
  std::string headerLine;
  std::vector<Head> header;
  std::vector<std::string> camposNombre;
  std::vector<std::string> tipoDato;
  std::vector<std::vector<std::string>> registros;
};

#endif // REGISTROS_H