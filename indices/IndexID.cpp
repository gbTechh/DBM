#include "IndexID.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

// Constructor de AVLNode
AVLNode::AVLNode(int id, std::vector<std::string> atributos, int inicio_,
                 int final_)
    : value(std::make_pair(id, atributos)), byteInicio(inicio_),
      byteFinal(final_), left(nullptr), right(nullptr), height(1) {}

// Constructor de IndexID
IndexID::IndexID() : root(nullptr) {}

// Destructor
IndexID::~IndexID() { destroyTree(root); }

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

AVLNode *IndexID::insertHelper(AVLNode *node, unsigned int id,
                               vector<string> atributos, int inicio,
                               int final) {
  if (!node)
    return new AVLNode(id, atributos, inicio, final);

  if (id < node->value.first) {
    node->left = insertHelper(node->left, id, atributos, inicio, final);
  } else if (id > node->value.first) {
    node->right = insertHelper(node->right, id, atributos, inicio, final);
  } else {
    // ID duplicado → ignorar (no sobreescribir)
    return node;
  }

  updateHeight(node);
  int balance = getBalance(node);

  if (balance > 1 && id < node->left->value.first)
    return rotateRight(node);
  if (balance < -1 && id > node->right->value.first)
    return rotateLeft(node);
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
  while (node->left)
    node = node->left;
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
      node->value.second = temp->value.second;
      node->right = deleteHelper(node->right, temp->value.first);
    }
  }

  if (!node)
    return node;

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

// =============== MÉTODOS PÚBLICOS =====================

void IndexID::insert(unsigned int value, vector<string> attributes, int inicio,
                     int final) {
  root = insertHelper(root, value, attributes, inicio, final);
}

void IndexID::remove(int value) { root = deleteHelper(root, value); }

void IndexID::inorder() {
  inorderHelper(root);
  cout << endl;
}

bool IndexID::find(unsigned int value,
                   pair<int, std::vector<std::string>> &registro, int &in,
                   int &fin) {
  AVLNode *current = root;
  while (current) {
    if (value == current->value.first) {
      registro.first = current->value.first;
      // Concatenar atributos con separador (ej: espacio)
      registro.second = current->value.second;
      in = current->byteInicio;
      fin = current->byteFinal;
      return true;
    }
    current = (value < current->value.first) ? current->left : current->right;
  }
  return false;
}

void IndexID::imprimirArbol(AVLNode *nodo, int espacio, int nivel = 5) {
  if (!nodo)
    return;

  espacio += nivel;
  imprimirArbol(nodo->right, espacio);

  std::cout << std::endl;
  for (int i = nivel; i < espacio; i++)
    std::cout << " ";
  std::cout << nodo->value.first << " ";
  for (const auto &attr : nodo->value.second)
    std::cout << attr << " ";

  imprimirArbol(nodo->left, espacio);
}

void IndexID::imprimir() {
  std::cout << "\nÁrbol AVL (vista rotada):\n";
  imprimirArbol(root, 0, 5);
  std::cout << "\n";
}