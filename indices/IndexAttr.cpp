#include "IndexAttr.h"
#include <algorithm>
#include <iostream>
#include <string>

// AQUÍ sí puedes usar using namespace std si quieres
using namespace std;

// Constructor de AVLNodeAttr
AVLNodeAttr::AVLNodeAttr(std::string attr)
    : value(attr), left(nullptr), right(nullptr), height(1) {}

// Constructor de IndexAttr
IndexAttr::IndexAttr() : root(nullptr) {}

// Destructor
IndexAttr::~IndexAttr() { destroyTree(root); }

// Implementación de métodos privados
int IndexAttr::getHeight(AVLNodeAttr *node) { return node ? node->height : 0; }

void IndexAttr::updateHeight(AVLNodeAttr *node) {
  if (node) {
    node->height = 1 + max(getHeight(node->left), getHeight(node->right));
  }
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

AVLNodeAttr *IndexAttr::insertHelper(AVLNodeAttr *node, std::string column,
                                     std::string atributo) {
  // Inserción normal de BST
  std::string attr = column + ":" + atributo;
  if (!node)
    return new AVLNodeAttr(attr);

  if (attr < node->value) {
    node->left = insertHelper(node->left, column, atributo);
  } else if (attr > node->value) {
    node->right = insertHelper(node->right, column, atributo);
  } else {
    return node; // Valor duplicado
  }

  // Actualizar altura
  updateHeight(node);

  // Obtener factor de balance
  int balance = getBalance(node);

  // Rotaciones
  if (balance > 1 && attr < node->left->value) {
    return rotateRight(node);
  }

  if (balance < -1 && attr > node->right->value) {
    return rotateLeft(node);
  }

  if (balance > 1 && attr > node->left->value) {
    node->left = rotateLeft(node->left);
    return rotateRight(node);
  }

  if (balance < -1 && attr < node->right->value) {
    node->right = rotateRight(node->right);
    return rotateLeft(node);
  }

  return node;
}

AVLNodeAttr *IndexAttr::findMin(AVLNodeAttr *node) {
  while (node->left) {
    node = node->left;
  }
  return node;
}

AVLNodeAttr *IndexAttr::deleteHelper(AVLNodeAttr *node, std::string value) {
  if (!node)
    return node;

  if (value < node->value) {
    node->left = deleteHelper(node->left, value);
  } else if (value > node->value) {
    node->right = deleteHelper(node->right, value);
  } else {
    if (!node->left || !node->right) {
      AVLNodeAttr *temp = node->left ? node->left : node->right;
      if (!temp) {
        temp = node;
        node = nullptr;
      } else {
        *node = *temp;
      }
      delete temp;
    } else {
      AVLNodeAttr *temp = findMin(node->right);
      node->value = temp->value;
      node->right = deleteHelper(node->right, temp->value);
    }
  }

  if (!node)
    return node;

  updateHeight(node);
  int balance = getBalance(node);

  if (balance > 1 && getBalance(node->left) >= 0) {
    return rotateRight(node);
  }

  if (balance > 1 && getBalance(node->left) < 0) {
    node->left = rotateLeft(node->left);
    return rotateRight(node);
  }

  if (balance < -1 && getBalance(node->right) <= 0) {
    return rotateLeft(node);
  }

  if (balance < -1 && getBalance(node->right) > 0) {
    node->right = rotateRight(node->right);
    return rotateLeft(node);
  }

  return node;
}

void IndexAttr::inorderHelper(AVLNodeAttr *node) {
  if (node) {
    inorderHelper(node->left);
    cout << node->value << " ";
    inorderHelper(node->right);
  }
}

void IndexAttr::destroyTree(AVLNodeAttr *node) {
  if (node) {
    destroyTree(node->left);
    destroyTree(node->right);
    delete node;
  }
}

// Implementación de métodos públicos
void IndexAttr::insert(std::string column, std::string attributes) {
  root = insertHelper(root, column, attributes);
}

void IndexAttr::remove(std::string value) { root = deleteHelper(root, value); }

void IndexAttr::inorder() {
  inorderHelper(root);
  cout << endl;
}

bool IndexAttr::find(std::string column, std::string &attr) {
  std::string value = column + ":" + attr;
  AVLNodeAttr *current = root;
  std::cout<<"Curren value 1: "<<current->value<<"\n";
  std::cout<<"value 1: "<<value<<"\n";
  while (current) {
    std::cout<<"Curren value: "<<current->value<<"\n";
    if (value == current->value) {
      value = current->value;
      return true;
    }
    current = (value < current->value) ? current->left : current->right;
  }
  return false;
}

void IndexAttr::imprimirArbol(AVLNodeAttr *nodo, int espacio = 0,
                              int nivel = 5) {
  if (nodo == nullptr)
    return;

  // Aumenta el espacio entre niveles
  espacio += nivel;

  // Imprime primero el subárbol derecho
  imprimirArbol(nodo->right, espacio);

  // Imprime el nodo actual después de espacios
  std::cout << std::endl;
  for (int i = nivel; i < espacio; i++)
    std::cout << " ";
  std::cout << nodo->value;

  // Luego el subárbol izquierdo
  imprimirArbol(nodo->left, espacio);
}
void IndexAttr::imprimir() {
  std::cout << "\nÁrbol AVL (vista rotada):\n";
  imprimirArbol(root);
  std::cout << "\n";
}
