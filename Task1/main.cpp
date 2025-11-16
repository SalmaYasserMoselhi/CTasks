#include <iostream>


#include "terminal_utils.h"

int main() {
    initTerminal();

    clearScreen();

    std::string title = colorText("Terminal Demo", Red);
    drawText(5, 5, title);
    drawText(10, 10, colorText("Hello in Cyan", Cyan));
    drawText(15, 15, colorText("Hello in green", Green));
    drawText(20, 20, "Plain default text");

    delay(5000);
    resetColor();
    clearScreen();

    std::cout << "\n";
    return 0;
}