#ifndef ATRIBUTO_H
#define ATRIBUTO_H

#include <string>
using namespace std;
#include "../utils/DataTypeInfo.h"  

class Atributo {
public:
    DataTypeInfo tipo; 
    std::string valor;
    Atributo() : tipo(DataType::INT, 0), valor("") {}

    Atributo(DataType type, const std::string& valor, int varcharSize = 0) 
        : tipo(type, varcharSize), valor(valor) {}  

    //Atributo(const std::string& tipoStr, const std::string& valor);

    int obtenerTamanio() const { return tipo.size; } 
    std::string getTipoString() const { return tipo.toString(); }
    std::string getValor() const { return valor; }
    void setValor(const std::string& nuevoValor) { valor = nuevoValor; }

private:
    DataTypeInfo convertirStringATipo(const string& tipoStr);  
};

#endif
