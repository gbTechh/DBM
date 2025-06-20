#ifndef REGISTROS_H
#define REGISTROS_H

#include <string>
#include <vector>

struct Head {
  int name;
  std::string type;
  int size;
}

class Registros {
public:
  void AddColumn(int name, std::string type, int size);
  Head GetColumnIndex(int index);
private:
  std::vector<Head> header;
};

#endif // REGISTROS_H