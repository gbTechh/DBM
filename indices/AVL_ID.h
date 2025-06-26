#ifndef AVL_ID_H
#define AVL_ID_H

#include <string>
#include <vector>

class CNode {
public:
  unsigned int id;
  int height;
  CNode *node[2];
  std::vector<std::string> registro;
  long int byte_inicial;
  long int byte_fin;
  std::vector<std::pair<long int, long int>> particion_dirs;

  CNode(std::vector<std::string> reg, long int dir, long int dir2,
        std::vector<std::pair<long int, long int>> p_dir);
};

class AVL_ID {
public:
  CNode *root;
  std::vector<CNode **> path;

  AVL_ID();
  bool insert(std::vector<std::string> reg, long int dir = 0, long int dir2 = 0,
              std::vector<std::pair<long int, long int>> p_dir = {{0, 0}});
  bool evaluando(CNode **current);
  CNode **rep(CNode **p);
  void inorder(CNode *p);
  CNode *Root();
  bool find(CNode **&p, unsigned int v);
  int height(CNode *p);
  int factor_balance(CNode *p);
  void rotaRight(CNode **p);
  void rotaLeft(CNode **p);
};

#endif // AVL_ID_H