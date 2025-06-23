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

void Registro::printHeader() {
  for (size_t i = 0; i < header.size(); i++) {
    std::cout << "name: " << header[i].name << "\n";
    std::cout << "type: " << header[i].type << "\n";
    std::cout << "size: " << header[i].size << "\n";
  }
}