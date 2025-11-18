// terminal_utils.cpp
#include "terminal_utils.h"

#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <cstdlib>

#if defined(_WIN32)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#else
#include <fcntl.h>
#endif
#include <unistd.h>

namespace
{
    const char *colorToAnsi(Color c)
    {
        switch (c)
        {
        case Black:
            return "\x1b[30m";
        case Red:
            return "\x1b[31m";
        case Green:
            return "\x1b[32m";
        case Yellow:
            return "\x1b[33m";
        case Blue:
            return "\x1b[34m";
        case Magenta:
            return "\x1b[35m";
        case Cyan:
            return "\x1b[36m";
        case White:
            return "\x1b[37m";
        case BrightBlack:
            return "\x1b[90m";
        case BrightRed:
            return "\x1b[91m";
        case BrightGreen:
            return "\x1b[92m";
        case BrightYellow:
            return "\x1b[93m";
        case BrightBlue:
            return "\x1b[94m";
        case BrightMagenta:
            return "\x1b[95m";
        case BrightCyan:
            return "\x1b[96m";
        case BrightWhite:
            return "\x1b[97m";
        case Default:
        default:
            return "\x1b[39m";
        }
    }

    inline bool validPos(int x, int y)
    {
        return x >= 1 && y >= 1; // 1-based coordinates
    }
}

// Pure ANSI cursor move + print
bool drawText(int x, int y, const std::string &text)
{
    if (!validPos(x, y))
        return false;
    std::cout << "\x1b[" << y << ";" << x << "H" << text;
    std::cout.flush();
    return true;
}

std::string colorText(const std::string &text, Color color)
{
    std::string out;
    out.reserve(text.size() + 8);
    out.append(colorToAnsi(color));
    out.append(text);
    out.append("\x1b[0m");
    return out;
}

void resetColor()
{
    std::cout << "\x1b[0m";
    std::cout.flush();
}

void clearScreen()
{
    // Clear screen, clear scrollback, move home
    std::cout << "\x1b[H\x1b[2J\x1b[3J\x1b[H";
    std::cout.flush();
}

// Keep the original delay behavior per your request.
void delay(int milliseconds)
{
#if defined(_WIN32)
    Sleep(milliseconds);
#else
    usleep(milliseconds * 1000);
#endif
}
