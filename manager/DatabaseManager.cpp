#include "DatabaseManager.h"
#include <stdexcept>

// Constructor con parámetros de disco
DbManager::DbManager(int platos, int pistas, int sectores, int tamanioSector)
    : disco(platos, pistas, sectores, tamanioSector) {
    // Constructor que crea un disco con los parámetros dados
}

// Constructor con nombre de archivo
DbManager::DbManager(const std::string& filename) : file(filename) {
    // El miembro 'file' se inicializa mediante su constructor
    // que ya maneja la apertura del archivo
}

// Método para leer un archivo
void DbManager::ReadFile(const std::string& filename) {
    try {
        // Cierra el archivo actual si está abierto
        if (file.IsOpen()) {
            // Asumiendo que has añadido un método Close() a tu clase File
            file.Reset();  // O cualquier método para reiniciar
        }
        
        // Abre el nuevo archivo (esto depende de tu implementación de File)
        // Necesitarías modificar tu clase File para permitir reabrir archivos
        // Alternativamente, podrías crear una nueva instancia de File:
        file = File(filename);  // Asumiendo que File tiene operador de asignación
        
        // Procesa el archivo
        ProcessFile();
    } catch (const std::runtime_error& e) {
        throw std::runtime_error("Error en ReadFile: " + std::string(e.what()));
    }
}

// Método para procesar el archivo cargado
void DbManager::ProcessFile() {
    if (!file.IsOpen()) {
        throw std::runtime_error("No hay archivo abierto para procesar");
    }
    
    try {
        // Leer cabecera
        std::string header = file.ReadHead();
        // Aquí iría el procesamiento de la cabecera
        
        // Leer registros
        std::string record;
        while (file.ReadLine(record)) {
            // Procesar cada registro y almacenar en el disco
            // disco.almacenar(record);  // Necesitarías implementar este método
        }
    } catch (const std::runtime_error& e) {
        throw std::runtime_error("Error procesando archivo: " + std::string(e.what()));
    }
}