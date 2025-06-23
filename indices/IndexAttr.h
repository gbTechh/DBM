#ifndef INDEXATTR_H
#define INDEXATTR_H

#include <string>
#include <utility>

class AVLNodeAttr {
public:
  std::string value;
  AVLNodeAttr *left;
  AVLNodeAttr *right;
  int height;

  AVLNodeAttr(std::string atributos);
};

class IndexAttr {
private:
  AVLNodeAttr *root;

  // Métodos privados - solo declaraciones
  int getHeight(AVLNodeAttr *node);
  void updateHeight(AVLNodeAttr *node);
  int getBalance(AVLNodeAttr *node);
  AVLNodeAttr *rotateRight(AVLNodeAttr *y);
  AVLNodeAttr *rotateLeft(AVLNodeAttr *x);
  AVLNodeAttr *insertHelper(AVLNodeAttr *node, std::string column,
                            std::string attr);
  AVLNodeAttr *findMin(AVLNodeAttr *node);
  AVLNodeAttr *deleteHelper(AVLNodeAttr *node, std::string attr);
  void inorderHelper(AVLNodeAttr *node);
  void destroyTree(AVLNodeAttr *node);

public:
  IndexAttr();
  ~IndexAttr();

  void insert(std::string column, std::string attr);
  void remove(std::string attr);
  void inorder();
  bool find(std::string column, std::string &attr);
  void imprimirArbol(AVLNodeAttr *nodo, int espacio, int nivel);
  void imprimir();
};

#endif // INDEXATTR_H
