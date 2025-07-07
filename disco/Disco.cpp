// Disco.cpp
#include "Disco.h"
#include <cstring>
#include <iostream>
#include <stdexcept>

Disco::Disco(int _platos, int _superficiesPorPlato, int _pistasPorSuperficie,
             int _sectoresPorPista, int _sizeSector)
    : platos(_platos), superficiesPorPlato(_superficiesPorPlato),
      pistasPorSuperficie(_pistasPorSuperficie),
      sectoresPorPista(_sectoresPorPista), sizeSector(_sizeSector) {

  if (platos <= 0 || superficiesPorPlato <= 0 || pistasPorSuperficie <= 0 ||
      sectoresPorPista <= 0 || sizeSector <= 0)
    throw std::invalid_argument("Todos los parámetros deben ser positivos");
}

UbicacionFisica Disco::calcularUbicacion(int bytePos) {
  UbicacionFisica ubicacion;

  // 1. Calcular sector absoluto
  int sectorAbs = bytePos / sizeSector;

  // 2. Calcular sectores por cilindro (pistas * superficies)
  int sectoresPorCilindro = sectoresPorPista * superficiesPorPlato * platos;

  // 3. Calcular cilindro (pista)
  ubicacion.pista = sectorAbs / sectoresPorCilindro;

  // 4. Sector restante dentro del cilindro
  int sectorEnCilindro = sectorAbs % sectoresPorCilindro;

  // 5. Determinar plato y superficie
  int sectoresPorPlato = sectoresPorPista * superficiesPorPlato;
  ubicacion.plato = sectorEnCilindro / sectoresPorPlato;
  int sectorEnPlato = sectorEnCilindro % sectoresPorPlato;

  ubicacion.superficie = sectorEnPlato / sectoresPorPista;
  ubicacion.sector = sectorEnPlato % sectoresPorPista;

  return ubicacion;
}

void Disco::mostrarUbicacion(int bytePos) {
  UbicacionFisica u = calcularUbicacion(bytePos);
  std::cout << "Byte " << bytePos << " está en:\n"
            << "Plato: " << u.plato << "\n"
            << "Superficie: " << (u.superficie == 0 ? "Superior" : "Inferior")
            << "\n"
            << "Pista: " << u.pista << "\n"
            << "Sector: " << u.sector << "\n";
}

void Disco::agregarRegistro(const std::vector<int> &campos, int &inicio,
                            int &fin) {
  inicio = posicionActual;
  int byteActual = posicionActual;
  int sectorLimit = ((byteActual / sizeSector) + 1) * sizeSector;

  for (size_t i = 0; i < campos.size(); ++i) {
    int campo = campos[i];

    if (byteActual + campo > sectorLimit) {
      // Ajustar al siguiente sector
      byteActual = sectorLimit;
      sectorLimit += sizeSector;
    }
    byteActual += campo;
  }

  fin = byteActual;
  posicionActual =
      byteActual; // Actualizamos la posición para el próximo registro
}

// Método para obtener la posición actual
int Disco::getPosicionActual() const { return posicionActual; }

// Método para reiniciar la posición (opcional)
void Disco::reiniciar() { posicionActual = 0; }

int Disco::getSizeDisk() const {
  return platos * superficiesPorPlato * pistasPorSuperficie * sectoresPorPista *
         sizeSector;
}