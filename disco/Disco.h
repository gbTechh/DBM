#ifndef DISCO_H
#define DISCO_H

#include <iostream>
#include <vector>

struct UbicacionFisica {
  int plato;
  int superficie; // 0: superior, 1: inferior
  int pista;
  int sector;
};

class Disco {
private:
  int platos;
  int superficiesPorPlato;
  int pistasPorSuperficie;
  int sectoresPorPista;
  int sizeSector;
  int posicionActual = 0;

public:
  Disco(int _platos, int _superficiesPorPlato, int _pistasPorSuperficie,
        int _sectoresPorPista, int _sizeSector);

  // Métodos principales
  UbicacionFisica calcularUbicacion(int bytePos) const;
  void mostrarUbicacion(int bytePos) const;
  void agregarRegistro(const std::vector<int> &campos, int &inicio, int &fin);

  // Métodos para ubicaciones cruzadas
  std::vector<UbicacionFisica> calcularUbicacionesRegistro(int inicio,
                                                           int size) const;
  void mostrarUbicacionesRegistro(int inicio, int size) const;

  // Getters y utilidades
  int getPosicionActual() const;
  void reiniciar();
  int getSizeDisk() const;
  int getSectoresTotales() const;
};

#endif