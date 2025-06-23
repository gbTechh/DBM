#ifndef DATALOADER_H
#define DATALOADER_H

#include "../manager/DatabaseManager.h"
#include "../registro/Registro.h"
#include <string>
#include <vector>

class DataLoader {
public:
  DataLoader();
  void loadFromFile(const std::string &filename, DatabaseManager &dbManager);
  void getColumnHead(std::string line);
  std::vector<std::string> split(const std::string &str, char delimiter);

private:
  // static Registro parseLineToRecord(const std::string& line);
};

#endif // DATALOADER_H