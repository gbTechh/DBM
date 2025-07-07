#ifndef CAMPO_CODIFICADO_H
#define CAMPO_CODIFICADO_H

#include "../utils/Enum.h"
#include <iostream>
#include <string>
#include <variant>

struct Campo_Codificado {
  unsigned int campo;
  ETypeData tipo;

  std::variant<unsigned long long, float, std::string> valor;

  Campo_Codificado();
  Campo_Codificado(unsigned int campo_, ETypeData tipo_,
                   const std::string &valor);
  Campo_Codificado(unsigned int campo_, std::string tipo_,
                   const std::string &valor);

  bool operator<(const Campo_Codificado &otra) const;
  bool operator>(const Campo_Codificado &otra) const;
  bool operator==(const Campo_Codificado &otra) const;

  int comparar(const std::string &valorComparado) const;
};

#endif // CAMPO_CODIFICADO_H
