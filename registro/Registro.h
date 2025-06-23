#ifndef REGISTROS_H
#define REGISTROS_H

#include <string>
#include <vector>

struct Head {
  std::string name;
  std::string type;
  int size;
};

class Registro {
public:
  void AddColumn(std::string name, std::string type, int size);
  Head GetHeaderByIndex(int index);
  void AddHeader(std::string name, std::string type, int size);

  void printHeader();

private:
  std::vector<Head> header;
};

#endif // REGISTROS_H