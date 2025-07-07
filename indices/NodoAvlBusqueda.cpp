#include "NodoAvlBusqueda.h"

NodoAVLBusqueda::NodoAVLBusqueda(const Campo_Codificado &clave, unsigned int id)
    : clave(clave), izquierdo(nullptr), derecho(nullptr), altura(1) {
  ids.push_back(id);
}
