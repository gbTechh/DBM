#include <vector>
#include <string>
#include "Sector.h"
#include "../modelo/DireccionDisco.h"
#include "../modelo/Registro.h"
using namespace std;

class Disco {
private:
    int platos;
    int superficies = 2;
    int pistas;
    int sectores;
    int sizeSector;
    int sizeResgistro;

public:
    Disco(int platos, int pistas, int sectores, int tamanioSector);

    int getSizeDisk();
    void setSizeRegistro(int size);

};

