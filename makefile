# Makefile para proyecto C++ con varias carpetas

CXX = g++
CXXFLAGS = -std=c++17 -g -Wall

# Directorios de origen
SRC_DIRS = front disco file indices loader manager registro sql

# Buscar todos los archivos .cpp en los directorios especificados
SRC = front/servidor.cpp \
      $(wildcard disco/*.cpp) \
      $(wildcard file/*.cpp) \
      $(wildcard indices/*.cpp) \
      $(wildcard loader/*.cpp) \
      $(wildcard manager/*.cpp) \
      $(wildcard registro/*.cpp) \
      $(wildcard sql/*.cpp)

# Nombre del ejecutable
TARGET = servidor

# Regla principal
all: $(TARGET)

# Regla para crear el ejecutable
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Limpiar archivos generados
clean:
	rm -f $(TARGET)

.PHONY: all clean