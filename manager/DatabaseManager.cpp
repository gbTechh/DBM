#include "DatabaseManager.h"
#include "../indices/CampoCodificado.h"
#include <cstddef>
#include <string>
#include <vector>

DatabaseManager::DatabaseManager(int platos, int superficies, int pistas,
                                 int sectores, int tamanioSector)
    : disco(platos, superficies, pistas, sectores, tamanioSector) {}

void DatabaseManager::insertRegistro(std::vector<std::string> reg) {
  std::vector<std::string> attrs;
  int id;
  indiceAttr.setCampos(registro.getCamposNombre());
  indiceAttr.setTipos(registro.getCamposTipo());

  std::vector<int> sizes = registro.getCamposSize();

  for (size_t i = 0; i < reg.size(); i++) {
    if (i == 0) {
      id = std::stoi(reg[i]);
    } else {
      attrs.push_back(reg[i]);
      Head h = registro.GetHeaderByIndex(i);
      indiceAttr.insert(Campo_Codificado(i, h.type, reg[i]), id);
    }
  }
  int inicio, final;
  disco.agregarRegistro(sizes, inicio, final);
  indiceId.insert(id, attrs, inicio, final);
}

std::vector<Registro> DatabaseManager::getAllRecords() const {
  std::vector<Registro> registros;
  // Aquí puedes recorrer todas las direcciones del disco y deserializar
  return registros;
}

Registro DatabaseManager::getRegitro() { return registro; }

bool DatabaseManager::getRegistroByID(
    int id, std::pair<int, std::vector<std::string>> &registro) {
  int in, fin;
  return indiceId.find(id, registro, in, fin);
}
bool DatabaseManager::getRegistroByAttr(std::string column, std::string &attr) {
  // return indiceAttr.find(column, attr);
  return true;
}

void DatabaseManager::AddHeaderInRegistro(std::string name, std::string type,
                                          int size) {
  registro.AddHeader(name, type, size);
}

void DatabaseManager::setHeadersRegistros() { registro.setCamposAndDatos(); }
void DatabaseManager::setHeaderLine(std::string h) {
  registro.setHeaderLine(h);
}
void DatabaseManager::setHeaderLineWithoutID(std::string h) {
  registro.setHeaderLineWithoutID(h);
}

IndexID &DatabaseManager::getIndexID() { return indiceId; }
IndexAttr &DatabaseManager::getIndexAttr() { return indiceAttr; }
Disco &DatabaseManager ::getDisco() { return disco; }
