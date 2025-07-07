#ifndef INDEXID_H
#define INDEXID_H

#include <string>
#include <utility>
#include <vector>

class AVLNode {
public:
  std::pair<unsigned int, std::vector<std::string>> value;
  AVLNode *left;
  AVLNode *right;
  int height;
  int byteInicio, byteFinal;

  AVLNode(int id, std::vector<std::string> atributos, int inicio, int final);
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
  AVLNode *insertHelper(AVLNode *node, unsigned int id,
                        std::vector<std::string> atributos, int inicio,
                        int final);
  AVLNode *findMin(AVLNode *node);
  AVLNode *deleteHelper(AVLNode *node, int value);
  void inorderHelper(AVLNode *node);
  void destroyTree(AVLNode *node);

public:
  IndexID();
  ~IndexID();

  void insert(unsigned int value, std::vector<std::string> attributes,
              int inicio, int final);
  void remove(int value);
  void inorder();
  bool find(unsigned int value,
            std::pair<int, std::vector<std::string>> &registro, int &in,
            int &fin);
  void imprimirArbol(AVLNode *nodo, int espacio, int nivel);
  void imprimir();
};

#endif // INDEXID_H
