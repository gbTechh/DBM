#ifndef AVL_BUSQUEDA_H
#define AVL_BUSQUEDA_H

#include "CampoCodificado.h"
#include "NodoAvlBusqueda.h"
#include <string>
#include <vector>

class AVL_Busqueda {
private:
  NodoAVLBusqueda *root;
  std::vector<std::string> campos, tipos;

  int altura(NodoAVLBusqueda *nodo);
  int balance(NodoAVLBusqueda *nodo);
  void actualizarAltura(NodoAVLBusqueda *nodo);
  NodoAVLBusqueda *rotarDerecha(NodoAVLBusqueda *y);
  NodoAVLBusqueda *rotarIzquierda(NodoAVLBusqueda *x);
  NodoAVLBusqueda *insertar(NodoAVLBusqueda *nodo,
                            const Campo_Codificado &clave, unsigned int id);
  void inorder(NodoAVLBusqueda *nodo);
  std::vector<unsigned int> busquedaInterna(NodoAVLBusqueda *nodo,
                                            const Campo_Codificado &clave);

public:
  AVL_Busqueda();
  void insertar(const Campo_Codificado &clave, unsigned int id);
  void inorder();
  void setCampos(const std::vector<std::string> &_campos);
  void setTipos(const std::vector<std::string> &_tipos);
  std::vector<unsigned int> buscar(const std::string &campoNombre,
                                   const std::string &valor);
  std::vector<unsigned int> buscarRango(const std::string &campo,
                                        const std::string &operador,
                                        const std::string &valor);

  friend class Buscador;
};

#endif // AVL_BUSQUEDA_H
