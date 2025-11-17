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
#include <conio.h>
#else
  #include <unistd.h>
  #include <termios.h>
  #include <fcntl.h>
  #include <sys/select.h>
  #include <cstdio>
#endif

namespace
{
    const char *colorToAnsi(Color c)
    {
        switch (c)
        {
        case Black:         return "\x1b[30m";
        case Red:           return "\x1b[31m";
        case Green:         return "\x1b[32m";
        case Yellow:        return "\x1b[33m";
        case Blue:          return "\x1b[34m";
        case Magenta:       return "\x1b[35m";
        case Cyan:          return "\x1b[36m";
        case White:         return "\x1b[37m";
        case BrightBlack:   return "\x1b[90m";
        case BrightRed:     return "\x1b[91m";
        case BrightGreen:   return "\x1b[92m";
        case BrightYellow:  return "\x1b[93m";
        case BrightBlue:    return "\x1b[94m";
        case BrightMagenta: return "\x1b[95m";
        case BrightCyan:    return "\x1b[96m";
        case BrightWhite:   return "\x1b[97m";
        case Default:
        default:            return "\x1b[39m";
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

// clear to end of the current line (after drawText positioned cursor)
static void clearToEol()
{
    std::cout << "\x1b[K";
}

// draw the menu vertically with a selected index
static void drawMenuUI(int selected)
{
    // Title
    drawText(10, 4, colorText("=== Main Menu ===", Default));
    clearToEol();

    // Button positions (vertically aligned)
    const int x = 12;
    const int yNew     = 7;
    const int yDisplay = 9;
    const int yExit    = 11;

    auto btn = [&](int y, const char* label, bool sel) {
        Color c = sel ? BrightCyan : Default;
        drawText(x, y, colorText(label, c));
        clearToEol();
    };

    btn(yNew,     "[ New ]",     selected == 0);
    btn(yDisplay, "[ Display ]", selected == 1);
    btn(yExit,    "[ Exit ]",    selected == 2);

    drawText(10, 14, colorText("Left=Up  Right=Down  Enter=select  Esc=quit  Backspace=back", BrightBlack));
    clearToEol();

    // Park cursor away from UI
    std::cout << "\x1b[1000;1H";
    std::cout.flush();
}

#if defined(_WIN32)
// Blocking key read (Windows, conio)
static Key readKey()
{
    int ch = _getch();

    if (ch == 27) return KeyEsc;           // ESC
    if (ch == '\r') return KeyEnter;       // Enter
    if (ch == 8)   return KeyBackspace;    // Backspace

    if (ch == 0 || ch == 224) {
        int c2 = _getch();
        switch (c2) {
            case 72: return KeyUp;
            case 80: return KeyDown;
            case 75: return KeyLeft;
            case 77: return KeyRight;
            default: return KeyNone;
        }
    }
    return KeyNone;
}

#else
// Raw mode with blocking getchar() reads (no read()).
class TerminalRawMode {
private:
    termios orig;
    bool enabled;
public:
    TerminalRawMode() : enabled(false) {
        if (tcgetattr(STDIN_FILENO, &orig) == 0) {
            termios raw = orig;

            // Raw-ish: disable canonical mode and echo so getchar() returns immediately per key
            raw.c_lflag &= ~(ICANON | ECHO);
            // VMIN/VTIME influence read(); getchar() uses stdio buffering.
            // To make getchar block for one char without echo, disabling ICANON/ECHO is sufficient.
            // We still set them to standard blocking semantics via termios in case underlying read is used.
            raw.c_cc[VMIN]  = 1;
            raw.c_cc[VTIME] = 0;

            if (tcsetattr(STDIN_FILENO, TCSANOW, &raw) == 0) {
                enabled = true;
                // Make stdin unbuffered to ensure getchar() returns per keystroke
                setvbuf(stdin, nullptr, _IONBF, 0);
            }
        }
    }

    ~TerminalRawMode() {
        if (enabled) {
            tcsetattr(STDIN_FILENO, TCSANOW, &orig);
            // Restore default buffering (optional)
            setvbuf(stdin, nullptr, _IOLBF, 0);
        }
    }
};

// Helper: try to non-destructively peek next char with a short timeout to detect ESC sequences
static bool stdinHasData(int timeoutUsec)
{
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(STDIN_FILENO, &rfds);
    timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = timeoutUsec;
    int r = select(STDIN_FILENO + 1, &rfds, nullptr, nullptr, &tv);
    return r == 1;
}

// Blocking key read using getchar() and minimal lookahead for escape sequences
static Key readKey()
{
    int c = std::getchar();
    if (c == EOF) return KeyNone;

    if (c == 27) { // ESC
        // Tiny lookahead to see if it's an escape sequence
        if (stdinHasData(30000)) { // 30 ms
            int b1 = std::getchar();
            if (b1 == '[') {
                if (stdinHasData(30000)) {
                    int b2 = std::getchar();
                    switch (b2) {
                        case 'A': return KeyUp;
                        case 'B': return KeyDown;
                        case 'C': return KeyRight;
                        case 'D': return KeyLeft;
                        default:  return KeyNone;
                    }
                }
                return KeyNone;
            }
            // Not a CSI sequence; consume and ignore.
            return KeyNone;
        } else {
            return KeyEsc; // bare ESC
        }
    }

    if (c == '\n' || c == '\r') return KeyEnter;
    if (c == 127 || c == 8)     return KeyBackspace;

    return KeyNone;
}
#endif

// Optional non-interactive menu render (kept as a simple call)
void showMenu()
{
    clearScreen();
    drawMenuUI(0);
    std::cout.flush();
}

// Wait screen after selection; Backspace returns to menu, Esc exits with -1
static int showSelectionScreen(int selected)
{
    clearScreen();
    const char* msg = (selected == 0) ? "You chose: New"
                     : (selected == 1) ? "You chose: Display"
                     : "You chose: Exit";
    drawText(10, 6, colorText(msg, BrightGreen));
    drawText(10, 8, colorText("Press Backspace to go back, or Esc to quit.", BrightBlack));
    std::cout << "\x1b[1000;1H";
    std::cout.flush();

#if !defined(_WIN32)
    TerminalRawMode _rawModeGuard;
#endif

    while (true) {
        Key k = readKey();
        if (k == KeyBackspace) {
            return -2; // go back to menu
        } else if (k == KeyEsc) {
            return -1; // exit app
        } else if (k == KeyEnter) {
            // Ignore
        }
    }
}

// Returns: -1 on Esc to exit. Otherwise loops allowing backspace to return.
int showMenuInteractive()
{
#if !defined(_WIN32)
    TerminalRawMode _rawModeGuard;
#endif

    // Hide cursor for the entire interactive session
    std::cout << "\x1b[?25l";

    int selected = 0;

    clearScreen();
    drawMenuUI(selected);

    while (true) {
        Key k = readKey();

        switch (k) {
            case KeyLeft:
            case KeyUp:
                if (selected > 0) {
                    selected--;
                    drawMenuUI(selected);
                }
                break;

            case KeyRight:
            case KeyDown:
                if (selected < 2) {
                    selected++;
                    drawMenuUI(selected);
                }
                break;

            case KeyEnter: {
                int r = showSelectionScreen(selected);
                if (r == -2) {
                    clearScreen();
                    drawMenuUI(selected);
                    continue;
                } else if (r == -1) {
                    clearScreen();
                    std::cout << "\x1b[?25h";
                    std::cout.flush();
                    return -1;
                }
                break;
            }

            case KeyEsc:
                clearScreen();
                std::cout << "\x1b[?25h";
                std::cout.flush();
                return -1;

            case KeyBackspace:
                // no-op on menu
                break;

            case KeyNone:
            default:
                break;
        }
    }
}