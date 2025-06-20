#include "manager/DatabaseManager.h"
#include "utils/Parser.h"
#include <iostream>

using namespace std;

int main() {
	try {

		// 1. Inicializar el disco y el gestor de base de datos
		cout << "Inicializando disco..." << endl;
		DatabaseManager dbManager(2, 2, 100, 8); // 2 platos, 2 pistas, 100 sectores, 8 tamaño sector

		// 2. Cargar datos desde archivo
		string archivo = "datos.txt";
		cout << "Cargando datos desde '" << archivo << "'..." << endl;
		DataLoader::loadFromFile(archivo, dbManager);

		// 3. Mostrar resumen de registros almacenados
		cout << "\n=== RESUMEN DE REGISTROS ===" << endl;
		dbManager.displayAllRecords();

		// 4. Ejemplo de búsqueda por ID (opcional)
		int idBuscar = 1;
		cout << "\nBuscando registro con ID = " << idBuscar << "..." << endl;
		Registro reg = dbManager.getRecordById(idBuscar);
		cout << reg.toString() << endl;
			
	} catch (const exception& e) {
		cerr << "Error: " << e.what() << endl;
		return 1;
	}
	
	return 0;
}