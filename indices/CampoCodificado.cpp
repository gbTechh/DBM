#include "CampoCodificado.h"

Campo_Codificado::Campo_Codificado()
    : campo(0), tipo(TIPO_ENTERO), valor(0ULL) {}

Campo_Codificado::Campo_Codificado(unsigned int campo_, ETypeData tipo_,
                                   const std::string &v)
    : campo(campo_), tipo(tipo_) {
  switch (tipo) {
  case TIPO_ENTERO:
    valor = std::stoull(v);
    break;
  case TIPO_FLOTANTE:
    valor = std::stof(v);
    break;
  case TIPO_STRING:
    valor = v;
    break;
  }
}

Campo_Codificado::Campo_Codificado(unsigned int campo_, std::string tipo_,
                                   const std::string &v) {
  campo = campo_;
  if (tipo_ == "int") {
    tipo = TIPO_ENTERO;
    valor = std::stoull(v);
  } else if (tipo_ == "float" || tipo_ == "decimal") {
    tipo = TIPO_FLOTANTE;
    valor = std::stof(v);
  } else {
    tipo = TIPO_STRING;
    valor = v;
  }
}

bool Campo_Codificado::operator<(const Campo_Codificado &otra) const {
  if (campo != otra.campo)
    return campo < otra.campo;
  if (tipo != otra.tipo)
    return tipo < otra.tipo;

  switch (tipo) {
  case TIPO_ENTERO:
    return std::get<unsigned long long>(valor) <
           std::get<unsigned long long>(otra.valor);
  case TIPO_FLOTANTE:
    return std::get<float>(valor) < std::get<float>(otra.valor);
  case TIPO_STRING:
    return std::get<std::string>(valor) < std::get<std::string>(otra.valor);
  default:
    return false;
  }
}

bool Campo_Codificado::operator>(const Campo_Codificado &otra) const {
  return !(*this < otra) && !(*this == otra);
}

bool Campo_Codificado::operator==(const Campo_Codificado &otra) const {
  if (campo != otra.campo || tipo != otra.tipo)
    return false;

  try {
    switch (tipo) {
    case TIPO_ENTERO:
      return std::get<unsigned long long>(valor) ==
             std::get<unsigned long long>(otra.valor);
    case TIPO_FLOTANTE:
      return std::get<float>(valor) == std::get<float>(otra.valor);
    case TIPO_STRING:
      return std::get<std::string>(valor) == std::get<std::string>(otra.valor);
    default:
      return false;
    }
  } catch (const std::bad_variant_access &e) {
    std::cerr << "[ERROR: operator==] Tipo no coincide con valor: " << e.what()
              << "\n";
    return false;
  }
}

int Campo_Codificado::comparar(const std::string &valorComparado) const {
  try {
    switch (tipo) {
    case TIPO_ENTERO: {
      unsigned long long v = std::stoull(valorComparado);
      unsigned long long actual = std::get<unsigned long long>(valor);
      if (actual < v)
        return -1;
      if (actual > v)
        return 1;
      return 0;
    }
    case TIPO_FLOTANTE: {
      float v = std::stof(valorComparado);
      float actual = std::get<float>(valor);
      if (actual < v)
        return -1;
      if (actual > v)
        return 1;
      return 0;
    }
    case TIPO_STRING: {
      const std::string &actual = std::get<std::string>(valor);
      if (actual < valorComparado)
        return -1;
      if (actual > valorComparado)
        return 1;
      return 0;
    }
    }
  } catch (const std::bad_variant_access &e) {
    std::cerr << "[ERROR comparar()] Fallo de tipo: " << e.what() << "\n";
  } catch (const std::exception &e) {
    std::cerr << "[ERROR comparar()] Error al convertir valor: " << e.what()
              << "\n";
  }

  return 0; // Por defecto si hay error
}