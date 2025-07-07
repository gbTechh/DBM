#include "Disco.h"
#include <iostream>
#include <stdexcept>
#include <vector>

Disco::Disco(int _platos, int _superficiesPorPlato, int _pistasPorSuperficie,
             int _sectoresPorPista, int _sizeSector)
    : platos(_platos), superficiesPorPlato(_superficiesPorPlato),
      pistasPorSuperficie(_pistasPorSuperficie),
      sectoresPorPista(_sectoresPorPista), sizeSector(_sizeSector) {

  if (platos <= 0 || superficiesPorPlato <= 0 || pistasPorSuperficie <= 0 ||
      sectoresPorPista <= 0 || sizeSector <= 0) {
    throw std::invalid_argument("Todos los parámetros deben ser positivos");
  }
}

UbicacionFisica Disco::calcularUbicacion(int bytePos) const {
  UbicacionFisica ubicacion;

  // 1. Calcular sector absoluto (indexado desde 0)
  int sectorAbs = bytePos / sizeSector;

  // 2. Calcular sectores por pista-superficie (sectores en una superficie
  // completa)
  int sectoresPorPistaSuperficie = sectoresPorPista * pistasPorSuperficie;

  // 3. Calcular sectores por cilindro (todas superficies en mismo radio)
  int sectoresPorCilindro =
      sectoresPorPistaSuperficie * superficiesPorPlato * platos;

  // 4. Determinar el cilindro (conjunto de pistas en mismo radio)
  ubicacion.pista = sectorAbs / sectoresPorCilindro;

  // 5. Sector relativo dentro del cilindro
  int sectorEnCilindro = sectorAbs % sectoresPorCilindro;

  // 6. Determinar plato específico
  int sectoresPorPlato = sectoresPorPistaSuperficie * superficiesPorPlato;
  ubicacion.plato = sectorEnCilindro / sectoresPorPlato;

  // 7. Sector relativo dentro del plato
  int sectorEnPlato = sectorEnCilindro % sectoresPorPlato;

  // 8. Determinar superficie específica (0 = superior, 1 = inferior)
  ubicacion.superficie = sectorEnPlato / sectoresPorPistaSuperficie;

  // 9. Sector relativo dentro de la superficie
  int sectorEnSuperficie = sectorEnPlato % sectoresPorPistaSuperficie;

  // 10. Determinar pista exacta y sector final
  ubicacion.pista =
      sectorEnSuperficie / sectoresPorPista; // Pista dentro de la superficie
  ubicacion.sector =
      sectorEnSuperficie % sectoresPorPista; // Sector dentro de la pista

  return ubicacion;
}

std::vector<UbicacionFisica>
Disco::calcularUbicacionesRegistro(int inicio, int size) const {
  std::vector<UbicacionFisica> ubicaciones;
  int byteActual = inicio;
  int byteFinal = inicio + size;

  while (byteActual < byteFinal) {
    UbicacionFisica u = calcularUbicacion(byteActual);
    ubicaciones.push_back(u);

    // Avanzar al siguiente sector
    int sectorActual = byteActual / sizeSector;
    int proximoSector = sectorActual + 1;
    byteActual = proximoSector * sizeSector;
  }

  return ubicaciones;
}

void Disco::mostrarUbicacionesRegistro(int inicio, int size) const {
  auto ubicaciones = calcularUbicacionesRegistro(inicio, size);
  std::cout << "El registro ocupa " << size << " bytes desde " << inicio
            << " hasta " << (inicio + size - 1) << " en:\n";

  for (const auto &u : ubicaciones) {
    std::cout << " - Plato " << u.plato << ", Superficie "
              << (u.superficie == 0 ? "Superior" : "Inferior") << ", Pista "
              << u.pista << ", Sector " << u.sector << "\n";
  }
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
  posicionActual = byteActual;
}

void Disco::mostrarUbicacion(int bytePos) const {
  UbicacionFisica u = calcularUbicacion(bytePos);
  std::cout << "Byte " << bytePos << " está en:\n"
            << "Plato: " << u.plato << "\n"
            << "Superficie: " << (u.superficie == 0 ? "Superior" : "Inferior")
            << "\n"
            << "Pista: " << u.pista << "\n"
            << "Sector: " << u.sector << "\n";
}

int Disco::getPosicionActual() const { return posicionActual; }

void Disco::reiniciar() { posicionActual = 0; }

int Disco::getSizeDisk() const {
  return platos * superficiesPorPlato * pistasPorSuperficie * sectoresPorPista *
         sizeSector;
}

int Disco::getSectoresTotales() const {
  return platos * superficiesPorPlato * pistasPorSuperficie * sectoresPorPista;
}