#include "DataLoader.h"
#include "File.h"
#include <sstream>
#include <stdexcept>
#include <iostream>

void DataLoader::loadFromFile(const std::string& filename, DatabaseManager& dbManager) {
    File file(filename); // Abre el archivo
    //std::vector<Registro> registros;

    // Leer cabecera (tipos o nombres de campos)
    std::string header;
    if (!file.readLine(header)) {
        throw std::runtime_error("El archivo está vacío o no se pudo leer la cabecera.");
    }

    // Leer registros línea por línea
    std::string line;
    while (file.readLine(line)) {
        Registro registro = parseLineToRecord(line);
        dbManager.insertRegistro(registro); // Insertar en disco e índice
        //registros.push_back(registro);
    }

    //return registros;
}

Registro DataLoader::parseLineToRecord(const std::string& line) {
    std::istringstream iss(line);
    Registro registro;
    std::string value;
    int fieldIndex = 0;

    while (iss >> value) {
        // Aquí puedes mejorar usando nombres de campo leídos de la cabecera si lo deseas
        std::string fieldName = "campo" + std::to_string(fieldIndex++);
        registro.atributos[fieldName] = value;
    }

    // Asignamos un ID básico basado en hash simple del contenido
    // OJO: esto es solo un ejemplo. Puedes tener un generador de IDs único si lo prefieres.
    registro.id = std::hash<std::string>{}(line);

    return registro;
}