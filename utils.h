#ifndef UTILS_H
#define UTILS_H

#include <string>

// Безопасный ввод целого числа с проверкой диапазона
int safeInputInt(int minValue, int maxValue, const std::string& errorMessage = "");

// Очистка буфера ввода
void clearInputBuffer();

#endif