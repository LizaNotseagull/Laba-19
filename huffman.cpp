#include "huffman.h"
#include "input_helper.h"
#include "utils.h"
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <vector>
#include <iomanip>
#include <cmath>
#include <windows.h>
#include <algorithm>

// Узел дерева Хаффмана
class Node {
public:
    char ch;          // символ (для листьев), для внутренних = 0
    int freq;         // частота (вес) узла
    Node* left;       // указатель на левого потомка
    Node* right;      // указатель на правого потомка

    // Конструктор для листа
    Node(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
    // Конструктор для внутреннего узла
    Node(int f, Node* l, Node* r) : ch(0), freq(f), left(l), right(r) {}
};

// Компаратор для очереди (мин-куча)
struct Compare {
    bool operator()(Node* a, Node* b) { return a->freq > b->freq; }
};

// Рекурсивная генерация кодов
static void generateCodes(Node* root, const std::string& code, std::map<char, std::string>& codes) {
    if (!root) return;
    if (!root->left && !root->right) {          // лист
        codes[root->ch] = code;
        return;
    }
    generateCodes(root->left, code + "0", codes);
    generateCodes(root->right, code + "1", codes);
}

// Вывод дерева в горизонтальном стиле (повёрнутое)
static void printTree(Node* root, int depth = 0) {
    if (!root) return;
    printTree(root->right, depth + 1);                     // сначала правое поддерево
    for (int i = 0; i < depth; ++i) std::cout << "    ";   // отступы
    if (!root->left && !root->right)
        std::cout << root->freq << ":< '" << root->ch << "'\n";
    else
        std::cout << root->freq << ":<\n";
    printTree(root->left, depth + 1);                      // затем левое
}

// Удаление дерева
static void deleteTree(Node* root) {
    if (!root) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

// Формирование закодированной строки (группы по 8 бит)
static std::string getEncodedString(const std::string& text, const std::map<char, std::string>& codes) {
    std::string bits;
    for (char c : text) bits += codes.at(c);
    std::string result;
    for (size_t i = 0; i < bits.size(); i += 8) {
        if (i > 0) result += " ";
        result += bits.substr(i, 8);
    }
    return result;
}

void huffmanTask() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    std::cout << "\n=== Код Хаффмана ===\n";
    std::cout << "1. С клавиатуры\n2. Случайно\n3. Из файла\n\nВыбери: ";
    int method = safeInputInt(1, 3, "");
    std::string text = getInputForHuffman(method);
    std::cout << "\nИсходная строка: \"" << text << "\"\n\n";

    // 1. Частоты символов
    std::map<char, int> freq;
    for (char c : text) freq[c]++;
    int totalChars = text.length();

    // 2. Построение дерева
    std::priority_queue<Node*, std::vector<Node*>, Compare> pq;
    for (auto& p : freq) pq.push(new Node(p.first, p.second));
    while (pq.size() > 1) {
        Node* l = pq.top(); pq.pop();
        Node* r = pq.top(); pq.pop();
        pq.push(new Node(l->freq + r->freq, l, r));
    }
    Node* root = pq.top();

    // 3. Генерация кодов
    std::map<char, std::string> codes;
    generateCodes(root, "", codes);

    // 4. Подготовка данных для таблицы (сортировка по возрастанию частоты)
    std::vector<std::tuple<int, char, std::string, int>> entries;
    for (auto& p : freq) {
        entries.push_back({ p.second, p.first, codes[p.first], codes[p.first].length() });
    }
    std::sort(entries.begin(), entries.end());

    // 5. Вывод таблицы (символ, частота, вероятность, код, длина)
    std::cout << "Таблица кодов:\n";
    std::cout << std::setw(10) << "Символ"
        << std::setw(10) << "Частота"
        << std::setw(12) << "Вероятность"
        << std::setw(12) << "Код"
        << std::setw(10) << "Длина" << "\n";
    for (auto& entry : entries) {
        int f = std::get<0>(entry);
        char ch = std::get<1>(entry);
        std::string code = std::get<2>(entry);
        int len = std::get<3>(entry);
        double prob = (double)f / totalChars;
        if (ch == ' ')
            std::cout << std::setw(10) << "' '";
        else
            std::cout << std::setw(10) << ch;
        std::cout << std::setw(10) << f
            << std::setw(12) << std::fixed << std::setprecision(4) << prob
            << std::setw(12) << code
            << std::setw(10) << len << "\n";
    }

    // 6. Вывод дерева
    std::cout << "\nДерево кодирования\n";
    printTree(root);

    // 7. Статистика (только размеры)
    int uniqueChars = freq.size();
    int bitsPerCharUniform = std::ceil(std::log2(uniqueChars));
    int uniformSize = totalChars * bitsPerCharUniform;
    int huffmanSize = 0;
    for (char c : text) huffmanSize += codes[c].length();

    std::cout << "\nУникальных символов: " << uniqueChars << "\n";
    std::cout << "Бит на символ (равномерно): " << bitsPerCharUniform << "\n";
    std::cout << "Равномерное: " << uniformSize << " бит\n";
    std::cout << "Код Хаффмана: " << huffmanSize << " бит\n";
    std::cout << "Разность: " << (uniformSize - huffmanSize) << " бит\n";

    // 8. Закодированная строка
    std::string encoded = getEncodedString(text, codes);
    std::cout << "\nЗакодированная битовая строка:\n" << encoded << "\n";

    // 9. Раскодированная (исходная) строка для проверки
    std::cout << "Раскодированная строка: \"" << text << "\"\n";

    deleteTree(root);
}