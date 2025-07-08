#include "../loader/DataLoader.h"
#include "../manager/DatabaseManager.h"
#include "../sql/Sql.h"
#include "httplib.h"
#include "json.hpp"
#include <iostream>
#include <vector>

using namespace std;
using json = nlohmann::json;

// Variables globales
DatabaseManager *dbmanager = nullptr;
DataLoader *loader = nullptr;
const string archivo = "datos.txt";

int main() {
  httplib::Server svr;

  // Logger para solicitudes
  svr.set_logger([](const httplib::Request &req, const httplib::Response &res) {
    cout << req.method << " " << req.path << " -> " << res.status << endl;
  });

  // Ruta POST para configurar DatabaseManager
  svr.Post("/configurar", [](const httplib::Request &req,
                             httplib::Response &res) {
    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_header("Access-Control-Allow-Methods", "POST, OPTIONS");
    res.set_header("Access-Control-Allow-Headers", "Content-Type");

    try {
      json config = json::parse(req.body);

      int platos = stoi(config["platos"].get<string>());
      int pistas = stoi(config["pistas"].get<string>());
      int superficies = stoi(config["superficies"].get<string>());
      int sectores = stoi(config["sectores"].get<string>());
      int capacidad = stoi(config["capacidad"].get<string>());

      // Liberar si ya existía
      if (dbmanager)
        delete dbmanager;
      if (loader)
        delete loader;

      // Crear nuevas instancias
      dbmanager =
          new DatabaseManager(platos, superficies, pistas, sectores, capacidad);
      loader = new DataLoader();

      res.set_content("{\"status\": \"success\", \"message\": \"Base de datos "
                      "configurada\"}",
                      "application/json");
    } catch (const exception &e) {
      cerr << "Error al configurar DBManager: " << e.what() << endl;
      res.status = 400;
      res.set_content(
          "{\"status\": \"error\", \"message\": \"Error en configuración\"}",
          "application/json");
    }
  });

  // Ruta POST para cargar datos desde archivo
  svr.Post(
      "/cargar-datos", [](const httplib::Request &req, httplib::Response &res) {
        if (!dbmanager || !loader) {
          res.status = 500;
          res.set_content("{\"status\": \"error\", \"message\": \"Base de "
                          "datos o loader no configurados\"}",
                          "application/json");
          return;
        }

        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "POST, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");

        try {
          json payload = json::parse(req.body);
          string archivo = payload["archivo"].get<string>();

          loader->loadFromFile(archivo, *dbmanager);

          json resultado;
          Registro r = dbmanager->getRegitro();
          resultado["status"] = "success";
          resultado["message"] = "Datos cargados correctamente";
          resultado["header"] = r.getHeaderLineWithoutID();
          // Opcional: agregar número de registros
          // resultado["num_registros"] = dbmanager->getRecordCount();

          res.set_content(resultado.dump(), "application/json");
        } catch (const exception &e) {
          cerr << "Error al cargar datos: " << e.what() << endl;
          res.status = 400;
          res.set_content(
              "{\"status\": \"error\", \"message\": \"Error al cargar datos\"}",
              "application/json");
        }
      });


  // Ruta POST para buscar por atributo
  svr.Post("/buscar", [](const httplib::Request &req, httplib::Response &res) {
    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_header("Access-Control-Allow-Methods", "POST, OPTIONS");
    res.set_header("Access-Control-Allow-Headers", "Content-Type");

    if (!dbmanager) {
      res.status = 500;
      res.set_content(
          R"({"status": "error", "message": "Base de datos o loader no configurados"})",
          "application/json");
      return;
    }

    try {
      // Parsear el cuerpo de la solicitud
      nlohmann::json payload = nlohmann::json::parse(req.body);
      const std::string consulta_sql = payload["query"];
      SQL sql(consulta_sql, *dbmanager);
      std::vector<std::tuple<int, int, int>> bites;
      std::vector<std::pair<int, std::vector<std::string>>> registros =
          sql.getRegistros(bites);

/*lo nuevo que agregue */
    //  std::vector<std::pair<UbicacionFisica, UbicacionFisica>> ubicaciones;
    //  std::vector<std::pair<int, std::vector<std::string>>> registros = sql.getRegistrosConUbicacion(ubicaciones);
/*aqui se acaba esta parte */

      nlohmann::json data_json = nlohmann::json::array();
      std::cout << "registros size: " << registros.size() << "\n";
/* esto estariamos quintando
      for (const auto &[id, fila] : registros) {
        nlohmann::json fila_json;
        fila_json["id"] = id;
        fila_json["valores"] = fila;
        data_json.push_back(fila_json);
      }
*/

        for (size_t i = 0; i < registros.size(); ++i) {
          const auto &[id, fila] = registros[i];
          const auto &[_, inicio, fin] = bites[i];

          int size = fin - inicio;
          auto ubicaciones = dbmanager->getDisco().calcularUbicacionesRegistro(inicio, size);

          nlohmann::json ubicaciones_json = json::array();
          for (const auto &u : ubicaciones) {
            ubicaciones_json.push_back({
              {"plato", u.plato},
              {"superficie", u.superficie},
              {"pista", u.pista},
              {"sector", u.sector}
            });
          }

          nlohmann::json fila_json;
          fila_json["valores"] = fila;
          fila_json["ubicaciones"] = ubicaciones_json;

          data_json.push_back(fila_json);
        }

      std::vector<std::string> headers =
          dbmanager->getRegitro().getCamposNombre();
      nlohmann::json headers_json = headers;

      nlohmann::json resultado;
      resultado["status"] = "success";
      resultado["data"] = data_json; // Esto asumo que ya lo construiste
      resultado["headers"] = headers_json;

      std::cout << resultado.dump(4) << std::endl; // ESTA LINEA MAS AGREGUEEE
      res.set_content(resultado.dump(), "application/json");
    } catch (const nlohmann::json::exception &e) {
      std::cerr << "Error al procesar JSON: " << e.what() << std::endl;
      res.status = 400;
      res.set_content(
          R"({"status": "error", "message": "Error al procesar JSON"})",
          "application/json");
    } catch (const std::exception &e) {
      std::cerr << "Error al procesar búsqueda: " << e.what() << std::endl;
      res.status = 400;
      res.set_content(
          R"({"status": "error", "message": "Error al procesar búsqueda"})",
          "application/json");
    }
  });

  // svr.Post("/buscar", [](const httplib::Request& req, httplib::Response& res)
  // {
  //     res.set_header("Access-Control-Allow-Origin", "*");
  //     res.set_header("Access-Control-Allow-Methods", "POST, OPTIONS");
  //     res.set_header("Access-Control-Allow-Headers", "Content-Type");

  //     if (!dbmanager) {
  //         res.status = 500;
  //         res.set_content("Base de datos no configurada", "text/plain");
  //         return;
  //     }

  //     try {
  //         json payload = json::parse(req.body);
  //         //json resultados = procesarPeticionCompleta(payload, dbmanager);
  //         json resultados = procesarPeticionCompleta(payload, dbmanager);
  //         cout<< resultados<<"\n";
  //         res.set_content(resultados.dump(), "application/json");
  //     } catch (const exception& e) {
  //         cerr << "Error al procesar búsqueda: " << e.what() << endl;
  //         res.status = 400;
  //         res.set_content("Error al procesar búsqueda", "text/plain");
  //     }
  // });

  // Rutas OPTIONS para CORS
  svr.Options("/configurar",
              [](const httplib::Request &req, httplib::Response &res) {
                res.set_header("Access-Control-Allow-Origin", "*");
                res.set_header("Access-Control-Allow-Methods", "POST, OPTIONS");
                res.set_header("Access-Control-Allow-Headers", "Content-Type");
                res.status = 204;
              });

  svr.Options("/cargar-datos",
              [](const httplib::Request &req, httplib::Response &res) {
                res.set_header("Access-Control-Allow-Origin", "*");
                res.set_header("Access-Control-Allow-Methods", "POST, OPTIONS");
                res.set_header("Access-Control-Allow-Headers", "Content-Type");
                res.status = 204;
              });

  svr.Options("/obtener-encabezado",
              [](const httplib::Request &req, httplib::Response &res) {
                res.set_header("Access-Control-Allow-Origin", "*");
                res.set_header("Access-Control-Allow-Methods", "GET, OPTIONS");
                res.set_header("Access-Control-Allow-Headers", "Content-Type");
                res.status = 204;
              });

  svr.Options("/buscar",
              [](const httplib::Request &req, httplib::Response &res) {
                res.set_header("Access-Control-Allow-Origin", "*");
                res.set_header("Access-Control-Allow-Methods", "POST, OPTIONS");
                res.set_header("Access-Control-Allow-Headers", "Content-Type");
                res.status = 204;
              });

  cout << "Servidor escuchando en http://localhost:8080\n";
  svr.listen("localhost", 8080);

  // Limpieza
  if (dbmanager)
    delete dbmanager;
  if (loader)
    delete loader;
  return 0;
}