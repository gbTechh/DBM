#include "Registro.h"
#include <cstddef>
#include <iostream>

void Registro::AddHeader(std::string _name, std::string _type, int _size) {
  Head h;
  h.name = _name;
  h.type = _type;
  h.size = _size;
  header.push_back(h);
  camposTipo.push_back(h.type);
  camposNombre.push_back(h.name);
}

Head Registro::GetHeaderByIndex(int index) { return header[index]; }

void Registro::addRegistro(std::vector<std::string> r) {
  registros.push_back(r);
}

void Registro::setCamposAndDatos() {
  for (size_t i = 0; i < header.size(); i++) {
    Head h = header[i];
    // camposNombre.push_back(h.name);
    //.push_back(h.type);
  }
}

std::vector<std::vector<std::string>> Registro::getAllRegistros() {
  return registros;
}

std::string Registro::getHeaderLine() { return headerLine; }
std::string Registro::getHeaderLineWithoutID() { return headerLineWithouID; }

void Registro::setHeaderLine(std::string h) { headerLine = h; }

void Registro::setHeaderLineWithoutID(std::string h) {
  // Buscar la posición del primer campo "id:INT:4"
  std::string toRemove = "id:INT:4,";
  size_t pos = h.find(toRemove);

  // Si se encuentra el campo, eliminarlo
  if (pos != std::string::npos) {
    h.erase(pos, toRemove.length());
  } else {
    // Si no se encuentra "id:INT:4,", verificar si está al final sin coma
    toRemove = "id:INT:4";
    pos = h.find(toRemove);
    if (pos != std::string::npos) {
      h.erase(pos, toRemove.length());
    }
  }

  // Asignar la cadena modificada a headerLine
  headerLineWithouID = h;
}

std::vector<std::string> Registro::getCamposNombre() const {
  return camposNombre;
}
std::vector<std::string> Registro::getCamposTipo() const { return camposTipo; }

std::vector<int> Registro::getCamposSize() const {
  std::vector<int> sizes;
  for (size_t i = 0; i < header.size(); i++) {
    sizes.push_back(header[i].size);
  }
  return sizes;
}

std::vector<std::string> Registro::getTipoDato() const { return tipoDato; }

void Registro::printHeader() {
  for (size_t i = 0; i < header.size(); i++) {
    std::cout << "name: " << header[i].name << "\n";
    std::cout << "type: " << header[i].type << "\n";
    std::cout << "size: " << header[i].size << "\n";
  }
}