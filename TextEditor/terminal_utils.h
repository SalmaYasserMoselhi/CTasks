// terminal_utils.h
#ifndef TERMINAL_UTILS_H
#define TERMINAL_UTILS_H

#include <string>

// ===== COLORS =====
enum Color
{
    Default = 0,
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

// ===== KEY CODES =====
enum Key
{
    KeyNone = 0,
    KeyUp = 1000,
    KeyDown,
    KeyLeft,
    KeyRight,
    KeyEnter,
    KeyBackspace,
    KeyEsc,
    // 🔧 الحروف العادية هترجع بـ ASCII value بتاعها (32-126)
    KeyChar = 32
};

// ===== DRAWING FUNCTIONS =====
bool drawText(int x, int y, const std::string &text);
std::string colorText(const std::string &text, Color color);
void resetColor();
void clearScreen();
void delay(int milliseconds);

// ===== RAW MODE & INPUT =====
void enableRawMode();
void disableRawMode();
bool stdinHasData(int timeoutUsec = 0);
Key readKey();

void waitForAnyKey();

void showCursor();
void hideCursor();

#endif // TERMINAL_UTILS_H