#include <iostream>
#include "terminal_utils.h"
#include "menu.h"

int main()
{
    atexit(disableRawMode); // أمان إضافي
    
    showCursor();
    resetColor();
    clearScreen();

    int result = showMenuInteractive();

    disableRawMode();
    showCursor();
    resetColor();
    clearScreen();
    std::cout << "\n";
    return 0;
}