#include "Sql.h"
#include <algorithm>
#include <cstddef>
#include <set>
#include <string>

SQL::SQL(const std::string &query, DatabaseManager &dbManager_)
    : tokenizer(query), parser(tokenizer.tokenize()), dbManager(dbManager_) {
  consulta = parser.parse();
}

std::vector<unsigned int> SQL::buscarAttr(const std::string columna,
                                          const std::string valor) {
  return dbManager.getIndexAttr().buscar(columna, valor);
}

bool SQL::cumpleCondiciones(const std::vector<std::string> &registro) {
  const auto &headers = dbManager.getRegitro().getCamposNombre();
  const auto &tipos = dbManager.getRegitro().getCamposTipo();

  bool resultado = true;

  // Debug output
  std::cout << "=== VERIFICANDO CONDICIONES ===" << std::endl;
  std::cout << "Registro: ";
  for (const auto &campo : registro) {
    std::cout << "[" << campo << "] ";
  }
  std::cout << std::endl;

  for (size_t i = 0; i < consulta.condiciones.size(); ++i) {
    const auto &cond = consulta.condiciones[i];

    // Obtener índice del campo
    auto it = std::find(headers.begin(), headers.end(), cond.campo);
    if (it == headers.end()) {
      std::cout << "Campo '" << cond.campo << "' no encontrado en headers"
                << std::endl;
      return false;
    }
    int idx = std::distance(headers.begin(), it);

    std::string valor = registro[idx];
    std::string esperado = cond.valor;
    std::string tipo = tipos[idx];
    std::string operador = cond.operador;

    std::cout << "Condición " << i << ": " << cond.campo << " " << operador
              << " " << esperado << " (valor real: " << valor
              << ", tipo: " << tipo << ")" << std::endl;

    bool cumple = false;

    try {
      if (tipo == "INT") {
        int val = std::stoi(valor);
        int esp = std::stoi(esperado);

        if (operador == "=")
          cumple = val == esp;
        else if (operador == "!=")
          cumple = val != esp;
        else if (operador == "<")
          cumple = val < esp;
        else if (operador == "<=")
          cumple = val <= esp;
        else if (operador == ">")
          cumple = val > esp;
        else if (operador == ">=")
          cumple = val >= esp;

      } else if (tipo == "FLOAT") {
        float val = std::stof(valor);
        float esp = std::stof(esperado);

        if (operador == "=")
          cumple = val == esp;
        else if (operador == "!=")
          cumple = val != esp;
        else if (operador == "<")
          cumple = val < esp;
        else if (operador == "<=")
          cumple = val <= esp;
        else if (operador == ">")
          cumple = val > esp;
        else if (operador == ">=")
          cumple = val >= esp;

      } else { // VARCHAR
        if (operador == "=")
          cumple = valor == esperado;
        else if (operador == "!=")
          cumple = valor != esperado;
        else if (operador == "<")
          cumple = valor < esperado;
        else if (operador == "<=")
          cumple = valor <= esperado;
        else if (operador == ">")
          cumple = valor > esperado;
        else if (operador == ">=")
          cumple = valor >= esperado;
      }
    } catch (const std::exception &e) {
      std::cout << "Error al convertir valores: " << e.what() << std::endl;
      cumple = false;
    }

    std::cout << "  → Cumple: " << (cumple ? "SÍ" : "NO") << std::endl;

    // Evaluar con conector lógico (AND / OR)
    if (i == 0) {
      resultado = cumple;
    } else {
      std::string conector = consulta.conectores[i - 1];
      std::cout << "  → Aplicando conector: " << conector << std::endl;
      if (conector == "AND") {
        resultado = resultado && cumple;
      } else if (conector == "OR") {
        resultado = resultado || cumple;
      }
    }
    std::cout << "  → Resultado acumulado: " << (resultado ? "SÍ" : "NO")
              << std::endl;
  }

  std::cout << "=== RESULTADO FINAL: " << (resultado ? "CUMPLE" : "NO CUMPLE")
            << " ===" << std::endl;
  return resultado;
}

std::vector<std::pair<int, std::vector<std::string>>>
SQL::getRegistros(std::vector<std::tuple<int, int, int>> &bitesVector) {
  std::vector<unsigned int> idsTotales;

  std::cout << "=== PROCESANDO CONSULTA ===" << std::endl;
  std::cout << "Número de condiciones: " << consulta.condiciones.size()
            << std::endl;

  // Si no hay condiciones, retornar todos los registros
  if (consulta.condiciones.empty()) {
    // Aquí deberías implementar la lógica para obtener todos los registros
    return {};
  }

  for (size_t i = 0; i < consulta.condiciones.size(); ++i) {
    const auto &cond = consulta.condiciones[i];
    std::vector<unsigned int> ids;

    std::cout << "Procesando condición " << i << ": " << cond.campo << " "
              << cond.operador << " " << cond.valor << std::endl;

    if (cond.operador == "=") {
      ids = dbManager.getIndexAttr().buscar(cond.campo, cond.valor);
    } else {
      ids = dbManager.getIndexAttr().buscarRango(cond.campo, cond.operador,
                                                 cond.valor);
    }

    std::cout << "  → IDs encontrados (" << ids.size() << "): ";
    for (auto id : ids)
      std::cout << id << " ";
    std::cout << std::endl;

    if (i == 0) {
      idsTotales = ids;
    } else {
      const std::string &conector = consulta.conectores[i - 1];
      std::cout << "  → Aplicando conector: " << conector << std::endl;

      if (conector == "AND") {
        std::vector<unsigned int> interseccion;
        std::sort(idsTotales.begin(), idsTotales.end());
        std::sort(ids.begin(), ids.end());
        std::set_intersection(idsTotales.begin(), idsTotales.end(), ids.begin(),
                              ids.end(), std::back_inserter(interseccion));
        idsTotales = interseccion;
      } else if (conector == "OR") {
        std::set<unsigned int> unionSet(idsTotales.begin(), idsTotales.end());
        unionSet.insert(ids.begin(), ids.end());
        idsTotales.assign(unionSet.begin(), unionSet.end());
      }
    }

    std::cout << "  → IDs totales tras operación (" << idsTotales.size()
              << "): ";
    for (auto id : idsTotales)
      std::cout << id << " ";
    std::cout << std::endl;
  }

  std::cout << "=== OBTENIENDO REGISTROS ===" << std::endl;
  std::vector<std::pair<int, std::vector<std::string>>> registros;

  for (unsigned int id : idsTotales) {
    std::pair<int, std::vector<std::string>> registro;
    int inicio, fin;

    if (!dbManager.getIndexID().find(id, registro, inicio, fin)) {
      std::cout << "No se pudo encontrar registro con ID: " << id << std::endl;
      continue;
    }

    std::cout << "Verificando registro con ID " << id << std::endl;

    // IMPORTANTE: Solo verificar condiciones adicionales si hay múltiples
    // condiciones y si el índice no pudo resolver todas las condiciones
    // completamente
    bool debeVerificar = false;

    // Verificar si necesitamos validación adicional
    // Esto sucede cuando tenemos condiciones que no pueden ser resueltas
    // completamente por índices
    for (const auto &cond : consulta.condiciones) {
      if (cond.operador != "=") {
        debeVerificar = true;
        break;
      }
    }

    if (debeVerificar && !cumpleCondiciones(registro.second)) {
      std::cout << "  → Registro rechazado por condiciones" << std::endl;
      continue;
    }

    std::cout << "  → Registro aceptado" << std::endl;

    std::tuple<int, int, int> bites = std::make_tuple(id, inicio, fin);
    bitesVector.push_back(bites);
    registros.push_back(registro);
  }

  std::cout << "=== RESULTADO FINAL: " << registros.size()
            << " registros ===" << std::endl;
  return registros;
}



std::vector<std::pair<int, std::vector<std::string>>> SQL::getRegistrosConUbicacion(std::vector<std::pair<UbicacionFisica, UbicacionFisica>> &ubicacionesVector) {
  std::vector<std::pair<int, std::vector<std::string>>> registros;
  std::vector<unsigned int> idsTotales;

  if (consulta.condiciones.empty()) return registros;

  for (size_t i = 0; i < consulta.condiciones.size(); ++i) {
    const auto &cond = consulta.condiciones[i];
    std::vector<unsigned int> ids = (cond.operador == "=")
        ? dbManager.getIndexAttr().buscar(cond.campo, cond.valor)
        : dbManager.getIndexAttr().buscarRango(cond.campo, cond.operador, cond.valor);

    if (i == 0) idsTotales = ids;
    else {
      const std::string &conector = consulta.conectores[i - 1];
      if (conector == "AND") {
        std::vector<unsigned int> interseccion;
        std::sort(idsTotales.begin(), idsTotales.end());
        std::sort(ids.begin(), ids.end());
        std::set_intersection(idsTotales.begin(), idsTotales.end(), ids.begin(), ids.end(), std::back_inserter(interseccion));
        idsTotales = interseccion;
      } else if (conector == "OR") {
        std::set<unsigned int> unionSet(idsTotales.begin(), idsTotales.end());
        unionSet.insert(ids.begin(), ids.end());
        idsTotales.assign(unionSet.begin(), unionSet.end());
      }
    }
  }

  for (unsigned int id : idsTotales) {
    std::pair<int, std::vector<std::string>> registro;
    int inicio, fin;
    if (!dbManager.getIndexID().find(id, registro, inicio, fin)) continue;
    bool necesitaVerificar = false;
    for (const auto &cond : consulta.condiciones) {
      if (cond.operador != "=") {
        necesitaVerificar = true;
        break;
      }
    }
    if (necesitaVerificar && !cumpleCondiciones(registro.second)) continue;

    auto ubicaciones = dbManager.getDisco().calcularUbicacionesRegistro(inicio, fin - inicio);
    if (!ubicaciones.empty()) {
      ubicacionesVector.emplace_back(ubicaciones.front(), ubicaciones.back());
    } else {
      UbicacionFisica dummy{};
      ubicacionesVector.emplace_back(dummy, dummy);
    }

    registros.push_back(registro);
  }
  return registros;
}
