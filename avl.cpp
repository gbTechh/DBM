#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

template <class T> struct Node {
  T data;
  Node<T> *nodes[2];
  int height;
  Node(T v) {
    data = v;
    height = 1;
    nodes[0] = nodes[1] = nullptr;
  }
};

template <class T> class AVLTree {
private:
  Node<T> *root;
  T targetValue;

  int getHeight(Node<T> *n) { return n ? n->height : 0; }

  int getBalance(Node<T> *n) {
    return n ? getHeight(n->nodes[0]) - getHeight(n->nodes[1]) : 0;
  }

  void updateHeight(Node<T> *n) {
    if (n)
      n->height = 1 + max(getHeight(n->nodes[0]), getHeight(n->nodes[1]));
  }

  Node<T> *rotateLeft(Node<T> *x) {
    Node<T> *y = x->nodes[1];
    x->nodes[1] = y->nodes[0];
    y->nodes[0] = x;
    updateHeight(x);
    updateHeight(y);
    return y;
  }

  Node<T> *rotateRight(Node<T> *y) {
    Node<T> *x = y->nodes[0];
    y->nodes[0] = x->nodes[1];
    x->nodes[1] = y;
    updateHeight(y);
    updateHeight(x);
    return x;
  }

  Node<T> *insert(Node<T> *node, T value) {
    if (!node)
      return new Node<T>(value);
    if (value < node->data)
      node->nodes[0] = insert(node->nodes[0], value);
    else if (value > node->data)
      node->nodes[1] = insert(node->nodes[1], value);
    else
      return node;

    updateHeight(node);
    int balance = getBalance(node);

    if (balance > 1 && value < node->nodes[0]->data)
      return rotateRight(node);
    if (balance < -1 && value > node->nodes[1]->data)
      return rotateLeft(node);
    if (balance > 1 && value > node->nodes[0]->data) {
      node->nodes[0] = rotateLeft(node->nodes[0]);
      return rotateRight(node);
    }
    if (balance < -1 && value < node->nodes[1]->data) {
      node->nodes[1] = rotateRight(node->nodes[1]);
      return rotateLeft(node);
    }

    return node;
  }

  Node<T> *findParent(Node<T> *current, T value) {
    if (!current)
      return nullptr;
    if ((current->nodes[0] && current->nodes[0]->data == value) ||
        (current->nodes[1] && current->nodes[1]->data == value))
      return current;
    Node<T> *left = findParent(current->nodes[0], value);
    return left ? left : findParent(current->nodes[1], value);
  }

  int getNodeLevel(Node<T> *node, T value, int level) {
    if (!node)
      return -1;
    if (node->data == value)
      return level;
    int left = getNodeLevel(node->nodes[0], value, level + 1);
    return left != -1 ? left : getNodeLevel(node->nodes[1], value, level + 1);
  }

  void getAllNodesAtLevel(Node<T> *node, int level, vector<T> &list,
                          int curr = 0) {
    if (!node)
      return;
    if (curr == level) {
      list.push_back(node->data);
      return;
    }
    getAllNodesAtLevel(node->nodes[0], level, list, curr + 1);
    getAllNodesAtLevel(node->nodes[1], level, list, curr + 1);
  }

  vector<T> findUncles(T value) {
    vector<T> uncles;
    Node<T> *parent = findParent(root, value);
    if (!parent)
      return {};
    int level = getNodeLevel(root, parent->data, 0);
    getAllNodesAtLevel(root, level, uncles);
    uncles.erase(remove(uncles.begin(), uncles.end(), parent->data),
                 uncles.end());
    return uncles;
  }

  vector<T> findCousins(T value) {
    vector<T> cousins;
    vector<T> uncles = findUncles(value);
    for (T u : uncles) {
      Node<T> *uncle = findNode(root, u);
      if (uncle) {
        if (uncle->nodes[0])
          cousins.push_back(uncle->nodes[0]->data);
        if (uncle->nodes[1])
          cousins.push_back(uncle->nodes[1]->data);
      }
    }
    return cousins;
  }

  Node<T> *findNode(Node<T> *node, T value) {
    if (!node)
      return nullptr;
    if (node->data == value)
      return node;
    Node<T> *left = findNode(node->nodes[0], value);
    return left ? left : findNode(node->nodes[1], value);
  }

public:
  AVLTree() : root(nullptr), targetValue(T()) {}

  void setTargetValue(T value) { targetValue = value; }

  void insert(T value) { root = insert(root, value); }

  void drawTree(sf::RenderWindow &window, Node<T> *node, int x, int y,
                int offsetX) {
    if (!node)
      return;

    int radius = 20;

    if (node->nodes[0]) {
      sf::Vertex line[] = {sf::Vertex(sf::Vector2f(x, y)),
                           sf::Vertex(sf::Vector2f(x - offsetX, y + 100))};
      window.draw(line, 2, sf::Lines);
      drawTree(window, node->nodes[0], x - offsetX, y + 100, offsetX / 2);
    }

    if (node->nodes[1]) {
      sf::Vertex line[] = {sf::Vertex(sf::Vector2f(x, y)),
                           sf::Vertex(sf::Vector2f(x + offsetX, y + 100))};
      window.draw(line, 2, sf::Lines);
      drawTree(window, node->nodes[1], x + offsetX, y + 100, offsetX / 2);
    }

    vector<T> uncles = findUncles(targetValue);
    vector<T> cousins = findCousins(targetValue);

    sf::Color color = sf::Color::Blue;
    if (node->data == targetValue)
      color = sf::Color::Red;
    else if (find(uncles.begin(), uncles.end(), node->data) != uncles.end())
      color = sf::Color::Green;
    else if (find(cousins.begin(), cousins.end(), node->data) != cousins.end())
      color = sf::Color::Cyan;

    sf::CircleShape circle(radius);
    circle.setFillColor(color);
    circle.setPosition(x - radius, y - radius);
    window.draw(circle);

    sf::Font font;
    if (!font.loadFromFile("./arial.ttf")) {
      cout << "No se pudo cargar la fuente\n";
      return;
    }

    sf::Text text;
    text.setFont(font);
    text.setString(node->data);
    text.setCharacterSize(14);
    text.setFillColor(sf::Color::White);
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(bounds.left + bounds.width / 2,
                   bounds.top + bounds.height / 2);
    text.setPosition(x, y);
    window.draw(text);
  }

  void draw(sf::RenderWindow &window) { drawTree(window, root, 400, 50, 200); }
};

int main() {
  sf::RenderWindow window(sf::VideoMode(800, 600), "Árbol AVL Visualizado");

  AVLTree<std::string> tree;
  vector<std::string> values = {
      "nombre:Raul",     "edad:16",     "tel:4543",     "nombre:Ana",
      "edad:17",         "tel:4542",    "nombre:maria", "edad:14",
      "tel:12343",       "nombre:juan", "edad:20",      "tel:1234",
      "nombre:catalina", "edad:13",     "tel:5645",     "nombre:luis",
      "edad:30",         "tel:123876",
  };

  for (const std::string &v : values) {
    tree.insert(v);
  }

  tree.setTargetValue("edad:17"); // <-- valor string
  -+-+

     while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }

    window.clear();
    tree.draw(window);
    window.display();
  }

  return 0;
}