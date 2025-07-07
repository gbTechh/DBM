// IndexAttr.h
#ifndef INDEXATTR_H
#define INDEXATTR_H

#include "../utils/Enum.h"
#include "CampoCodificado.h"
#include <string>
#include <vector>

class AVLNodeAttr {
public:
  Campo_Codificado key;
  std::vector<unsigned int> ids;
  AVLNodeAttr *left, *right;
  int height;

  AVLNodeAttr(const Campo_Codificado &k, unsigned int id);
};

class IndexAttr {
private:
  AVLNodeAttr *root;
  std::vector<std::string> campos;
  std::vector<std::string> tipos;

  int getHeight(AVLNodeAttr *node);
  void updateHeight(AVLNodeAttr *node);
  int getBalance(AVLNodeAttr *node);
  AVLNodeAttr *rotateRight(AVLNodeAttr *y);
  AVLNodeAttr *rotateLeft(AVLNodeAttr *x);
  AVLNodeAttr *insertHelper(AVLNodeAttr *node, const Campo_Codificado &key,
                            unsigned int id);
  AVLNodeAttr *deleteHelper(AVLNodeAttr *node, const Campo_Codificado &key);
  AVLNodeAttr *findMin(AVLNodeAttr *node);
  void destroyTree(AVLNodeAttr *node);
  void inorderHelper(AVLNodeAttr *node);
  std::vector<unsigned int> searchHelper(AVLNodeAttr *node,
                                         const Campo_Codificado &key);
  void buscarRangoHelper(AVLNodeAttr *node, unsigned int columnaIndex,
                         const std::string &operador, const std::string &valor,
                         std::vector<unsigned int> &resultados);

public:
  IndexAttr();
  ~IndexAttr();

  void insert(const Campo_Codificado &key, unsigned int id);
  void remove(const Campo_Codificado &key);
  std::vector<unsigned int> find(const Campo_Codificado &key);
  void inorder();

  void setCampos(const std::vector<std::string> &_campos);
  void setTipos(const std::vector<std::string> &_tipos);

  std::vector<unsigned int> buscar(const std::string &campo,
                                   const std::string &valor);

  std::vector<unsigned int> buscarRango(const std::string &columna,
                                        const std::string &operador,
                                        const std::string &valor);
  void imprimirArbol(AVLNodeAttr *nodo, int espacio = 0, int nivel = 5);
  void imprimir();
};

#endif // INDEXATTR_H
