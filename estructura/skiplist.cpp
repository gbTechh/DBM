#include <iostream>
#include <vector>
#include <random>
#include <climits>

template<typename T>
class SkipList {
private:
    struct Node {
        T data;
        std::vector<Node*> forward;
        
        Node(T value, int level) : data(value), forward(level + 1, nullptr) {}
    };
    
    Node* header;
    int maxLevel;
    int currentLevel;
    std::mt19937 rng;
    std::uniform_real_distribution<double> dist;
    
    // Función para generar nivel aleatorio
    int randomLevel() {
        int level = 0;
        while (dist(rng) < 0.5 && level < maxLevel) {
            level++;
        }
        return level;
    }
    
public:
    SkipList(int maxLvl = 16) : maxLevel(maxLvl), currentLevel(0), rng(std::random_device{}()), dist(0.0, 1.0) {
        // Crear nodo header con valor mínimo
        header = new Node(T{}, maxLevel);
    }
    
    ~SkipList() {
        Node* current = header;
        while (current != nullptr) {
            Node* next = current->forward[0];
            delete current;
            current = next;
        }
    }
    
    // Insertar elemento
    void insert(T value) {
        std::vector<Node*> update(maxLevel + 1, nullptr);
        Node* current = header;
        
        // Buscar posición de inserción
        for (int i = currentLevel; i >= 0; i--) {
            while (current->forward[i] != nullptr && current->forward[i]->data < value) {
                current = current->forward[i];
            }
            update[i] = current;
        }
        
        current = current->forward[0];
        
        // Si el elemento no existe, insertarlo
        if (current == nullptr || current->data != value) {
            int newLevel = randomLevel();
            
            // Si el nuevo nivel es mayor que el actual, actualizar
            if (newLevel > currentLevel) {
                for (int i = currentLevel + 1; i <= newLevel; i++) {
                    update[i] = header;
                }
                currentLevel = newLevel;
            }
            
            // Crear nuevo nodo
            Node* newNode = new Node(value, newLevel);
            
            // Actualizar punteros
            for (int i = 0; i <= newLevel; i++) {
                newNode->forward[i] = update[i]->forward[i];
                update[i]->forward[i] = newNode;
            }
        }
    }
    
    // Buscar elemento
    bool search(T value) {
        Node* current = header;
        
        for (int i = currentLevel; i >= 0; i--) {
            while (current->forward[i] != nullptr && current->forward[i]->data < value) {
                current = current->forward[i];
            }
        }
        
        current = current->forward[0];
        return (current != nullptr && current->data == value);
    }
    
    // Eliminar elemento
    void remove(T value) {
        std::vector<Node*> update(maxLevel + 1, nullptr);
        Node* current = header;
        
        // Buscar elemento a eliminar
        for (int i = currentLevel; i >= 0; i--) {
            while (current->forward[i] != nullptr && current->forward[i]->data < value) {
                current = current->forward[i];
            }
            update[i] = current;
        }
        
        current = current->forward[0];
        
        // Si el elemento existe, eliminarlo
        if (current != nullptr && current->data == value) {
            // Actualizar punteros
            for (int i = 0; i <= currentLevel; i++) {
                if (update[i]->forward[i] != current) {
                    break;
                }
                update[i]->forward[i] = current->forward[i];
            }
            
            delete current;
            
            // Reducir nivel si es necesario
            while (currentLevel > 0 && header->forward[currentLevel] == nullptr) {
                currentLevel--;
            }
        }
    }
    
    // Mostrar la skip list
    void display() {
        std::cout << "Skip List:\n";
        for (int i = currentLevel; i >= 0; i--) {
            std::cout << "Level " << i << ": ";
            Node* current = header->forward[i];
            while (current != nullptr) {
                std::cout << current->data << " ";
                current = current->forward[i];
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }
    
    // Verificar si está vacía
    bool empty() {
        return header->forward[0] == nullptr;
    }
    
    // Obtener el tamaño (recorrido completo)
    int size() {
        int count = 0;
        Node* current = header->forward[0];
        while (current != nullptr) {
            count++;
            current = current->forward[0];
        }
        return count;
    }
};

// Función de ejemplo de uso
int main() {
    SkipList<int> skiplist(4);
    
    // Insertar elementos
    std::cout << "Insertando elementos: 3, 6, 7, 9, 12, 19, 17, 26, 21, 25\n";
    skiplist.insert(3);
    skiplist.insert(6);
    skiplist.insert(7);
    skiplist.insert(9);
    skiplist.insert(12);
    skiplist.insert(19);
    skiplist.insert(17);
    skiplist.insert(26);
    skiplist.insert(21);
    skiplist.insert(25);
    
    skiplist.display();
    
    // Buscar elementos
    std::cout << "Buscar 19: " << (skiplist.search(19) ? "Encontrado" : "No encontrado") << "\n";
    std::cout << "Buscar 15: " << (skiplist.search(15) ? "Encontrado" : "No encontrado") << "\n";
    
    // Eliminar elemento
    std::cout << "\nEliminando 19...\n";
    skiplist.remove(19);
    skiplist.display();
    
    std::cout << "Tamaño de la skip list: " << skiplist.size() << "\n";
    
    return 0;
}