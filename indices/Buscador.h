#ifndef BUSCADOR_H
#define BUSCADOR_H

#include "AVL_Busqueda.h"
#include "AVL_ID.h"
#include <string>
#include <vector>

enum Operador { AND, OR };
enum Orden { NO, ASC, DES };

class Buscador {
private:
  AVL_Busqueda arbol_busqueda;
  AVL_ID arbol_id;
  std::vector<unsigned int> combinarVec(std::vector<unsigned int> final,
                                        std::vector<unsigned int> parcial,
                                        Operador op);

public:
  Buscador(std::vector<std::string> &campos, std::vector<std::string> &tipoDato,
           std::vector<std::vector<std::string>> &registros);
  Buscador();
  std::vector<std::vector<std::string>>
  busquedaCompleta(std::vector<std::string> &camposBuscados,
                   std::vector<std::string> &valoresBuscados,
                   std::vector<std::string> &camposEsperados, Operador ope,
                   std::string &orden, Orden dirOrden);
  bool insertReg(std::vector<std::string> registro);
  void print();
};

#endif // BUSCADOR_H