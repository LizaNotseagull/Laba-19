#include "caesar.h"     
#include "utils.h"       
#include <iostream>      
#include <string>       
#include <fstream>       
#include <windows.h>    
#include <random>        
#include <ctime>        

// Русские заглавные буквы (33 буквы, включая Ё)
static const std::string RUS_UPPER = "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";

// Русские строчные буквы (33 буквы, включая ё)
static const std::string RUS_LOWER = "абвгдеёжзийклмнопрстуфхцчшщъыьэюя";

// Латинские заглавные буквы (26 букв)
static const std::string LATIN_UPPER = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

// Латинские строчные буквы (26 букв)
static const std::string LATIN_LOWER = "abcdefghijklmnopqrstuvwxyz";

// Объединённый алфавит для случайной генерации текста
static const std::string ALL_LETTERS =
RUS_UPPER + RUS_LOWER + LATIN_UPPER + LATIN_LOWER; 

// Функция сдвига одного символа в соответствии с шифром Цезаря

static char shiftChar(char c, int shift) {
    // Поиск в русском заглавном алфавите
    size_t pos = RUS_UPPER.find(c);
    if (pos != std::string::npos) {
        // Вычисляем новую позицию с учётом сдвига и зацикливания (33 буквы)
        int newPos = (static_cast<int>(pos) + shift) % 33;
        if (newPos < 0) newPos += 33;   // Коррекция для отрицательного сдвига
        return RUS_UPPER[newPos];
    }

    // Поиск в русском строчном алфавите
    pos = RUS_LOWER.find(c);
    if (pos != std::string::npos) {
        int newPos = (static_cast<int>(pos) + shift) % 33;
        if (newPos < 0) newPos += 33;
        return RUS_LOWER[newPos];
    }

    // Поиск в латинском заглавном алфавите
    pos = LATIN_UPPER.find(c);
    if (pos != std::string::npos) {
        int newPos = (static_cast<int>(pos) + shift) % 26;
        if (newPos < 0) newPos += 26;
        return LATIN_UPPER[newPos];
    }

    // Поиск в латинском строчном алфавите
    pos = LATIN_LOWER.find(c);
    if (pos != std::string::npos) {
        int newPos = (static_cast<int>(pos) + shift) % 26;
        if (newPos < 0) newPos += 26;
        return LATIN_LOWER[newPos];
    }

    // Символ не найден ни в одном алфавите – оставляем без изменений
    return c;
}

// Функции шифрования и дешифрования целой строки

// Шифрование
static std::string caesarEncrypt(const std::string& text, int shift) {
    std::string result;
    result.reserve(text.size());
    for (char c : text) {
        result.push_back(shiftChar(c, shift));
    }
    return result;
}

// Дешифрование
static std::string caesarDecrypt(const std::string& text, int shift) {
    return caesarEncrypt(text, -shift);
}

// Генерация случайного текста

static std::string generateRandomText() {
    static std::mt19937 rng(static_cast<unsigned>(time(nullptr)));

    // Распределение длины текста (от 5 до 20 символов)
    std::uniform_int_distribution<int> lenDist(5, 20);

    // Распределение индекса в объединённом алфавите (0..117)
    std::uniform_int_distribution<size_t> charDist(0, ALL_LETTERS.size() - 1);

    int length = lenDist(rng);
    std::string result;
    result.reserve(length);

    for (int i = 0; i < length; ++i) {
        result.push_back(ALL_LETTERS[charDist(rng)]);
    }
    return result;
}

void caesarTask() {

    SetConsoleCP(1251);         
    SetConsoleOutputCP(1251);  

    std::cout << "\n=== Шифр Цезаря (русские и латинские буквы) ===\n";
    std::cout << "Выберите способ ввода:\n";
    std::cout << "1. С клавиатуры\n";
    std::cout << "2. Из файла\n";
    std::cout << "3. Случайно\n";
    std::cout << "Введите число 1-3: " << std::flush;

    int method = safeInputInt(1, 3, "");

    std::string text;

    if (method == 1) {
        std::cout << "Введите текст: " << std::flush;
        std::getline(std::cin, text);
    }
    else if (method == 2) {
        std::string filename;
        std::cout << "Имя файла: " << std::flush;
        std::cin >> filename;
        std::ifstream file(filename);
        if (file.is_open()) {
            std::getline(file, text);   
            file.close();
        }
        else {
            std::cout << "Ошибка открытия файла\n";
            return;   // выход из функции, чтобы не продолжать с пустым текстом
        }
    }
    else {
        // Случайная генерация текста
        text = generateRandomText();
        std::cout << "Случайный текст: " << text << "\n";
    }

    // Вывод исходного текста и запрос сдвига
    std::cout << "Исходный текст: " << text << "\n";
    std::cout << "Сдвиг: " << std::flush;
    int shift = safeInputInt(-1000000, 1000000, "Введите целое число: ");

    // Шифрование и дешифрование
    std::string encrypted = caesarEncrypt(text, shift);
    std::cout << "Зашифровано: " << encrypted << "\n";

    std::string decrypted = caesarDecrypt(encrypted, shift);
    std::cout << "Расшифровано: " << decrypted << "\n";
}