#include "loader/DataLoader.h"
#include "manager/DatabaseManager.h"
#include "sql/Parser.h"
#include "sql/Sql.h"
#include "sql/Tokenizer.h"
#include <cstddef>
#include <iostream>

using namespace std;

int main() {
  try {

    DatabaseManager dbManager(
        2, 2, 2, 5, 16); // 2 platos, 2 pistas, 100 sectores, 8 tamaño sector
    // 1. Inicializar el disco y el gestor de base de datos
    cout << "Inicializando disco..." << endl;

    // 2. Cargar datos desde archivo
    string archivo = "datos.txt";
    cout << "Cargando datos desde '" << archivo << "'..." << endl;
    DataLoader loader;
    loader.loadFromFile(archivo, dbManager);
    std::vector<std::string> campos = dbManager.getRegitro().getCamposNombre();
    for (size_t i = 0; i < campos.size(); i++) {
      std::cout << campos[i] << ", ";
    }
    std::cout << "\n";

    SQL sql("SELECT nombre,edad,telefono FROM data WHERE edad = 30.9",
            dbManager);
    std::vector<std::tuple<int, int, int>> bites;
    std::vector<std::pair<int, std::vector<std::string>>> registros =
        sql.getRegistros(bites);

    std::cout << "registros size: " << registros.size() << "\n";
    for (size_t i = 0; i < registros.size(); ++i) {
      const auto &fila =
          registros[i].second; // accedemos al vector<string> de la pareja
      for (size_t j = 0; j < fila.size(); ++j) {
        std::cout << "Registro: " << fila[j] << ", ";
      }
      std::cout << "\n";
    }

    for (size_t i = 0; i < bites.size(); ++i) {
      int id = std::get<0>(bites[i]);
      int inicio = std::get<1>(bites[i]);
      int final = std::get<2>(bites[i]);
      dbManager.getDisco().mostrarUbicacion(inicio);
      std::cout << "-----------------\n";
    }
    //  dbManager.getIndexAttr().imprimir();

    // cout << "ID: " << registro.first << " ATTRS: " << registro.second <<
    // "\n";

    // std::string val = "Juan";
    // int b = dbManager.getRegistroByAttr("nombre", val);
    // cout << "JUAN: " << b << "\n";
    // cout << "value: " << val << "\n";

    // 3. Mostrar resumen de registros almacenados
    // cout << "\n=== RESUMEN DE REGISTROS ===" << endl;
    // dbManager.displayAllRecords();

    // 4. Ejemplo de búsqueda por ID (opcional)
    // int idBuscar = 1;
    // cout << "\nBuscando registro con ID = " << idBuscar << "..." << endl;
    // Registro reg = dbManager.getRecordById(idBuscar);
    // cout << reg.toString() << endl;
    std::string query = "SELECT nombre,edad,telefono FROM data WHERE nombre = "
                        "Carlos AND edad = 40";

    Tokenizer tokenizer(query);
    auto tokens = tokenizer.tokenize();

    Parser parser(tokens);
    try {
      Consulta c = parser.parse();

      std::cout << "Campos: ";
      for (const auto &campo : c.camposSeleccionados)
        std::cout << campo << " ";
      std::cout << "\nTabla: " << c.tabla << "\n";

      for (size_t i = 0; i < c.condiciones.size(); ++i) {
        const auto &cond = c.condiciones[i];
        std::cout << "Condición: " << cond.campo << " " << cond.operador << " "
                  << cond.valor << "\n";
        if (i < c.conectores.size())
          std::cout << "Conector: " << c.conectores[i] << "\n";
      }

    } catch (const std::exception &e) {
      std::cerr << "Error al analizar: " << e.what() << "\n";
    }

  } catch (const exception &e) {
    cerr << "Errorrr: " << e.what() << endl;
    return 1;
  }

  return 0;
}