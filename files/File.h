#ifndef FILE_H  
#define FILE_H

#include <fstream>
#include <string>
#include <stdexcept>  
class File {
private:
    std::string filename;
    std::ifstream file;

public:
    explicit File(const std::string& _filename);
    
    ~File();
    
    File(const File&) = delete;
    File& operator=(const File&) = delete;
    
    std::string ReadHead();
    bool ReadLine(std::string& line);
    
    bool IsOpen() const { return file.is_open(); }
    
    // Método para reiniciar la lectura
    void Reset();
};

#endif