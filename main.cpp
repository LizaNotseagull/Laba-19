#include <iostream>
#include <clocale>
#include "hamming.h"
#include "huffman.h"
#include "caesar.h"
#include "utils.h"

void showMenu() {
    std::cout << "\n===== ËÀÁÎÐÀÒÎÐÍÀß ÐÀÁÎÒÀ 19 =====\n";
    std::cout << "1. Êîä Õýììèíãà\n";
    std::cout << "2. Êîä Õàôôìàíà\n";
    std::cout << "3. Øèôð Öåçàðÿ\n";
    std::cout << "0. Âûõîä\n";
    std::cout << "Âûáåðèòå çàäà÷ó: ";
}

int main() {
    std::setlocale(LC_ALL, "Russian");
    int choice;
    do {
        showMenu();
        choice = safeInputInt(0, 3, "Íåâåðíûé ââîä. Âûáåðèòå 0-3: ");
        switch (choice) {
        case 1: hammingTask(); break;
        case 2: huffmanTask(); break;
        case 3: caesarTask(); break;
        case 0: std::cout << "Äî ñâèäàíèÿ!\n"; break;
        }
    } while (choice != 0);
    return 0;
}