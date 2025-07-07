#include "DatabaseManager.h"
#include "../indices/CampoCodificado.h"
#include <cstddef>
#include <iostream>
#include <sstream>
#include <stdexcept>
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
      //  registro.addRegistro(reg);
      // buscador.insertReg(reg);
    }
  }
  int inicio, final;
  disco.agregarRegistro(sizes, inicio, final);
  indiceId.insert(id, attrs, inicio, final);
  //   DireccionDisco direccion = disco.buscarEspacioLibre();

  //   std::string data = registro.serializar();
  //   disco.escribir(direccion.plato, direccion.superficie, direccion.pista,
  //                  direccion.sector, data.c_str(), data.size());

  //   // Guardar en índice
  //   indiceId.insertar(registro.getId(), direccion);
}

std::vector<Registro> DatabaseManager::getAllRecords() const {
  std::vector<Registro> registros;
  // Aquí puedes recorrer todas las direcciones del disco y deserializar
  return registros;
}

Registro DatabaseManager::getRegitro() { return registro; }

bool DatabaseManager::getRegistroByID(
    int id, std::pair<int, std::vector<std::string>> &registro,
    DireccionDisco &dir) {
  int in, fin;
  return indiceId.find(id, registro, in, fin);
}
bool DatabaseManager::getRegistroByAttr(std::string column, std::string &attr) {
  // return indiceAttr.find(column, attr);
  return true;
}

void DatabaseManager::AddHeaderInRegistro(std::string name, std::string type,
                                          int size) {
  // EnumType eType;
  // std::cout << "tipe Header:: " << type << "\n";
  // if (type == "INT") {
  //   eType = INT;
  // } else if (type == "FLOAT" || type == "DOUBLE") {
  //   eType = FLOAT;
  // } else if (type == "VARCHAR") {
  //   eType = VARCHAR;
  // }
  // std::cout << "ETYPE HEADER: " << eType << "\n";
  registro.AddHeader(name, type, size);
}

void DatabaseManager::setHeadersRegistros() { registro.setCamposAndDatos(); }
void DatabaseManager::setHeaderLine(std::string h) {
  registro.setHeaderLine(h);
}
void DatabaseManager::setHeaderLineWithoutID(std::string h) {
  registro.setHeaderLineWithoutID(h);
}

void DatabaseManager::setBuscador(Buscador b) { buscador = b; }
Buscador DatabaseManager::getBuscador() { return buscador; }

IndexID &DatabaseManager::getIndexID() { return indiceId; }
IndexAttr &DatabaseManager::getIndexAttr() { return indiceAttr; }
Disco &DatabaseManager ::getDisco() { return disco; }

Buscador DatabaseManager::cargarBuscador() {
  auto campos = registro.getCamposNombre();
  auto tipos = registro.getTipoDato();
  auto registros = registro.getAllRegistros();

  std::cout << "CAMPOS: ";
  // buscador = Buscador(campos, tipos, registros);
  buscador = Buscador();
  return buscador;
}
// Registro DatabaseManager::getRecordById(int id) const {
//   DireccionDisco dir = indiceId.buscar(id);
//   char *buffer = new char[disco.getTamanioSector()];

//   disco.leer(dir.plato, dir.superficie, dir.pista, dir.sector, buffer,
//              disco.getTamanioSector());

//   Registro reg = Registro::deserializar(std::string(buffer));
//   delete[] buffer;
//   return reg;
// }

// void DatabaseManager::displayAllRecords() const {
//   for (const Registro &reg : getAllRecords()) {
//     std::cout << reg.toString() << std::endl;
//   }
// }