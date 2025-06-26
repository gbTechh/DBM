#include "../loader/DataLoader.h"
#include "../manager/DatabaseManager.h"
#include "httplib.h"
#include "json.hpp"
#include "procesador_filtro.hpp"
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
      int sectores = stoi(config["sectores"].get<string>());
      int capacidad = stoi(config["capacidad"].get<string>());

      // Liberar si ya existía
      if (dbmanager)
        delete dbmanager;
      if (loader)
        delete loader;

      // Crear nuevas instancias
      dbmanager = new DatabaseManager(platos, pistas, sectores, capacidad);
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
          resultado["header"] = r.getHeaderLine();
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

  // Ruta GET para obtener el encabezado
  //   svr.Get("/obtener-encabezado",
  //           [](const httplib::Request &req, httplib::Response &res) {
  //             res.set_header("Access-Control-Allow-Origin", "*");

  //             if (!dbmanager) {
  //               res.status = 500;
  //               res.set_content("{\"status\": \"error\", \"message\": \"Base
  //               de "
  //                               "datos no configurada\"}",
  //                               "application/json");
  //               return;
  //             }

  //             try {
  //               Registro r = dbmanager->getRegitro();
  //               vector<string> encabezado = r.GetHeaderByIndex(1);
  //               json resultado;
  //               resultado["status"] = "success";
  //               resultado["encabezado"] = encabezado;

  //               res.set_content(resultado.dump(), "application/json");
  //             } catch (const exception &e) {
  //               cerr << "Error al obtener encabezado: " << e.what() << endl;
  //               res.status = 400;
  //               res.set_content("{\"status\": \"error\", \"message\": \"Error
  //               al "
  //                               "obtener encabezado\"}",
  //                               "application/json");
  //             }
  //           });

  // Ruta POST para buscar por atributo
  svr.Post("/buscar", [](const httplib::Request &req, httplib::Response &res) {
    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_header("Access-Control-Allow-Methods", "POST, OPTIONS");
    res.set_header("Access-Control-Allow-Headers", "Content-Type");

    if (!dbmanager) {
      res.status = 500;
      res.set_content("{\"status\": \"error\", \"message\": \"Base de "
                      "datos o loader no configurados\"}",
                      "application/json");
      return;
    }

    try {
      json payload = json::parse(req.body);
      vector<string> campos_bus = payload["campos_bus"].get<vector<string>>();
      vector<string> valores_bus = payload["valores_bus"].get<vector<string>>();
      string operador = payload["operador"].get<string>();
      vector<string> columnas = payload["columnas"].get<vector<string>>();
      string atributo_orden = payload["ordenar"]["atributo"].get<string>();
      string direccion_orden = payload["ordenar"]["direccion"].get<string>();

      Buscador b = dbmanager->getBuscador();
      vector<vector<string>> resultados = b.busquedaCompleta(
          campos_bus, valores_bus, columnas, AND, atributo_orden, ASC);

      json resultado;
      resultado["status"] = "success";
      resultado["data"] = resultados;

      res.set_content(resultado.dump(), "application/json");
    } catch (const exception &e) {
      cerr << "Error al procesar búsqueda: " << e.what() << endl;
      res.status = 400;
      res.set_content("{\"status\": \"error\", \"message\": \"Error al "
                      "procesar búsqueda\"}",
                      "application/json");
    }
  });

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