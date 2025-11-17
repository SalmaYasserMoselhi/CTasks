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

// Key enum for input
enum Key {
    KeyNone = 0,
    KeyUp,
    KeyDown,
    KeyLeft,
    KeyRight,
    KeyEnter,
    KeyEsc,
    KeyBackspace
};

bool drawText(int x, int y, const std::string &text);
std::string colorText(const std::string &text, Color color);
void resetColor();
void clearScreen();
void delay(int milliseconds);

// Menu API
void showMenu();                 // optional, kept for compatibility (can be no-op or simple render)
int  showMenuInteractive();      // interactive: returns -1 on Esc

#endif // TERMINAL_UTILS_H