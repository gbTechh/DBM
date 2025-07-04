#include "IndexID.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <utility>

// AQUÍ sí puedes usar using namespace std si quieres
using namespace std;

// Constructor de AVLNode
AVLNode::AVLNode(int id, std::string atributos)
    : value(std::make_pair(id, atributos)), left(nullptr), right(nullptr),
      height(1) {}

// Constructor de IndexID
IndexID::IndexID() : root(nullptr) {}

// Destructor
IndexID::~IndexID() { destroyTree(root); }

// Implementación de métodos privados
int IndexID::getHeight(AVLNode *node) { return node ? node->height : 0; }

void IndexID::updateHeight(AVLNode *node) {
  if (node) {
    node->height = 1 + max(getHeight(node->left), getHeight(node->right));
  }
}

int IndexID::getBalance(AVLNode *node) {
  return node ? getHeight(node->left) - getHeight(node->right) : 0;
}

AVLNode *IndexID::rotateRight(AVLNode *y) {
  AVLNode *x = y->left;
  AVLNode *T2 = x->right;

  x->right = y;
  y->left = T2;

  updateHeight(y);
  updateHeight(x);

  return x;
}

AVLNode *IndexID::rotateLeft(AVLNode *x) {
  AVLNode *y = x->right;
  AVLNode *T2 = y->left;

  y->left = x;
  x->right = T2;

  updateHeight(x);
  updateHeight(y);

  return y;
}

AVLNode *IndexID::insertHelper(AVLNode *node, int id, std::string attributes) {
  // Inserción normal de BST
  if (!node)
    return new AVLNode(id, attributes);

  if (id < node->value.first) {
    node->left = insertHelper(node->left, id, attributes);
  } else if (id > node->value.first) {
    node->right = insertHelper(node->right, id, attributes);
  } else {
    node->value.second = attributes;
    return node; // Valor duplicado
  }

  // Actualizar altura
  updateHeight(node);

  // Obtener factor de balance
  int balance = getBalance(node);

  // Rotaciones
  if (balance > 1 && id < node->left->value.first) {
    return rotateRight(node);
  }

  if (balance < -1 && id > node->right->value.first) {
    return rotateLeft(node);
  }

  if (balance > 1 && id > node->left->value.first) {
    node->left = rotateLeft(node->left);
    return rotateRight(node);
  }

  if (balance < -1 && id < node->right->value.first) {
    node->right = rotateRight(node->right);
    return rotateLeft(node);
  }

  return node;
}

AVLNode *IndexID::findMin(AVLNode *node) {
  while (node->left) {
    node = node->left;
  }
  return node;
}

AVLNode *IndexID::deleteHelper(AVLNode *node, int value) {
  if (!node)
    return node;

  if (value < node->value.first) {
    node->left = deleteHelper(node->left, value);
  } else if (value > node->value.first) {
    node->right = deleteHelper(node->right, value);
  } else {
    if (!node->left || !node->right) {
      AVLNode *temp = node->left ? node->left : node->right;
      if (!temp) {
        temp = node;
        node = nullptr;
      } else {
        *node = *temp;
      }
      delete temp;
    } else {
      AVLNode *temp = findMin(node->right);
      node->value.first = temp->value.first;
      node->right = deleteHelper(node->right, temp->value.first);
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

void IndexID::inorderHelper(AVLNode *node) {
  if (node) {
    inorderHelper(node->left);
    cout << node->value.first << " ";
    inorderHelper(node->right);
  }
}

void IndexID::destroyTree(AVLNode *node) {
  if (node) {
    destroyTree(node->left);
    destroyTree(node->right);
    delete node;
  }
}

// Implementación de métodos públicos
void IndexID::insert(int value, std::string attributes) {
  root = insertHelper(root, value, attributes);
}

void IndexID::remove(int value) { root = deleteHelper(root, value); }

void IndexID::inorder() {
  inorderHelper(root);
  cout << endl;
}

bool IndexID::find(int value, std::pair<int, std::string> &registro) {
  AVLNode *current = root;
  while (current) {
    if (value == current->value.first) {
      registro = current->value;
      return true;
    }
    current = (value < current->value.first) ? current->left : current->right;
  }
  return false;
}

void IndexID::imprimirArbol(AVLNode *nodo, int espacio = 0, int nivel = 5) {
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
  std::cout << nodo->value.first << " " << nodo->value.second;

  // Luego el subárbol izquierdo
  imprimirArbol(nodo->left, espacio);
}
void IndexID::imprimir() {
  std::cout << "\nÁrbol AVL (vista rotada):\n";
  imprimirArbol(root);
  std::cout << "\n";
}