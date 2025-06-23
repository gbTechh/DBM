# Makefile para proyecto C++ con varias carpetas

CXX = g++
CXXFLAGS = -std=c++17 -g -Wall

# Buscar todos los archivos .cpp en subdirectorios
SRC = main.cpp \
      $(wildcard disco/*.cpp) \
      $(wildcard file/*.cpp) \
      $(wildcard indices/*.cpp) \
      $(wildcard loader/*.cpp) \
      $(wildcard manager/*.cpp) \
      $(wildcard registro/*.cpp) \
      $(wildcard sql/*.cpp)
      # $(wildcard estructura/*.cpp)

# Nombre del ejecutable
TARGET = main

# Regla principal
all: $(TARGET)

# Regla para crear el ejecutable
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Limpiar archivos generados
clean:
	rm -f $(TARGET)

.PHONY: all clean
