#ifndef TERMINAL_UTILS_H
#define TERMINAL_UTILS_H

#include <string>

enum Color
{
    Default,
    Black,
    Red,
    Green,
    Yellow,
    Blue,
    Magenta,
    Cyan,
    White,
    BrightBlack,
    BrightRed,
    BrightGreen,
    BrightYellow,
    BrightBlue,
    BrightMagenta,
    BrightCyan,
    BrightWhite
};

bool initTerminal();
bool drawText(int x, int y, const std::string &text);
std::string colorText(const std::string &text, Color color);
void resetColor();
bool ansiEnabled();
void clearScreen();
void delay(int milliseconds);

// Menu API
void showMenu();                 // non-interactive render (optional)
int  showMenuInteractive();      // interactive: returns -1 on Esc

#endif // TERMINAL_UTILS_H