#include <iostream>
#include "terminal_utils.h"

int main() {
    clearScreen();

    std::string title = colorText("Draw in Terminal Demo", Red);
    drawText(5, 5, title);
    drawText(10, 10, colorText("This is C/C++ Course", Cyan));
    drawText(15, 15, colorText("ENG: MINA IS THE BEST", BrightYellow));
    delay(5000);

    resetColor();
    clearScreen();

    std::cout << "\n";
    return 0;
}