#include "Disco.h"

Disco::Disco(int _platos, int _pistas, int _sectores, int _tamanioSector)
    : platos(_platos), pistas(_pistas), sectores(_sectores), sizeSector(_tamanioSector) {
}

int Disco::getSizeDisk(){
	return platos * superficies * pistas * sectores * sizeSector;
}

void Disco::setSizeRegistro(int size){
	sizeResgistro = size;
}

