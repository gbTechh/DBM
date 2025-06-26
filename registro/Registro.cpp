#include "Registro.h"
#include <cstddef>
#include <iostream>

void Registro::AddHeader(std::string _name, std::string _type, int _size) {
  Head h;
  h.name = _name;
  h.type = _type;
  h.size = _size;
  header.push_back(h);
}

Head Registro::GetHeaderByIndex(int index) { return header[index]; }

void Registro::addRegistro(std::vector<std::string> r) {
  registros.push_back(r);
}

void Registro::setCamposAndDatos() {
  for (size_t i = 0; i < header.size(); i++) {
    Head h = header[i];
    camposNombre.push_back(h.name);
    tipoDato.push_back(h.type);
  }
}

std::vector<std::vector<std::string>> Registro::getAllRegistros() {
  return registros;
}

std::string Registro::getHeaderLine() { return headerLine; }

void Registro::setHeaderLine(std::string h) { headerLine = h; }

void Registro::printHeader() {
  for (size_t i = 0; i < header.size(); i++) {
    std::cout << "name: " << header[i].name << "\n";
    std::cout << "type: " << header[i].type << "\n";
    std::cout << "size: " << header[i].size << "\n";
  }
}