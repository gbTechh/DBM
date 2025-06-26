#include "AVL_ID.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

CNode::CNode(vector<string> reg, long int dir, long int dir2,
             vector<pair<long int, long int>> p_dir)
    : id(stoi(reg[0])), height(1), registro(reg), byte_inicial(dir),
      byte_fin(dir2), particion_dirs(p_dir) {
  node[0] = nullptr;
  node[1] = nullptr;
}

AVL_ID::AVL_ID() : root(nullptr) {}

CNode *AVL_ID::Root() { return root; }

void AVL_ID::inorder(CNode *n) {
  if (n == nullptr)
    return;

  inorder(n->node[0]);
  cout << n->id << " ";
  inorder(n->node[1]);
}

bool AVL_ID::find(CNode **&p, unsigned int v) {
  p = &root;
  path.clear();
  while (*p && (*p)->id != v) {
    path.push_back(p);
    p = &((*p)->node[v > (*p)->id]);
  }
  return *p != nullptr;
}

void AVL_ID::rotaRight(CNode **p) {
  CNode *C = *p;
  CNode *B = C->node[0];
  CNode *br = B->node[1] ? B->node[1] : nullptr;

  *p = B;
  C->node[0] = br;
  B->node[1] = C;

  height(C);
  height(B);
}

void AVL_ID::rotaLeft(CNode **p) {
  CNode *A = *p;
  CNode *B = A->node[1];
  CNode *bl = B->node[0] ? B->node[0] : nullptr;

  *p = B;
  A->node[1] = bl;
  B->node[0] = A;

  height(A);
  height(B);
}

bool AVL_ID::evaluando(CNode **current) {
  int balance = factor_balance(*current);

  if (balance == -2) {
    int hijo_izquierdo = factor_balance((*current)->node[0]);
    if (hijo_izquierdo <= 0) {
      rotaRight(current);
    } else {
      rotaLeft(&((*current)->node[0]));
      rotaRight(current);
    }
    return true;
  }
  if (balance == 2) {
    int hijo_derecho = factor_balance((*current)->node[1]);
    if (hijo_derecho >= 0) {
      rotaLeft(current);
    } else {
      rotaRight(&((*current)->node[1]));
      rotaLeft(current);
    }
    return true;
  }
  return false;
}

bool AVL_ID::insert(vector<string> reg, long int dir, long int dir2,
                    vector<pair<long int, long int>> p_dir) {
  path.clear();
  CNode **pos;
  if (reg.empty())
    return false;
  if (find(pos, stoi(reg[0])))
    return false;
  *pos = new CNode(reg, dir, dir2, p_dir);

  for (size_t i = path.size(); i > 0; --i) {
    CNode **current = path[i - 1];
    height(*current);
    if (evaluando(current))
      break;
  }
  return true;
}

int AVL_ID::height(CNode *p) {
  if (!p)
    return 0;
  int l = height(p->node[0]);
  int r = height(p->node[1]);

  p->height = 1 + (l > r ? l : r);
  return p->height;
}

int AVL_ID::factor_balance(CNode *p) {
  if (!p)
    return 0;

  int leftHeight = p->node[0] ? p->node[0]->height : 0;
  int rightHeight = p->node[1] ? p->node[1]->height : 0;
  return rightHeight - leftHeight;
}

CNode **AVL_ID::rep(CNode **p) {
  // Implementación vacía en el código original, se mantiene como tal
  return p;
}