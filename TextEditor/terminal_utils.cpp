#include "terminal_utils.h"

#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <unistd.h>

#if defined(_WIN32)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#else
#include <fcntl.h>
#include <termios.h>
#include <sys/select.h>
#include <cstdio>
#endif

namespace
{
    const char *colorToAnsi(Color c)
    {
        switch (c)
        {
        case Black: return "\x1b[30m";
        case Red: return "\x1b[31m";
        case Green: return "\x1b[32m";
        case Yellow: return "\x1b[33m";
        case Blue: return "\x1b[34m";
        case Magenta: return "\x1b[35m";
        case Cyan: return "\x1b[36m";
        case White: return "\x1b[37m";
        case BrightBlack: return "\x1b[90m";
        case BrightRed: return "\x1b[91m";
        case BrightGreen: return "\x1b[92m";
        case BrightYellow: return "\x1b[93m";
        case BrightBlue: return "\x1b[94m";
        case BrightMagenta: return "\x1b[95m";
        case BrightCyan: return "\x1b[96m";
        case BrightWhite: return "\x1b[97m";
        default: return "\x1b[39m";
        }
    }

    inline bool validPos(int x, int y)
    {
        return x >= 1 && y >= 1; // 1‑based
    }
}

// ====== BASIC OUTPUT ======
bool drawText(int x, int y, const std::string &text)
{
    if (!validPos(x, y)) return false;
    std::cout << "\x1b[" << y << ";" << x << "H" << text;
    std::cout.flush();
    return true;
}

std::string colorText(const std::string &text, Color color)
{
    return std::string(colorToAnsi(color)) + text + "\x1b[0m";
}

void resetColor()
{
    std::cout << "\x1b[0m";
    std::cout.flush();
}

void clearScreen()
{
    std::cout << "\x1b[H\x1b[2J\x1b[3J\x1b[H";
    std::cout.flush();
}

void delay(int ms)
{
#if defined(_WIN32)
    Sleep(ms);
#else
    usleep(ms * 1000);
#endif
}

// ===== RAW MODE =====
#if !defined(_WIN32)
static struct termios origTermios;

void disableRawMode()
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &origTermios);
    std::cout << "\x1b[?25h\x1b[0m";
    std::cout.flush();
}

void enableRawMode()
{
    tcgetattr(STDIN_FILENO, &origTermios);
    atexit(disableRawMode);

    termios raw = origTermios;
    raw.c_lflag &= ~(ECHO | ICANON | ISIG);
    raw.c_iflag &= ~(ICRNL | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    setbuf(stdin, NULL);
}

bool stdinHasData(int timeoutUsec)
{
    fd_set rfds; FD_ZERO(&rfds); FD_SET(STDIN_FILENO, &rfds);
    timeval tv{}; tv.tv_usec = timeoutUsec;
    return select(STDIN_FILENO + 1, &rfds, nullptr, nullptr, &tv) == 1;
}

static int readCharWithTimeout(int timeoutUsec)
{
    if (!stdinHasData(timeoutUsec)) return -1;
    return getchar();
}

Key readKey()
{
    int c = readCharWithTimeout(30000);
    if (c == -1 || c == EOF) return KeyNone;

    if (c == 27)
    {
        int b1 = readCharWithTimeout(30000);
        if (b1 == -1) return KeyEsc;
        if (b1 != '[') return KeyEsc;
        int b2 = readCharWithTimeout(30000);
        if (b2 == -1) return KeyNone;
        if (b2 == 'A') return KeyUp;
        if (b2 == 'B') return KeyDown;
        if (b2 == 'C') return KeyRight;
        if (b2 == 'D') return KeyLeft;
        return KeyNone;
    }
    if (c == '\n' || c == '\r') return KeyEnter;
    if (c == 127 || c == 8)     return KeyBackspace;
    if (c >= 32 && c < 127)     return (Key)c;
    return KeyNone;
}
#endif

void waitForAnyKey()
{
    while (true)
    {
        Key k = readKey();
        if (k != KeyNone) break;
        delay(10);
    }
}

void hideCursor()
{
    std::cout << "\x1b[?25l"; std::cout.flush();
}

void showCursor()
{
    std::cout << "\x1b[?25h"; std::cout.flush();
}