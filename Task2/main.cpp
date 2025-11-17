#include <iostream>
#include "terminal_utils.h"

int main() {
    int result = showMenuInteractive();

    resetColor();
    clearScreen();
    std::cout << "\n";
    return 0;
}