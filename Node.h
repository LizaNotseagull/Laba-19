#ifndef NODE_H
#define NODE_H

// Узел дерева Хаффмана
class Node {
public:
    char ch;                // символ (для листьев), для внутренних узлов = 0
    int freq;               // частота (вес) узла
    Node* left;             // указатель на левого потомка
    Node* right;            // указатель на правого потомка

    // Конструктор листового узла задаём символ и частоту
    Node(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}

    // Конструктор внутреннего узла задаём сумму частот и потомков
    Node(int f, Node* l, Node* r) : ch(0), freq(f), left(l), right(r) {}
};

// Компаратор для очереди (минимальная куча)
struct Compare {
    bool operator()(Node* a, Node* b) { return a->freq > b->freq; }
};

#endif