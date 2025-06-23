#include <algorithm>
#include <iostream>

using namespace std;

class AVLNode {
public:
  int value;
  AVLNode *left;
  AVLNode *right;
  int height;

  AVLNode(int val) : value(val), left(nullptr), right(nullptr), height(1) {}
};

class AVLTree {
private:
  AVLNode *root;

  // Obtener altura de un nodo
  int getHeight(AVLNode *node) { return node ? node->height : 0; }

  // Actualizar altura de un nodo
  void updateHeight(AVLNode *node) {
    if (node) {
      node->height = 1 + max(getHeight(node->left), getHeight(node->right));
    }
  }

  // Calcular factor de balance
  int getBalance(AVLNode *node) {
    return node ? getHeight(node->left) - getHeight(node->right) : 0;
  }

  // Rotación a la derecha
  AVLNode *rotateRight(AVLNode *y) {
    AVLNode *x = y->left;
    AVLNode *T2 = x->right;

    // Realizar rotación
    x->right = y;
    y->left = T2;

    // Actualizar alturas
    updateHeight(y);
    updateHeight(x);

    return x; // Nueva raíz
  }

  // Rotación a la izquierda
  AVLNode *rotateLeft(AVLNode *x) {
    AVLNode *y = x->right;
    AVLNode *T2 = y->left;

    // Realizar rotación
    y->left = x;
    x->right = T2;

    // Actualizar alturas
    updateHeight(x);
    updateHeight(y);

    return y; // Nueva raíz
  }

  // Insertar recursivamente
  AVLNode *insertHelper(AVLNode *node, int value) {
    // 1. Inserción normal de BST
    if (!node)
      return new AVLNode(value);

    if (value < node->value) {
      node->left = insertHelper(node->left, value);
    } else if (value > node->value) {
      node->right = insertHelper(node->right, value);
    } else {
      return node; // Valor duplicado, no insertar
    }

    // 2. Actualizar altura
    updateHeight(node);

    // 3. Obtener factor de balance
    int balance = getBalance(node);

    // 4. Realizar rotaciones si es necesario
    // Caso Left Left
    if (balance > 1 && value < node->left->value) {
      return rotateRight(node);
    }

    // Caso Right Right
    if (balance < -1 && value > node->right->value) {
      return rotateLeft(node);
    }

    // Caso Left Right
    if (balance > 1 && value > node->left->value) {
      node->left = rotateLeft(node->left);
      return rotateRight(node);
    }

    // Caso Right Left
    if (balance < -1 && value < node->right->value) {
      node->right = rotateRight(node->right);
      return rotateLeft(node);
    }

    return node;
  }

  // Encontrar el nodo con valor mínimo
  AVLNode *findMin(AVLNode *node) {
    while (node->left) {
      node = node->left;
    }
    return node;
  }

  // Eliminar recursivamente
  AVLNode *deleteHelper(AVLNode *node, int value) {
    // 1. Eliminación normal de BST
    if (!node)
      return node;

    if (value < node->value) {
      node->left = deleteHelper(node->left, value);
    } else if (value > node->value) {
      node->right = deleteHelper(node->right, value);
    } else {
      // Nodo a eliminar encontrado
      if (!node->left || !node->right) {
        // Caso con 0 o 1 hijo
        AVLNode *temp = node->left ? node->left : node->right;
        if (!temp) {
          // Sin hijos
          temp = node;
          node = nullptr;
        } else {
          // Un hijo
          *node = *temp;
        }
        delete temp;
      } else {
        // Caso con 2 hijos
        AVLNode *temp = findMin(node->right);
        node->value = temp->value;
        node->right = deleteHelper(node->right, temp->value);
      }
    }

    if (!node)
      return node;

    // 2. Actualizar altura
    updateHeight(node);

    // 3. Obtener factor de balance
    int balance = getBalance(node);

    // 4. Realizar rotaciones si es necesario
    // Caso Left Left
    if (balance > 1 && getBalance(node->left) >= 0) {
      return rotateRight(node);
    }

    // Caso Left Right
    if (balance > 1 && getBalance(node->left) < 0) {
      node->left = rotateLeft(node->left);
      return rotateRight(node);
    }

    // Caso Right Right
    if (balance < -1 && getBalance(node->right) <= 0) {
      return rotateLeft(node);
    }

    // Caso Right Left
    if (balance < -1 && getBalance(node->right) > 0) {
      node->right = rotateRight(node->right);
      return rotateLeft(node);
    }

    return node;
  }

  // Recorrido inorder recursivo
  void inorderHelper(AVLNode *node) {
    if (node) {
      inorderHelper(node->left);
      cout << node->value << " ";
      inorderHelper(node->right);
    }
  }

  // Destructor recursivo
  void destroyTree(AVLNode *node) {
    if (node) {
      destroyTree(node->left);
      destroyTree(node->right);
      delete node;
    }
  }

public:
  AVLTree() : root(nullptr) {}

  ~AVLTree() { destroyTree(root); }

  void insert(int value) { root = insertHelper(root, value); }

  void remove(int value) { root = deleteHelper(root, value); }

  void inorder() {
    inorderHelper(root);
    cout << endl;
  }

  bool find(int value) {
    AVLNode *current = root;
    while (current) {
      if (value == current->value)
        return true;
      current = (value < current->value) ? current->left : current->right;
    }
    return false;
  }
};
