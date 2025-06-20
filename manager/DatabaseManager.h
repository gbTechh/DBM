#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include "../disco/Disco.h"
#include "../files/File.h"

class DbManager {
  private:
    Disco disco;
    File file;
  
  public:
    DbManager(int platos = 2, int pistas = 100, int sectores = 8, int tamanioSector = 100);
    explicit DbManager(const std::string& filename); 

    const Disco& getDisco() const { return disco; }
    const File& getFile() const { return file; }

    void ReadFile(const std::string& filename);
};

#endif 