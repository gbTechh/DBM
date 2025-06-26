#include "AVL_Busqueda.h"
#include <algorithm>
#include <cstring>
#include <functional>
#include <iostream>

using namespace std;

bool Campo_Codificado::operator<(const Campo_Codificado &otra) const {
  if (campo != otra.campo)
    return campo < otra.campo;
  if (tipo != otra.tipo)
    return tipo < otra.tipo;

  switch (tipo) {
  case TIPO_ENTERO:
    return valorEntero < otra.valorEntero;
  case TIPO_FLOTANTE:
    return valorFloat < otra.valorFloat;
  case TIPO_STRING:
    return strcmp(valorString, otra.valorString) < 0;
  default:
    return false;
  }
}

bool Campo_Codificado::operator>(const Campo_Codificado &otra) const {
  return !(*this < otra);
}

bool Campo_Codificado::operator==(const Campo_Codificado &otra) const {
  if (campo != otra.campo || tipo != otra.tipo)
    return false;
  switch (tipo) {
  case TIPO_ENTERO:
    return valorEntero == otra.valorEntero;
  case TIPO_FLOTANTE:
    return valorFloat == otra.valorFloat;
  case TIPO_STRING:
    return strcmp(valorString, otra.valorString) == 0;
  default:
    return false;
  }
}

NodoAVL::NodoAVL(const Campo_Codificado &k, unsigned int id)
    : clave(k), izquierdo(nullptr), derecho(nullptr), altura(1) {
  ids.push_back(id);
}

AVL_Busqueda::AVL_Busqueda() : root(nullptr) {}

int AVL_Busqueda::altura(NodoAVL *nodo) { return nodo ? nodo->altura : 0; }

int AVL_Busqueda::balance(NodoAVL *nodo) {
  return nodo ? altura(nodo->izquierdo) - altura(nodo->derecho) : 0;
}

void AVL_Busqueda::actualizarAltura(NodoAVL *nodo) {
  nodo->altura = 1 + max(altura(nodo->izquierdo), altura(nodo->derecho));
}

NodoAVL *AVL_Busqueda::rotarDerecha(NodoAVL *y) {
  NodoAVL *x = y->izquierdo;
  NodoAVL *T2 = x->derecho;
  x->derecho = y;
  y->izquierdo = T2;
  actualizarAltura(y);
  actualizarAltura(x);
  return x;
}

NodoAVL *AVL_Busqueda::rotarIzquierda(NodoAVL *x) {
  NodoAVL *y = x->derecho;
  NodoAVL *T2 = y->izquierdo;
  y->izquierdo = x;
  x->derecho = T2;
  actualizarAltura(x);
  actualizarAltura(y);
  return y;
}

NodoAVL *AVL_Busqueda::insertar(NodoAVL *nodo, const Campo_Codificado &clave,
                                unsigned int id) {
  if (!nodo)
    return new NodoAVL(clave, id);

  if (clave == nodo->clave) {
    nodo->ids.push_back(id);
    if (nodo->ids.size() > 1) {
      if (nodo->ids[nodo->ids.size() - 1] < nodo->ids[nodo->ids.size() - 2])
        sort(nodo->ids.begin(), nodo->ids.end());
    }
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

  if (bal < -1 && nodo->derecho && clave > nodo->derecho->clave)
    return rotarIzquierda(nodo);

  if (bal > 1 && nodo->izquierdo && clave > nodo->izquierdo->clave) {
    nodo->izquierdo = rotarIzquierda(nodo->izquierdo);
    return rotarDerecha(nodo);
  }
  if (bal < -1 && nodo->derecho && clave < nodo->derecho->clave) {
    nodo->derecho = rotarDerecha(nodo->derecho);
    return rotarIzquierda(nodo);
  }
  return nodo;
}

void AVL_Busqueda::insertar(const Campo_Codificado &clave, unsigned int id) {
  root = insertar(root, clave, id);
}

void AVL_Busqueda::inorder(NodoAVL *nodo) {
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
    cout << nodo->clave.valorEntero;
    break;
  case TIPO_FLOTANTE:
    cout << nodo->clave.valorFloat;
    break;
  case TIPO_STRING:
    cout << nodo->clave.valorString;
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

vector<unsigned int> AVL_Busqueda::buscar(const string &campoNombre,
                                          const string &valor) {
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

  string tipo = tipos[campoIndex];
  if (tipo == "int") {
    clave.tipo = TIPO_ENTERO;
    clave.valorEntero = stoi(valor);
  } else if (tipo == "float") {
    clave.tipo = TIPO_FLOTANTE;
    clave.valorFloat = stof(valor);
  } else {
    clave.tipo = TIPO_STRING;
    clave.valorString = valor.c_str();
  }

  return busquedaInterna(root, clave);
}

vector<unsigned int>
AVL_Busqueda::busquedaInterna(NodoAVL *nodo, const Campo_Codificado &clave) {
  if (!nodo)
    return {};

  if (clave == nodo->clave)
    return nodo->ids;
  else if (clave < nodo->clave)
    return busquedaInterna(nodo->izquierdo, clave);
  else
    return busquedaInterna(nodo->derecho, clave);
}

vector<unsigned int> AVL_Busqueda::buscarRango(const string &campo,
                                               const string &operador,
                                               const string &valor) {
  vector<unsigned int> res;
  size_t campoIndex = 0;
  for (size_t i = 0; i < campos.size(); ++i) {
    if (campos[i] == campo) {
      campoIndex = i;
      break;
    }
  }
  if (campoIndex >= campos.size())
    return res;

  function<void(NodoAVL *)> recorrer = [&](NodoAVL *nodo) {
    if (!nodo)
      return;
    recorrer(nodo->izquierdo);
    if (nodo->clave.campo == campoIndex) {
      bool cumple = false;
      if (nodo->clave.tipo == TIPO_ENTERO) {
        int cmp = nodo->clave.valorEntero;
        int v = stoi(valor);
        cumple = (operador == "<" && cmp < v) || (operador == ">" && cmp > v) ||
                 (operador == "<=" && cmp <= v) ||
                 (operador == ">=" && cmp >= v);
      } else if (nodo->clave.tipo == TIPO_FLOTANTE) {
        float cmp = nodo->clave.valorFloat;
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