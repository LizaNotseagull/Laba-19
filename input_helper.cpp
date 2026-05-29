#include "input_helper.h"
#include "utils.h"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <random>
#include <ctime>

static std::random_device rd;
static std::mt19937 gen(rd());

std::string getInputForHamming(int method) {
    std::string data;
    bool ok = false;
    while (!ok) {
        switch (method) {
        case 1:
            std::cout << "Введите двоичную строку (только 0 и 1): ";
            std::cin >> data;
            // Удаляем все символы, не являющиеся 0 или 1
            data.erase(std::remove_if(data.begin(), data.end(),
                [](char c) { return c != '0' && c != '1'; }), data.end());
            if (data.empty()) {
                std::cout << "Строка не должна быть пустой и должна содержать только 0 и 1. Попробуйте снова.\n";
                continue;
            }
            if (data.length() > 1000) {
                std::cout << "Строка слишком длинная (максимум 1000 бит). Попробуйте снова.\n";
                continue;
            }
            ok = true;
            break;
        case 2: {
            std::string filename;
            std::cout << "Введите имя файла: ";
            std::cin >> filename;
            std::ifstream file(filename);
            if (file.is_open()) {
                std::getline(file, data);
                file.close();
                data.erase(std::remove_if(data.begin(), data.end(),
                    [](char c) { return c != '0' && c != '1'; }), data.end());
                if (data.empty() || data.length() > 1000) {
                    std::cout << "Файл содержит некорректные данные (пустой или слишком длинный). Используйте другой файл.\n";
                    continue;
                }
                ok = true;
            }
            else {
                std::cout << "Не удалось открыть файл. Попробуйте другой файл.\n";
                continue;
            }
            break;
        }
        case 3: {
            std::uniform_int_distribution<> lenDist(8, 20);
            int len = lenDist(gen);
            std::uniform_int_distribution<> bitDist(0, 1);
            data.reserve(len);
            for (int i = 0; i < len; ++i)
                data.push_back(bitDist(gen) ? '1' : '0');
            std::cout << "Сгенерирована случайная двоичная строка: " << data << "\n";
            ok = true;
            break;
        }
        default:
            data = "001010000101";
            ok = true;
        }
    }
    return data;
}

std::string getInputForHuffman(int method) {
    std::string text;
    bool ok = false;
    while (!ok) {
        switch (method) {
        case 1:
            std::cout << "Введите текст: ";
            std::getline(std::cin, text);
            if (text.empty()) {
                std::cout << "Текст не может быть пустым. Попробуйте снова.\n";
                continue;
            }
            if (text.length() > 10000) {
                std::cout << "Текст слишком длинный (максимум 10000 символов).\n";
                continue;
            }
            ok = true;
            break;
        case 2: {
            std::string filename;
            std::cout << "Введите имя файла: ";
            std::cin >> filename;
            std::ifstream file(filename);
            if (file.is_open()) {
                std::getline(file, text);
                file.close();
                if (text.empty()) {
                    std::cout << "Файл пуст. Используйте другой файл.\n";
                    continue;
                }
                if (text.length() > 10000) {
                    std::cout << "Текст в файле слишком длинный (максимум 10000 символов).\n";
                    continue;
                }
                ok = true;
            }
            else {
                std::cout << "Не удалось открыть файл. Попробуйте другой файл.\n";
                continue;
            }
            break;
        }
        case 3: {
            std::uniform_int_distribution<> lenDist(10, 30);
            int len = lenDist(gen);
            const std::string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";
            std::uniform_int_distribution<> charDist(0, chars.size() - 1);
            text.reserve(len);
            for (int i = 0; i < len; ++i)
                text.push_back(chars[charDist(gen)]);
            std::cout << "Сгенерирован случайный текст: " << text << "\n";
            ok = true;
            break;
        }
        default:
            text = "У ЁЛКИ ИГОЛКИ КОЛКИ";
            ok = true;
        }
    }
    return text;
}

std::string getInputForCaesar(int method) {
    std::string text;
    bool ok = false;
    while (!ok) {
        switch (method) {
        case 1:
            std::cin.ignore();
            std::cout << "Введите текст для шифрования: ";
            std::getline(std::cin, text);
            // Пустой текст разрешён (ничего шифровать)
            ok = true;
            break;
        case 2: {
            std::string filename;
            std::cout << "Введите имя файла: ";
            std::cin >> filename;
            std::ifstream file(filename);
            if (file.is_open()) {
                std::getline(file, text);
                file.close();
                ok = true;
            }
            else {
                std::cout << "Не удалось открыть файл. Попробуйте другой файл.\n";
                continue;
            }
            break;
        }
        case 3: {
            std::uniform_int_distribution<> lenDist(10, 30);
            int len = lenDist(gen);
            const std::string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
            std::uniform_int_distribution<> charDist(0, chars.size() - 1);
            text.reserve(len);
            for (int i = 0; i < len; ++i)
                text.push_back(chars[charDist(gen)]);
            std::cout << "Сгенерирован случайный текст: " << text << "\n";
            ok = true;
            break;
        }
        default:
            text = "HELLOWORLD";
            ok = true;
        }
    }
    return text;
}