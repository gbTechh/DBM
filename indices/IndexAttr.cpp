// IndexAttr.cpp
#include "IndexAttr.h"
#include <algorithm>
#include <iostream>
#include <variant>

using namespace std;

AVLNodeAttr::AVLNodeAttr(const Campo_Codificado &k, unsigned int id)
    : key(k), left(nullptr), right(nullptr), height(1) {
  ids.push_back(id);
}

IndexAttr::IndexAttr() : root(nullptr) {}

IndexAttr::~IndexAttr() { destroyTree(root); }

int IndexAttr::getHeight(AVLNodeAttr *node) { return node ? node->height : 0; }

void IndexAttr::updateHeight(AVLNodeAttr *node) {
  if (node)
    node->height = 1 + max(getHeight(node->left), getHeight(node->right));
}

int IndexAttr::getBalance(AVLNodeAttr *node) {
  return node ? getHeight(node->left) - getHeight(node->right) : 0;
}

AVLNodeAttr *IndexAttr::rotateRight(AVLNodeAttr *y) {
  AVLNodeAttr *x = y->left;
  AVLNodeAttr *T2 = x->right;
  x->right = y;
  y->left = T2;
  updateHeight(y);
  updateHeight(x);
  return x;
}

AVLNodeAttr *IndexAttr::rotateLeft(AVLNodeAttr *x) {
  AVLNodeAttr *y = x->right;
  AVLNodeAttr *T2 = y->left;
  y->left = x;
  x->right = T2;
  updateHeight(x);
  updateHeight(y);
  return y;
}

AVLNodeAttr *IndexAttr::insertHelper(AVLNodeAttr *node,
                                     const Campo_Codificado &key,
                                     unsigned int id) {
  if (!node)
    return new AVLNodeAttr(key, id);

  if (key == node->key) {
    node->ids.push_back(id);
    sort(node->ids.begin(), node->ids.end());
    return node;
  }

  if (key < node->key)
    node->left = insertHelper(node->left, key, id);
  else
    node->right = insertHelper(node->right, key, id);

  updateHeight(node);
  int balance = getBalance(node);

  if (balance > 1 && key < node->left->key)
    return rotateRight(node);
  if (balance < -1 && key > node->right->key)
    return rotateLeft(node);
  if (balance > 1 && key > node->left->key) {
    node->left = rotateLeft(node->left);
    return rotateRight(node);
  }
  if (balance < -1 && key < node->right->key) {
    node->right = rotateRight(node->right);
    return rotateLeft(node);
  }

  return node;
}

AVLNodeAttr *IndexAttr::deleteHelper(AVLNodeAttr *node,
                                     const Campo_Codificado &key) {
  if (!node)
    return nullptr;

  if (key < node->key) {
    node->left = deleteHelper(node->left, key);
  } else if (key > node->key) {
    node->right = deleteHelper(node->right, key);
  } else {
    if (!node->left || !node->right) {
      AVLNodeAttr *temp = node->left ? node->left : node->right;
      delete node;
      return temp;
    } else {
      AVLNodeAttr *temp = findMin(node->right);
      node->key = temp->key;
      node->ids = temp->ids;
      node->right = deleteHelper(node->right, temp->key);
    }
  }

  updateHeight(node);
  int balance = getBalance(node);

  if (balance > 1 && getBalance(node->left) >= 0)
    return rotateRight(node);
  if (balance > 1 && getBalance(node->left) < 0) {
    node->left = rotateLeft(node->left);
    return rotateRight(node);
  }
  if (balance < -1 && getBalance(node->right) <= 0)
    return rotateLeft(node);
  if (balance < -1 && getBalance(node->right) > 0) {
    node->right = rotateRight(node->right);
    return rotateLeft(node);
  }

  return node;
}

AVLNodeAttr *IndexAttr::findMin(AVLNodeAttr *node) {
  while (node->left)
    node = node->left;
  return node;
}

void IndexAttr::destroyTree(AVLNodeAttr *node) {
  if (node) {
    destroyTree(node->left);
    destroyTree(node->right);
    delete node;
  }
}

void IndexAttr::insert(const Campo_Codificado &key, unsigned int id) {
  root = insertHelper(root, key, id);
}

void IndexAttr::remove(const Campo_Codificado &key) {
  root = deleteHelper(root, key);
}

std::vector<unsigned int> IndexAttr::find(const Campo_Codificado &key) {
  AVLNodeAttr *current = root;
  while (current) {
    if (key == current->key)
      return current->ids;
    current = (key < current->key) ? current->left : current->right;
  }
  return {};
}

void IndexAttr::inorderHelper(AVLNodeAttr *node) {
  if (!node)
    return;
  inorderHelper(node->left);
  cout << node->key.campo << ": ";
  switch (node->key.tipo) {
  case TIPO_ENTERO:
    if (std::holds_alternative<unsigned long long>(node->key.valor))
      cout << std::get<unsigned long long>(node->key.valor);
    else
      cout << "[Error: tipo esperado int]";
    break;
  case TIPO_FLOTANTE:
    if (std::holds_alternative<float>(node->key.valor))
      cout << std::get<float>(node->key.valor);
    else
      cout << "[Error: tipo esperado float]";
    break;
  case TIPO_STRING:
    if (std::holds_alternative<std::string>(node->key.valor))
      cout << std::get<std::string>(node->key.valor);
    else
      cout << "[Error: tipo esperado string]";
    break;
  }
  cout << " → IDs: ";
  for (auto id : node->ids)
    cout << id << " ";
  cout << endl;
  inorderHelper(node->right);
}

void IndexAttr::inorder() { inorderHelper(root); }

void IndexAttr::setCampos(const std::vector<std::string> &_campos) {
  campos = _campos;
}

void IndexAttr::setTipos(const std::vector<std::string> &_tipos) {
  tipos = _tipos;
}

std::vector<unsigned int> IndexAttr::buscar(const std::string &campo,
                                            const std::string &valor) {
  size_t campoIndex = 0;
  for (size_t i = 0; i < campos.size(); ++i) {
    if (campos[i] == campo) {
      campoIndex = i;
      break;
    }
  }
  if (campoIndex >= tipos.size())
    return {};

  std::string tipo = tipos[campoIndex];
  Campo_Codificado clave;
  clave.campo = static_cast<unsigned int>(campoIndex);

  if (tipo == "int") {
    clave.tipo = TIPO_ENTERO;
    clave.valor = std::stoull(valor);
  } else if (tipo == "float" || tipo == "decimal" || tipo == "double") {
    clave.tipo = TIPO_FLOTANTE;
    clave.valor = stof(valor);
  } else {
    clave.tipo = TIPO_STRING;
    clave.valor = valor;
  }

  return find(clave);
}

void IndexAttr::buscarRangoHelper(AVLNodeAttr *node, unsigned int columnaIndex,
                                  const std::string &operador,
                                  const std::string &valor,
                                  std::vector<unsigned int> &resultados) {
  if (!node)
    return;

  // 🔒 Solo comparar si el campo coincide
  if (node->key.campo != columnaIndex) {
    buscarRangoHelper(node->left, columnaIndex, operador, valor, resultados);
    buscarRangoHelper(node->right, columnaIndex, operador, valor, resultados);
    return;
  }

  int cmp = node->key.comparar(valor); // <0 menor, ==0 igual, >0 mayor

  // Recorrer según operador
  if (operador == "=") {
    if (cmp == 0)
      resultados.insert(resultados.end(), node->ids.begin(), node->ids.end());

  } else if (operador == "<") {
    if (cmp < 0)
      resultados.insert(resultados.end(), node->ids.begin(), node->ids.end());

  } else if (operador == "<=") {
    if (cmp <= 0)
      resultados.insert(resultados.end(), node->ids.begin(), node->ids.end());

  } else if (operador == ">") {
    if (cmp > 0)
      resultados.insert(resultados.end(), node->ids.begin(), node->ids.end());

  } else if (operador == ">=") {
    if (cmp >= 0)
      resultados.insert(resultados.end(), node->ids.begin(), node->ids.end());
  }

  buscarRangoHelper(node->left, columnaIndex, operador, valor, resultados);
  buscarRangoHelper(node->right, columnaIndex, operador, valor, resultados);
}

std::vector<unsigned int> IndexAttr::buscarRango(const std::string &columna,
                                                 const std::string &operador,
                                                 const std::string &valor) {
  std::vector<unsigned int> resultados;

  // Convertir columna a índice
  unsigned int columnaIndex = 0;
  for (unsigned int i = 0; i < campos.size(); ++i) {
    if (campos[i] == columna) {
      columnaIndex = i;
      break;
    }
  }

  buscarRangoHelper(root, columnaIndex, operador, valor, resultados);
  return resultados;
}

void IndexAttr::imprimirArbol(AVLNodeAttr *nodo, int espacio, int nivel) {
  if (!nodo)
    return;
  espacio += nivel;
  imprimirArbol(nodo->right, espacio);
  cout << endl;
  for (int i = nivel; i < espacio; i++)
    cout << " ";
  cout << nodo->key.campo;
  imprimirArbol(nodo->left, espacio);
}

void IndexAttr::imprimir() {
  cout << "\nÁrbol AVL (vista rotada):\n";
  imprimirArbol(root);
  cout << endl;
}
