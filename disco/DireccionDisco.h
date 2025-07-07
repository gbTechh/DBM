#ifndef DISCO_DIRECCION_H
#define DISCO_DIRECCION_H

struct DireccionDisco {
  int plato;
  int superficie;
  int pista;
  int sector;

  bool operator==(const DireccionDisco &other) const {
    return plato == other.plato && superficie == other.superficie &&
           pista == other.pista && sector == other.sector;
  }
};

#endif