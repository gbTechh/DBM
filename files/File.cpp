#include "File.h"
#include <stdexcept>

File::File(const std::string& _filename) : filename(_filename) {
    file.open(filename);
    if (!file.is_open()) {
        throw std::runtime_error("No se pudo abrir el archivo: " + filename);
    }
}

File::~File() {
    if (file.is_open()) {
        file.close();
    }
}

std::string File::ReadHead() {
    if (!file.is_open()) {
        throw std::runtime_error("Archivo no abierto");
    }
    
    std::string header;
    if (!std::getline(file, header)) {
        throw std::runtime_error("Error al leer la cabecera");
    }
    return header;
}

bool File::ReadLine(std::string& line) {
    if (!file.is_open()) {
        throw std::runtime_error("Archivo no abierto");
    }
    return static_cast<bool>(std::getline(file, line));
}

void File::Reset() {
    if (file.is_open()) {
        file.clear();
        file.seekg(0);
    }
}