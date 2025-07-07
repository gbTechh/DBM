#ifndef DISCO_H
#define DISCO_H

#include "DireccionDisco.h"
#include <vector>

struct UbicacionFisica {
  int plato;
  int superficie;
  int pista;
  int sector;
};

class Disco {
private:
  int platos;
  int superficiesPorPlato; // ahora es variable, antes era fijo en 2
  int pistasPorSuperficie;
  int sectoresPorPista;
  int sizeSector;
  int sizeRegistro;

  int posicionActual = 0;

  UbicacionFisica calcularUbicacion(int bytePos);

public:
  Disco(int platos, int superficiesPorPlato, int pistasPorSuperficie,
        int sectoresPorPista, int sizeSector);

  int getPlatos() const { return platos; }
  int getSuperficiesPorPlato() const { return superficiesPorPlato; }
  int getPistasPorSuperficie() const { return pistasPorSuperficie; }
  int getSectoresPorPista() const { return sectoresPorPista; }
  int getSizeDisk() const;
  int calcularDireccionFisica(const std::vector<int> &campos, int &final);

  void reiniciar();
  int getPosicionActual() const;
  void agregarRegistro(const std::vector<int> &campos, int &inicio, int &fin);
  void mostrarUbicacion(int bytePos);
};

#endif