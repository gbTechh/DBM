#include "DatabaseManager.h"
#include <string>
#include <vector>

DatabaseManager::DatabaseManager(int platos, int pistas, int sectores,
                                 int tamanioSector)
    : disco(platos, pistas, sectores, tamanioSector) {}

void DatabaseManager::insertRegistro(std::vector<std::string> reg) {
  std::string attrs;
  int id;
  for (size_t i = 0; i < reg.size(); i++) {
    if (i == 0) {
      id = std::stoi(reg[i]);
      indiceId.insert(id, attrs);
    } else {
      attrs = attrs + reg[i];
      Head h = registro.GetHeaderByIndex(i);
      indiceAttr.insert(h.name, reg[i]);
    }
  }

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

bool DatabaseManager::getRegistroByID(int id,
                                      std::pair<int, std::string> &registro) {

  return indiceId.find(id, registro);
}
bool DatabaseManager::getRegistroByAttr(std::string column, std::string &attr) {
  return indiceAttr.find(column, attr);
}

void DatabaseManager::AddHeaderInRegistro(std::string name, std::string type,
                                          int size) {
  registro.AddHeader(name, type, size);
}

IndexID DatabaseManager::getIndexID() { return indiceId; }
IndexAttr DatabaseManager::getIndexAttr() { return indiceAttr; }

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