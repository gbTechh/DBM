#ifndef INDEXID_H
#define INDEXID_H

#include <string>
#include <utility>

class AVLNode {
public:
  std::pair<int, std::string> value;
  AVLNode *left;
  AVLNode *right;
  int height;

  AVLNode(int id, std::string atributos);
};

class IndexID {
private:
  AVLNode *root;

  // Métodos privados - solo declaraciones
  int getHeight(AVLNode *node);
  void updateHeight(AVLNode *node);
  int getBalance(AVLNode *node);
  AVLNode *rotateRight(AVLNode *y);
  AVLNode *rotateLeft(AVLNode *x);
  AVLNode *insertHelper(AVLNode *node, int id, std::string atributos);
  AVLNode *findMin(AVLNode *node);
  AVLNode *deleteHelper(AVLNode *node, int value);
  void inorderHelper(AVLNode *node);
  void destroyTree(AVLNode *node);

public:
  IndexID();
  ~IndexID();

  void insert(int value, std::string attributes);
  void remove(int value);
  void inorder();
  bool find(int value, std::pair<int, std::string> &registro);
  void imprimirArbol(AVLNode *nodo, int espacio, int nivel);
  void imprimir();
};

#endif // INDEXID_H
