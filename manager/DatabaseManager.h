#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include "../disco/Disco.h"
#include "../indices/AVL_Busqueda.h"
#include "../indices/Buscador.h"
#include "../indices/IndexAttr.h"
#include "../indices/IndexID.h"
#include "../registro/Registro.h"

#include <string>
#include <vector>

class DatabaseManager {
public:
  // Constructor: inicializa el disco con los parámetros dados
  DatabaseManager(int platos, int superficies, int pistas, int sectores,
                  int tamanioSector);

  void insertRegistro(std::vector<std::string> reg);
  std::vector<Registro> getAllRecords() const;

  Registro getRegitro();
  bool getRegistroByID(int id,
                       std::pair<int, std::vector<std::string>> &registro,
                       DireccionDisco &dir);
  bool getRegistroByAttr(std::string column, std::string &attr);

  void AddHeaderInRegistro(std::string name, std::string type, int size);
  void setHeadersRegistros();
  void setHeaderLine(std::string h);
  void setHeaderLineWithoutID(std::string h);

  void setBuscador(Buscador b);
  Buscador getBuscador();
  IndexID &getIndexID();
  IndexAttr &getIndexAttr();
  Disco &getDisco();
  Buscador cargarBuscador();

private:
  Registro registro;
  Disco disco;
  IndexID indiceId;        // Índice B+ para búsquedas por ID
  IndexAttr indiceAttr;    // Índice B+ para búsquedas por ID
  AVL_Busqueda avlIndices; // Índice B+ para búsquedas por ID
  Buscador buscador;
  size_t offsetActual = 0;
};

#endif // DATABASE_MANAGER_H