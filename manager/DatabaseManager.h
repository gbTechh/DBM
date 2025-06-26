#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include "../disco/Disco.h"
#include "../indices/Buscador.h"
#include "../indices/IndexAttr.h"
#include "../indices/IndexID.h"
#include "../registro/Registro.h"

#include <string>
#include <vector>

class DatabaseManager {
public:
  // Constructor: inicializa el disco con los parámetros dados
  DatabaseManager(int platos = 2, int pistas = 100, int sectores = 8,
                  int tamanioSector = 100);

  DatabaseManager(Buscador b);
  // Insertar un registro en el disco
  void insertRegistro(std::vector<std::string> line);
  void insertHeader(const Registro &registro);

  // Obtener todos los registros (lee desde el disco)
  std::vector<Registro> getAllRecords() const;

  // Buscar por ID usando índice
  Registro getRecordById(int id) const;

  // Mostrar todos los registros (para depuración)
  void displayAllRecords() const;

  Registro getRegitro();

  void AddHeaderInRegistro(std::string name, std::string type, int size);

  IndexID getIndexID();
  IndexAttr getIndexAttr();

  bool getRegistroByID(int id, std::pair<int, std::string> &registro);
  bool getRegistroByAttr(std::string column, std::string &value);

  void setBuscador(Buscador b);
  Buscador getBuscador();

  void setHeadersRegistros();
  void setHeaderLine(std::string h);

private:
  Registro registro;
  Disco disco;
  IndexID indiceId;     // Índice B+ para búsquedas por ID
  IndexAttr indiceAttr; // Índice B+ para búsquedas por ID
  Buscador buscador;
};

#endif // DATABASE_MANAGER_H