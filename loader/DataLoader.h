#ifndef DATALOADER_H
#define DATALOADER_H

#include <string>
#include <vector>
#include "../modelo/Registro.h"
#include "../DatabaseManager.h"

class DataLoader {
public:
    void loadFromFile(const std::string& filename, DatabaseManager& dbManager);

private:
    static Registro parseLineToRecord(const std::string& line);
};

#endif // DATALOADER_H