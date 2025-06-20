#include "DatabaseManager.h"
#include <stdexcept>

DatabaseManager::DatabaseManager(int platos, int pistas, int sectores, int tamanioSector)
    : disco(platos, pistas, sectores, tamanioSector) {}


void DatabaseManager::insertRegistro(const Registro& registro) {
    DireccionDisco direccion = disco.buscarEspacioLibre();

    std::string data = registro.serializar();
    disco.escribir(
        direccion.plato,
        direccion.superficie,
        direccion.pista,
        direccion.sector,
        data.c_str(),
        data.size()
    );

    // Guardar en índice
    indiceId.insertar(registro.getId(), direccion);
}

std::vector<Registro> DatabaseManager::getAllRecords() const {
    std::vector<Registro> registros;
    // Aquí puedes recorrer todas las direcciones del disco y deserializar
    return registros;
}

Registro DatabaseManager::getRecordById(int id) const {
    DireccionDisco dir = indiceId.buscar(id);
    char* buffer = new char[disco.getTamanioSector()];
    
    disco.leer(dir.plato, dir.superficie, dir.pista, dir.sector, buffer, disco.getTamanioSector());
    
    Registro reg = Registro::deserializar(std::string(buffer));
    delete[] buffer;
    return reg;
}

void DatabaseManager::displayAllRecords() const {
    for (const Registro& reg : getAllRecords()) {
        std::cout << reg.toString() << std::endl;
    }
}