#include <iostream>
#include "terminal_utils.h"

int main() {
    initTerminal();

    int result = showMenuInteractive();

    resetColor();
    clearScreen();
    std::cout << "\n";
    return 0;
}