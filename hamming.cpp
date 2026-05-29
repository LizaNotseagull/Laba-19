#include "hamming.h"
#include "input_helper.h"
#include "utils.h"
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <windows.h>
#include <iomanip>

// Вывод строки битов в виде таблицы с номерами позиций
static void printBitsTable(const std::vector<int>& bits, int n, const std::string& title = "") {
    if (!title.empty()) std::cout << title << "\n";
    // Шапка с номерами
    std::cout << "    ";
    for (int i = 1; i <= n; ++i) {
        std::cout << std::setw(3) << i;
    }
    std::cout << "\n    ";
    for (int i = 1; i <= n; ++i) {
        std::cout << "___";
    }
    std::cout << "\n    ";
    for (int i = 1; i <= n; ++i) {
        if (bits[i] == -1) std::cout << "  _";
        else std::cout << "  " << bits[i];
    }
    std::cout << "\n\n";
}

// Вычисление контрольных битов и вывод подробностей
static std::vector<int> buildHammingCodeWithDetails(const std::string& dataBits, std::vector<int>& codeBits, int& n, int& r) {
    int m = dataBits.length();
    r = 0;
    while ((1 << r) < m + r + 1) ++r;
    n = m + r;

    // Инициализируем вектор битов (1-индексация), -1 = не определено
    std::vector<int> bits(n + 1, -1);
    int dataIdx = 0;
    for (int i = 1; i <= n; ++i) {
        if ((i & (i - 1)) == 0) {
            bits[i] = -1;  // контрольный бит, пока не вычислен
        }
        else {
            bits[i] = dataBits[dataIdx++] - '0';
        }
    }

    // Вывод таблицы с размещёнными данными
    printBitsTable(bits, n, "Размещение битов данных:");

    // Вычисление контрольных битов
    std::vector<int> controlPositions;
    for (int p = 1; p <= n; p <<= 1) controlPositions.push_back(p);

    std::cout << "Вычисление контрольных битов:\n";
    for (int p : controlPositions) {
        // Собираем позиции, которые контролирует p
        std::vector<int> positions;
        for (int j = p; j <= n; ++j) {
            if (j & p) positions.push_back(j);
        }
        // Суммируем биты (кроме самого p, он пока -1, считаем как 0)
        int sum = 0;
        for (int j : positions) {
            if (j == p) continue; // сам контрольный бит пока не участвует
            if (bits[j] == 1) sum ^= 1; // XOR для чётности
        }
        bits[p] = sum; // если нечётная сумма, ставим 1
        // Вывод формулы
        std::cout << "C" << p << " = ";
        bool first = true;
        for (int j : positions) {
            if (j == p) continue;
            if (!first) std::cout << " + ";
            std::cout << bits[j];
            first = false;
        }
        std::cout << " = " << sum << "\n";
    }

    // Вывод итоговой таблицы
    printBitsTable(bits, n, "Итоговое кодовое слово Хэмминга:");
    codeBits = bits;
    return controlPositions;
}

// Вычисление синдрома с подробным выводом
static int computeSyndromeWithDetails(const std::vector<int>& bits, const std::vector<int>& controlPositions, int n) {
    std::cout << "Расчёт синдромов:\n";
    int syndrome = 0;
    for (int p : controlPositions) {
        // Собираем позиции, которые контролирует p
        std::vector<int> positions;
        for (int j = p; j <= n; ++j) {
            if (j & p) positions.push_back(j);
        }
        // Суммируем все биты в группе (включая сам контрольный)
        int sum = 0;
        for (int j : positions) {
            if (bits[j] == 1) sum ^= 1;
        }
        std::cout << "s" << p << " = ";
        bool first = true;
        for (int j : positions) {
            if (!first) std::cout << " + ";
            std::cout << bits[j];
            first = false;
        }
        std::cout << " = " << sum;
        if (sum != 0) {
            std::cout << " -> нарушение, добавляем " << p;
            syndrome += p;
        }
        std::cout << "\n";
        // Вывод списка позиций группы
        std::cout << "        v" << p << " = {";
        for (size_t i = 0; i < positions.size(); ++i) {
            if (i > 0) std::cout << ", ";
            std::cout << positions[i];
        }
        std::cout << "}\n";
    }
    return syndrome;
}

void hammingTask() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    std::cout << "\n=== Код Хэмминга (обнаружение и исправление одиночной ошибки) ===\n";
    std::cout << "1. С клавиатуры\n2. Из файла\n3. Случайно\n\nВыбери: ";
    int method = safeInputInt(1, 3, "");
    std::string dataBits = getInputForHamming(method);

    std::cout << "\nВведи двоичное сообщение (только 0 и 1): " << dataBits << "\n";

    int n, r;
    std::vector<int> codeBits;
    std::vector<int> controlPositions = buildHammingCodeWithDetails(dataBits, codeBits, n, r);

    std::cout << "Исходное сообщение (" << dataBits.length() << " бит): " << dataBits << "\n";
    std::cout << "Контрольных битов: " << r << "\n";
    std::cout << "Общая длина кода: " << n << "\n\n";

    // Имитация ошибки
    std::cout << "Введи номер бита для ошибочки: ";
    int errorBit = safeInputInt(1, n, "Введите число от 1 до " + std::to_string(n) + ": ");

    std::vector<int> received = codeBits;
    if (received[errorBit] == 0) received[errorBit] = 1;
    else received[errorBit] = 0;

    std::cout << "\nПосле внесения ошибки:\n";
    printBitsTable(received, n, "");

    // Вычисление синдрома
    int syndrome = computeSyndromeWithDetails(received, controlPositions, n);
    std::cout << "\nСиндром = " << syndrome;
    if (syndrome != 0) {
        std::cout << " - ошибка в бите " << syndrome << "\n";
        // Исправляем
        received[syndrome] = (received[syndrome] == 0) ? 1 : 0;
        std::cout << "\nИсправленное слово:\n";
        printBitsTable(received, n, "");
        // Извлекаем исходные биты
        std::string restored;
        for (int i = 1; i <= n; ++i) {
            if ((i & (i - 1)) != 0) { // не степень двойки
                restored.push_back(received[i] + '0');
            }
        }
        std::cout << "Восстановленные исходные биты: " << restored << "\n";
    }
    else {
        std::cout << " - ошибок не обнаружено.\n";
    }
}