
#include "AVL_Busqueda.h"
#include "NodoAvlBusqueda.h"
#include <algorithm>
#include <cstring>
#include <functional>
#include <iostream>

using namespace std;

AVL_Busqueda::AVL_Busqueda() : root(nullptr) {}

int AVL_Busqueda::altura(NodoAVLBusqueda *nodo) {
  return nodo ? nodo->altura : 0;
}

int AVL_Busqueda::balance(NodoAVLBusqueda *nodo) {
  return nodo ? altura(nodo->izquierdo) - altura(nodo->derecho) : 0;
}

void AVL_Busqueda::actualizarAltura(NodoAVLBusqueda *nodo) {
  nodo->altura = 1 + max(altura(nodo->izquierdo), altura(nodo->derecho));
}

NodoAVLBusqueda *AVL_Busqueda::rotarDerecha(NodoAVLBusqueda *y) {
  NodoAVLBusqueda *x = y->izquierdo;
  NodoAVLBusqueda *T2 = x->derecho;

  x->derecho = y;
  y->izquierdo = T2;

  actualizarAltura(y);
  actualizarAltura(x);

  return x;
}

NodoAVLBusqueda *AVL_Busqueda::rotarIzquierda(NodoAVLBusqueda *x) {
  NodoAVLBusqueda *y = x->derecho;
  NodoAVLBusqueda *T2 = y->izquierdo;

  y->izquierdo = x;
  x->derecho = T2;

  actualizarAltura(x);
  actualizarAltura(y);

  return y;
}

NodoAVLBusqueda *AVL_Busqueda::insertar(NodoAVLBusqueda *nodo,
                                        const Campo_Codificado &clave,
                                        unsigned int id) {
  std::cout << "Insertando en el avl buscar..." << "\n";
  if (!nodo)
    return new NodoAVLBusqueda(clave, id);

  if (clave == nodo->clave) {
    nodo->ids.push_back(id);
    if (nodo->ids.size() > 1 &&
        nodo->ids[nodo->ids.size() - 1] < nodo->ids[nodo->ids.size() - 2])
      sort(nodo->ids.begin(), nodo->ids.end());
    return nodo;
  }

  if (clave < nodo->clave)
    nodo->izquierdo = insertar(nodo->izquierdo, clave, id);
  else
    nodo->derecho = insertar(nodo->derecho, clave, id);

  actualizarAltura(nodo);
  int bal = balance(nodo);

  if (bal > 1 && clave < nodo->izquierdo->clave)
    return rotarDerecha(nodo);
  if (bal < -1 && clave > nodo->derecho->clave)
    return rotarIzquierda(nodo);
  if (bal > 1 && clave > nodo->izquierdo->clave) {
    nodo->izquierdo = rotarIzquierda(nodo->izquierdo);
    return rotarDerecha(nodo);
  }
  if (bal < -1 && clave < nodo->derecho->clave) {
    nodo->derecho = rotarDerecha(nodo->derecho);
    return rotarIzquierda(nodo);
  }

  return nodo;
}

void AVL_Busqueda::insertar(const Campo_Codificado &clave, unsigned int id) {
  root = insertar(root, clave, id);
}

std::vector<unsigned int> AVL_Busqueda::buscar(const std::string &campoNombre,
                                               const std::string &valor) {
  size_t campoIndex = 0;
  for (size_t i = 0; i < campos.size(); ++i) {
    if (campos[i] == campoNombre) {
      campoIndex = i;
      break;
    }
  }
  if (campoIndex >= tipos.size())
    return {};

  Campo_Codificado clave;
  clave.campo = static_cast<unsigned int>(campoIndex);

  std::string tipo = tipos[campoIndex];
  if (tipo == "int") {
    clave.tipo = TIPO_ENTERO;
    try {
      // clave.valorEntero =
      std::stoll(valor); // Usa long long si cambias el tipo en tu struct
    } catch (const std::exception &e) {
      std::cerr << "Error de conversión a long long: " << e.what() << "\n";
    }
  } else if (tipo == "float") {
    clave.tipo = TIPO_FLOTANTE;
    // clave.valorFloat = stof(valor);
  } else {
    clave.tipo = TIPO_STRING;
    // clave.valorString = strdup(valor.c_str()); // cuidado con memoria
  }

  return busquedaInterna(root, clave);
}

std::vector<unsigned int>
AVL_Busqueda::busquedaInterna(NodoAVLBusqueda *nodo,
                              const Campo_Codificado &clave) {
  if (!nodo)
    return {};

  if (clave == nodo->clave)
    return nodo->ids;
  else if (clave < nodo->clave)
    return busquedaInterna(nodo->izquierdo, clave);
  else
    return busquedaInterna(nodo->derecho, clave);
}

std::vector<unsigned int> AVL_Busqueda::buscarRango(const std::string &campo,
                                                    const std::string &operador,
                                                    const std::string &valor) {
  std::vector<unsigned int> res;
  size_t campoIndex = 0;
  for (size_t i = 0; i < campos.size(); ++i) {
    if (campos[i] == campo) {
      campoIndex = i;
      break;
    }
  }

  if (campoIndex >= campos.size())
    return res;

  std::function<void(NodoAVLBusqueda *)> recorrer = [&](NodoAVLBusqueda *nodo) {
    if (!nodo)
      return;

    recorrer(nodo->izquierdo);

    if (nodo->clave.campo == campoIndex) {
      bool cumple = false;

      if (nodo->clave.tipo == TIPO_ENTERO) {
        int cmp = nodo->clave.campo;
        int v = stoi(valor);
        cumple = (operador == "<" && cmp < v) || (operador == ">" && cmp > v) ||
                 (operador == "<=" && cmp <= v) ||
                 (operador == ">=" && cmp >= v);
      } else if (nodo->clave.tipo == TIPO_FLOTANTE) {
        float cmp = nodo->clave.campo;
        float v = stof(valor);
        cumple = (operador == "<" && cmp < v) || (operador == ">" && cmp > v) ||
                 (operador == "<=" && cmp <= v) ||
                 (operador == ">=" && cmp >= v);
      }

      if (cumple)
        res.insert(res.end(), nodo->ids.begin(), nodo->ids.end());
    }

    recorrer(nodo->derecho);
  };

  recorrer(root);
  return res;
}

void AVL_Busqueda::inorder(NodoAVLBusqueda *nodo) {
  if (!nodo)
    return;

  inorder(nodo->izquierdo);

  if (nodo->clave.campo < campos.size())
    cout << campos[nodo->clave.campo];
  else
    cout << static_cast<int>(nodo->clave.campo);

  cout << ": ";

  switch (nodo->clave.tipo) {
  case TIPO_ENTERO:
    // cout << nodo->clave.valorEntero;
    break;
  case TIPO_FLOTANTE:
    // cout << nodo->clave.valorFloat;
    break;
  case TIPO_STRING:
    // cout << nodo->clave.valorString;
    break;
  }

  cout << " → IDs: ";
  for (unsigned int id : nodo->ids)
    cout << id << " ";
  cout << endl;

  inorder(nodo->derecho);
}

void AVL_Busqueda::inorder() { inorder(root); }

void AVL_Busqueda::setCampos(const vector<string> &_campos) {
  campos = _campos;
}

void AVL_Busqueda::setTipos(const vector<string> &_tipos) { tipos = _tipos; }
