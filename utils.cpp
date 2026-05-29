#include "utils.h"
#include <iostream>
#include <limits>

void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int safeInputInt(int minValue, int maxValue, const std::string& errorMessage) {
    int value;
    while (true) {
        std::cin >> value;
        if (std::cin.fail() || value < minValue || value > maxValue) {
            clearInputBuffer();
            if (!errorMessage.empty())
                std::cout << errorMessage;
            else
                std::cout << "Ошибка! Введите целое число от " << minValue << " до " << maxValue << ": ";
        }
        else {
            clearInputBuffer(); // очищаем после успешного ввода (на случай лишних символов)
            break;
        }
    }
    return value;
}