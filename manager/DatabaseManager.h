#include "./disco/Disco.h"
#include "./registro/Registro.h"
#include "./indices/IndexID.h"

#include <vector>

class DatabaseManager {
public:
    // Constructor: inicializa el disco con los parámetros dados
    DatabaseManager(int platos = 2, int pistas = 100, int sectores = 8, int tamanioSector = 100);

    // Insertar un registro en el disco
    void insertRegistro(const Registro& registro);

    // Obtener todos los registros (lee desde el disco)
    std::vector<Registro> getAllRecords() const;

    // Buscar por ID usando índice
    Registro getRecordById(int id) const;

    // Mostrar todos los registros (para depuración)
    void displayAllRecords() const;

private:
    Disco disco;
    IndexID indiceId;  // Índice B+ para búsquedas por ID
};

#endif // DATABASE_MANAGER_H