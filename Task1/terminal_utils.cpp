#include "terminal_utils.h"

#include <iostream>

#if defined(_WIN32)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
// Fallback for older MinGW headers:
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
#else
  #include <unistd.h>
#endif


namespace
{
    // Simple global flags instead of std::once_flag
    static bool g_inited = false;
    static bool g_ansiEnabled = false;

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

#if defined(_WIN32)
    bool enableVirtualTerminalProcessing()
    {
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hOut == INVALID_HANDLE_VALUE || hOut == nullptr)
            return false;

        DWORD dwMode = 0;
        if (!GetConsoleMode(hOut, &dwMode))
            return false;

        DWORD desired = dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        if (!SetConsoleMode(hOut, desired))
        {
            return false;
        }
        return true; // console can interpret ANSI codes
    }
#endif
} // anonymous namespace

bool initTerminal()
{
    if (!g_inited)
    {
#if defined(_WIN32)
        g_ansiEnabled = enableVirtualTerminalProcessing();
#else
        g_ansiEnabled = true;
#endif
        g_inited = true;
    }
    return g_ansiEnabled;
}

bool ansiEnabled()
{ // ensures initTerminal called
    (void)initTerminal();
    return g_ansiEnabled;
}

bool drawText(int x, int y, const std::string &text)
{
    if (!validPos(x, y))
        return false;

#if defined(_WIN32)
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE); // Console Handle. THink of it as a cardID for console
    if (hOut == INVALID_HANDLE_VALUE || hOut == nullptr)
        return false;

    CONSOLE_SCREEN_BUFFER_INFO csbi;  // Get console bufferinfo: screen size, cursor, ...
    if (!GetConsoleScreenBufferInfo(hOut, &csbi))
    {
        if (ansiEnabled())
        {
            std::cout << "\x1b[" << y << ";" << x << "H" << text;
            std::cout.flush();
            return true;
        }
        return false;
    }

    COORD pos;
    pos.X = static_cast<SHORT>(x - 1); // 0-based coordinates
    pos.Y = static_cast<SHORT>(y - 1);

    if (!SetConsoleCursorPosition(hOut, pos))
    {
        if (ansiEnabled())
        {
            std::cout << "\x1b[" << y << ";" << x << "H" << text;
            std::cout.flush();
            return true;
        }
        return false;
    }

    DWORD written = 0;  // Double word ->> WinAPI datatype. for storing no.of characters written
    if (!WriteConsoleA(hOut, text.c_str(),
                       static_cast<DWORD>(text.size()),
                       &written, nullptr))
    {
        std::cout << text;
        std::cout.flush();
        return false;
    }
    return true;
#else
    std::cout << "\x1b[" << y << ";" << x << "H" << text;
    std::cout.flush();
    return true;
#endif
}

std::string colorText(const std::string &text, Color color)
{
    if (!ansiEnabled())
        return text;

    std::string out;
    out.reserve(text.size() + 8);
    out.append(colorToAnsi(color));
    out.append(text);
    out.append("\x1b[0m");
    return out;  // ex: "\x1b[31mTEXT\x1b[0m"  ->> Red TEXT
}

void resetColor()
{
    if (ansiEnabled())
    {
        std::cout << "\x1b[0m";
        std::cout.flush();
    }
}

void clearScreen()
{
    if (ansiEnabled())
    {
        std::cout << "\x1b[2J\x1b[H";
        std::cout.flush();
    }
}

void delay(int milliseconds)
{
#if defined(_WIN32)
    Sleep(milliseconds); // Windows: Sleep بالـ milliseconds
#else
    sleep(milliseconds / 1000); 
#endif
}
