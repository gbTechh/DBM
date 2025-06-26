#ifndef AVL_BUSQUEDA_H
#define AVL_BUSQUEDA_H

#include <string>
#include <vector>

enum TipoDato { TIPO_ENTERO, TIPO_FLOTANTE, TIPO_STRING };

struct Campo_Codificado {
  unsigned int campo; // Nombre del atributo codificado
  TipoDato tipo;      // Tipo del valor del atributo
  union {
    int valorEntero;
    float valorFloat;
    const char *valorString;
  };

  bool operator<(const Campo_Codificado &otra) const;
  bool operator>(const Campo_Codificado &otra) const;
  bool operator==(const Campo_Codificado &otra) const;
};

struct NodoAVL {
  Campo_Codificado clave;
  std::vector<unsigned int> ids;
  NodoAVL *izquierdo;
  NodoAVL *derecho;
  int altura;

  NodoAVL(const Campo_Codificado &k, unsigned int id);
};

class AVL_Busqueda {
private:
  NodoAVL *root;
  int altura(NodoAVL *nodo);
  int balance(NodoAVL *nodo);
  void actualizarAltura(NodoAVL *nodo);
  NodoAVL *rotarDerecha(NodoAVL *y);
  NodoAVL *rotarIzquierda(NodoAVL *x);
  void inorder(NodoAVL *nodo);
  std::vector<unsigned int> busquedaInterna(NodoAVL *nodo,
                                            const Campo_Codificado &clave);
  NodoAVL *insertar(NodoAVL *nodo, const Campo_Codificado &clave,
                    unsigned int id);

public:
  AVL_Busqueda();
  std::vector<std::string> campos, tipos;
  void insertar(const Campo_Codificado &clave, unsigned int id);
  void inorder();
  void setCampos(const std::vector<std::string> &_campos);
  void setTipos(const std::vector<std::string> &_tipos);
  std::vector<unsigned int> buscar(const std::string &campoNombre,
                                   const std::string &valor);
  std::vector<unsigned int> buscarRango(const std::string &campo,
                                        const std::string &operador,
                                        const std::string &valor);
};

#endif // AVL_BUSQUEDA_H