#ifndef NODO_AVL_B_H
#define NODO_AVL_B_H

#include "CampoCodificado.h"
#include <vector>

struct NodoAVLBusqueda {
  Campo_Codificado clave;
  std::vector<unsigned int> ids;
  NodoAVLBusqueda *izquierdo;
  NodoAVLBusqueda *derecho;
  int altura;

  NodoAVLBusqueda(const Campo_Codificado &clave, unsigned int id);
};

#endif // NODO_AVL_H
