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
        res.set_content(R"({"status": "error", "message": "Base de datos o loader no configurados"})",
                        "application/json");
        return;
    }

    try {
        // Parsear el cuerpo de la solicitud
        nlohmann::json payload = nlohmann::json::parse(req.body);

        // Verificar que el payload es un objeto
        if (!payload.is_object()) {
            throw std::invalid_argument("El payload debe ser un objeto JSON");
        }

        // Extraer los campos del payload
        std::vector<std::string> campos_bus = payload.at("campos_bus").get<std::vector<std::string>>();
        std::vector<std::string> valores_bus = payload.at("valores_bus").get<std::vector<std::string>>();
        std::string operador = payload.at("operador").get<std::string>();
        std::vector<std::string> columnas = payload.at("columnas").get<std::vector<std::string>>();
        std::string atributo_orden = payload.at("ordenar").at("atributo").get<std::string>();
        std::string direccion_orden = payload.at("ordenar").at("direccion").get<std::string>();

        // Validar el operador
        Operador op = (operador == "AND") ? AND : OR; // Asume AND por defecto si no es OR
        // Validar la dirección de ordenamiento
        Orden dir_orden = (direccion_orden == "ASC") ? ASC : DES; // Asume ASC por defecto si no es DES

        // Realizar la búsqueda
        Buscador b = dbmanager->cargarBuscador();
        b.print();
        
        std::vector<std::vector<std::string>> resultados = b.busquedaCompleta(
            campos_bus, valores_bus, columnas, OR, atributo_orden, dir_orden);

        // Imprimir resultados para depuración
        for (size_t i = 0; i < resultados.size(); ++i) {
            for (size_t j = 0; j < resultados[i].size(); ++j) { // Corregido: usar resultados[i].size()
                std::cout << resultados[i][j] << ", ";
            }
            std::cout << "\n";
        }

        // Crear el objeto JSON de respuesta
        nlohmann::json resultado;
        resultado["status"] = "success";
        resultado["data"] = resultados; // Asignar resultados como un campo del objeto

        // Enviar la respuesta
        res.set_content(resultado.dump(), "application/json");
    } catch (const nlohmann::json::exception &e) {
        std::cerr << "Error al procesar JSON: " << e.what() << std::endl;
        res.status = 400;
        res.set_content(R"({"status": "error", "message": "Error al procesar JSON"})",
                        "application/json");
    } catch (const std::exception &e) {
        std::cerr << "Error al procesar búsqueda: " << e.what() << std::endl;
        res.status = 400;
        res.set_content(R"({"status": "error", "message": "Error al procesar búsqueda"})",
                        "application/json");
    }
});

    // svr.Post("/buscar", [](const httplib::Request& req, httplib::Response& res) {
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