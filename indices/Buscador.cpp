#include "Buscador.h"
#include <algorithm>
#include <cstring>
#include <iostream>
#include <limits>

using namespace std;

Buscador::Buscador() {}

Buscador::Buscador(vector<string> &campos, vector<string> &tipoDato,
                   vector<vector<string>> &registros) {
  arbol_busqueda.setCampos(campos);
  arbol_busqueda.setTipos(tipoDato);
  if (arbol_busqueda.campos.size() != arbol_busqueda.tipos.size())
    throw invalid_argument("Error en tamaño de vectores :( \n");

  for (size_t i = 0; i < registros.size(); ++i) {
    if (!arbol_id.insert(registros[i]))
      throw invalid_argument("Error en insercion de AVL_ID :( \n");
    for (size_t j = 1; j < registros[i].size(); ++j) {
      if (j >= arbol_busqueda.tipos.size())
        throw invalid_argument("Error! :( \n");
      Campo_Codificado clave;
      clave.campo = static_cast<unsigned int>(j);
      string tipo = arbol_busqueda.tipos[j];
      string valor = registros[i][j];

      if (tipo == "int") {
        clave.tipo = TIPO_ENTERO;
        clave.valorEntero = stoi(valor);
      } else if (tipo == "float") {
        clave.tipo = TIPO_FLOTANTE;
        clave.valorFloat = stof(valor);
      } else {
        clave.tipo = TIPO_STRING;
        clave.valorString = strdup(valor.c_str());
        if (!clave.valorString)
          throw invalid_argument("Error! :(");
      }

      unsigned long raw_id = stoul(registros[i][0]);
      if (raw_id > numeric_limits<unsigned int>::max())
        throw invalid_argument("Error! :(");
      unsigned int id = static_cast<unsigned int>(raw_id);
      arbol_busqueda.insertar(clave, id);
    }
  }
}

bool Buscador::insertReg(vector<string> registro) {
  if (!arbol_id.insert(registro))
    return false;
  for (size_t j = 1; j < registro.size(); ++j) {
    if (j >= arbol_busqueda.tipos.size())
      return false;
    Campo_Codificado clave;
    clave.campo = static_cast<unsigned int>(j);
    string tipo = arbol_busqueda.tipos[j];
    string valor = registro[j];

    if (tipo == "int") {
      clave.tipo = TIPO_ENTERO;
      clave.valorEntero = stoi(valor);
    } else if (tipo == "float") {
      clave.tipo = TIPO_FLOTANTE;
      clave.valorFloat = stof(valor);
    } else {
      clave.tipo = TIPO_STRING;
      clave.valorString = strdup(valor.c_str());
      if (!clave.valorString)
        return false;
    }

    unsigned long raw_id = stoul(registro[0]);
    if (raw_id > numeric_limits<unsigned int>::max())
      return false;
    unsigned int id = static_cast<unsigned int>(raw_id);
    arbol_busqueda.insertar(clave, id);
  }
  return true;
}

vector<unsigned int> Buscador::combinarVec(vector<unsigned int> final,
                                           vector<unsigned int> parcial,
                                           Operador op) {
  size_t i = 0, j = 0;
  vector<unsigned int> resultado;
  if (op == AND) {
    while (i < final.size() && j < parcial.size()) {
      if (final[i] < parcial[j])
        ++i;
      else if (parcial[j] < final[i])
        ++j;
      else {
        resultado.push_back(final[i]);
        ++i;
        ++j;
      }
    }
  } else if (op == OR) {
    while (i < final.size() && j < parcial.size()) {
      if (final[i] < parcial[j]) {
        resultado.push_back(final[i]);
        ++i;
      } else if (parcial[j] < final[i]) {
        resultado.push_back(parcial[j]);
        ++j;
      } else {
        resultado.push_back(final[i]);
        ++i;
        ++j;
      }
    }
    while (i < final.size()) {
      resultado.push_back(final[i]);
      ++i;
    }
    while (j < parcial.size()) {
      resultado.push_back(parcial[j]);
      ++j;
    }
  }
  return resultado;
}

vector<vector<string>>
Buscador::busquedaCompleta(vector<string> &camposBuscados,
                           vector<string> &valoresBuscados,
                           vector<string> &camposEsperados, Operador ope,
                           string &orden, Orden dirOrden) {
  vector<vector<string>> resu;
  if (camposBuscados.size() != valoresBuscados.size())
    return {{"ERRORRR"}};
  vector<unsigned int> idFinal;

  for (size_t i = 0; i < camposBuscados.size(); ++i) {
    vector<unsigned int> ids;
    string campo = camposBuscados[i];
    string valor = valoresBuscados[i];

    if (campo == "id") {
      try {
        unsigned long raw_id = stoul(valor);
        if (raw_id <= numeric_limits<unsigned int>::max()) {
          unsigned int id = static_cast<unsigned int>(raw_id);
          CNode **nodoPtr;
          if (arbol_id.find(nodoPtr, id))
            ids.push_back(id);
        }
      } catch (...) {
        return {{"ERROR2"}};
      }
    } else {
      auto itTipo = find(arbol_busqueda.campos.begin(),
                         arbol_busqueda.campos.end(), campo);
      if (itTipo == arbol_busqueda.campos.end())
        return {{"ERROR3"}};
      size_t idxTipo =
          static_cast<size_t>(itTipo - arbol_busqueda.campos.begin());
      string tipo = arbol_busqueda.tipos[idxTipo];

      if (valor.find("..") != string::npos) {
        size_t pos = valor.find("..");
        string valMin = valor.substr(0, pos);
        string valMax = valor.substr(pos + 2);
        vector<unsigned int> idsMin =
            arbol_busqueda.buscarRango(campo, ">=", valMin);
        vector<unsigned int> idsMax =
            arbol_busqueda.buscarRango(campo, "<=", valMax);
        ids = combinarVec(idsMin, idsMax, AND);
      } else {
        string operador = "=";
        if ((valor[0] == '<' || valor[0] == '>') && valor[1] == '=') {
          operador = valor.substr(0, 2);
          valor = valor.substr(2);
        } else if (valor[0] == '<' || valor[0] == '>') {
          operador = valor.substr(0, 1);
          valor = valor.substr(1);
        }

        if (operador == "=") {
          ids = arbol_busqueda.buscar(campo, valor);
        } else {
          ids = arbol_busqueda.buscarRango(campo, operador, valor);
        }
      }
    }

    if (i == 0) {
      idFinal = ids;
    } else {
      sort(idFinal.begin(), idFinal.end());
      sort(ids.begin(), ids.end());
      idFinal = combinarVec(idFinal, ids, ope);
    }
  }

  if (idFinal.empty())
    return {{"VACIO"}};

  for (unsigned int id : idFinal) {
    CNode **nodoPtr;
    if (arbol_id.find(nodoPtr, id)) {
      const vector<string> &reg = (*nodoPtr)->registro;
      vector<string> temp;
      for (size_t k = 0; k < camposEsperados.size(); ++k) {
        auto it = find(arbol_busqueda.campos.begin(),
                       arbol_busqueda.campos.end(), camposEsperados[k]);
        if (it != arbol_busqueda.campos.end()) {
          size_t idx = static_cast<size_t>(it - arbol_busqueda.campos.begin());
          temp.push_back(reg[idx]);
        }
      }
      resu.push_back(temp);
    }
  }

  auto it = find(camposEsperados.begin(), camposEsperados.end(), orden);
  if (it != camposEsperados.end()) {
    size_t idxOrden = static_cast<size_t>(it - camposEsperados.begin());
    auto itTipo =
        find(arbol_busqueda.campos.begin(), arbol_busqueda.campos.end(), orden);
    size_t idxTipo =
        static_cast<size_t>(itTipo - arbol_busqueda.campos.begin());
    string tipoOrden = arbol_busqueda.tipos[idxTipo];

    if (tipoOrden == "int") {
      sort(resu.begin(), resu.end(),
           [=](const vector<string> &a, const vector<string> &b) {
             return dirOrden == ASC ? stoi(a[idxOrden]) < stoi(b[idxOrden])
                                    : stoi(a[idxOrden]) > stoi(b[idxOrden]);
           });
    } else if (tipoOrden == "float") {
      sort(resu.begin(), resu.end(),
           [=](const vector<string> &a, const vector<string> &b) {
             return dirOrden == ASC ? stof(a[idxOrden]) < stof(b[idxOrden])
                                    : stof(a[idxOrden]) > stof(b[idxOrden]);
           });
    } else {
      sort(resu.begin(), resu.end(),
           [=](const vector<string> &a, const vector<string> &b) {
             return dirOrden == ASC ? a[idxOrden] < b[idxOrden]
                                    : a[idxOrden] > b[idxOrden];
           });
    }
  }

  return resu;
}

void Buscador::print() {
  arbol_id.inorder(arbol_id.Root());
  cout << "\nÁrbol de atributos (campo:valor → IDs):\n";
  arbol_busqueda.inorder();
}