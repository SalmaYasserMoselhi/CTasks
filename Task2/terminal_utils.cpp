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
// Fallback for older MinGW headers:
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
#else
  #include <unistd.h>
  #include <termios.h>
  #include <fcntl.h>
  #include <sys/select.h>
#endif

namespace
{
    // Simple global flags
    static bool g_inited = false;
    static bool g_ansiEnabled = false;

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
        return true;
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
        // Only enable ANSI if stdout is a TTY and TERM is not "dumb"
        bool isTTY = (isatty(STDOUT_FILENO) != 0);
        const char *term = std::getenv("TERM");
        bool termOk = (term && std::string(term) != "dumb");
        g_ansiEnabled = (isTTY && termOk);
#endif
        g_inited = true;
    }
    return g_ansiEnabled;
}

bool ansiEnabled()
{
    (void)initTerminal();
    return g_ansiEnabled;
}

bool drawText(int x, int y, const std::string &text)
{
    if (!validPos(x, y))
        return false;

#if defined(_WIN32)
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE || hOut == nullptr)
        return false;

    CONSOLE_SCREEN_BUFFER_INFO csbi;
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
    pos.X = static_cast<SHORT>(x - 1);
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

    DWORD written = 0;
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
    return out;
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
#if defined(_WIN32)
    if (ansiEnabled())
    {
        // Clear screen and move cursor home. Try also to clear scrollback.
        std::cout << "\x1b[H\x1b[2J\x1b[3J\x1b[H";
        std::cout.flush();
        return;
    }
    // Legacy WinAPI fallback
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE || hOut == nullptr) return;

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (!GetConsoleScreenBufferInfo(hOut, &csbi)) return;

    DWORD cellCount = csbi.dwSize.X * csbi.dwSize.Y;
    DWORD written;

    COORD home = {0, 0};
    FillConsoleOutputCharacter(hOut, ' ', cellCount, home, &written);
    FillConsoleOutputAttribute(hOut, csbi.wAttributes, cellCount, home, &written);
    SetConsoleCursorPosition(hOut, home);
#else
    if (ansiEnabled())
    {
        // Move cursor home, clear screen, clear scrollback, then move home again
        std::cout << "\x1b[H\x1b[2J\x1b[3J\x1b[H";
        std::cout.flush();
    } else {
        // Fallback when no ANSI: print many newlines to emulate clear
        for (int i = 0; i < 80; ++i) std::cout << '\n';
        std::cout.flush();
    }
#endif
}

void delay(int milliseconds)
{
#if defined(_WIN32)
    Sleep(milliseconds); // Windows: Sleep بالـ milliseconds
#else
    usleep(milliseconds * 1000);
#endif
}

/* =========================
   Interactive menu section
   ========================= */

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

// Helper: clear to end of the current line (after drawText positioned cursor)
static void clearToEol()
{
    if (ansiEnabled()) std::cout << "\x1b[K";
}

// Helper: draw the menu vertically with a selected index
// IMPORTANT: no full-screen clear and no cursor show/hide here.
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

    // Optionally park the cursor far away to avoid showing near UI in some terminals
    if (ansiEnabled()) std::cout << "\x1b[1000;1H";

    std::cout.flush();
}

#if defined(_WIN32)
#include <conio.h>

// Blocking key read
static Key readKey()
{
    int ch = _getch(); // blocks

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
// Raw mode with blocking reads (VMIN=1, VTIME=0)
class TerminalRawMode {
public:
    TerminalRawMode() : enabled(false) {
        if (tcgetattr(STDIN_FILENO, &orig) == 0) {
            termios raw = orig;
            raw.c_lflag &= ~(ICANON | ECHO);
            raw.c_cc[VMIN]  = 1; // block for at least 1 byte
            raw.c_cc[VTIME] = 0; // no timeout
            if (tcsetattr(STDIN_FILENO, TCSANOW, &raw) == 0) {
                enabled = true;
            }
        }
    }
    ~TerminalRawMode() {
        if (enabled) tcsetattr(STDIN_FILENO, TCSANOW, &orig);
    }
private:
    termios orig;
    bool enabled;
};

// POSIX: reliably detect bare ESC with a tiny timeout
static Key readKey()
{
    unsigned char c;
    ssize_t n = read(STDIN_FILENO, &c, 1);
    if (n <= 0) return KeyNone;

    if (c == 27) { // ESC or start of CSI sequence
        // Wait briefly for the next byte to see if it's an escape sequence.
        fd_set rfds;
        FD_ZERO(&rfds);
        FD_SET(STDIN_FILENO, &rfds);
        timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 30000; // 30 ms

        int r = select(STDIN_FILENO + 1, &rfds, nullptr, nullptr, &tv);
        if (r == 1) {
            // Another byte available: read and parse sequence
            unsigned char b1;
            if (read(STDIN_FILENO, &b1, 1) == 1) {
                if (b1 == '[') {
                    unsigned char b2;
                    if (read(STDIN_FILENO, &b2, 1) == 1) {
                        switch (b2) {
                            case 'A': return KeyUp;
                            case 'B': return KeyDown;
                            case 'C': return KeyRight;
                            case 'D': return KeyLeft;
                            default:  return KeyNone;
                        }
                    }
                }
            }
            return KeyNone;
        } else {
            // No more bytes: treat as bare ESC
            return KeyEsc;
        }
    }

    if (c == '\n' || c == '\r') return KeyEnter;
    if (c == 127 || c == 8)     return KeyBackspace; // DEL or BS
    return KeyNone;
}
#endif

// Wait screen after selection; Backspace returns to menu, Esc exits with -1
static int showSelectionScreen(int selected)
{
    clearScreen();
    const char* msg = (selected == 0) ? "You chose: New"
                     : (selected == 1) ? "You chose: Display"
                     : "You chose: Exit";
    drawText(10, 6, colorText(msg, BrightGreen));
    drawText(10, 8, colorText("Press Backspace to go back, or Esc to quit.", BrightBlack));
    if (ansiEnabled()) std::cout << "\x1b[1000;1H";
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
            // ignore or future confirm behavior
        }
    }
}

// Returns: -1 on Esc to exit. Otherwise loops allowing backspace to return.
int showMenuInteractive()
{
    initTerminal();

#if !defined(_WIN32)
    TerminalRawMode _rawModeGuard;
#endif

    // Hide cursor for the entire interactive session
    if (ansiEnabled()) std::cout << "\x1b[?25l";

    int selected = 0;

    clearScreen();
    drawMenuUI(selected);

    while (true) {
        Key k = readKey();

        switch (k) {
            // Left behaves like Up
            case KeyLeft:
            case KeyUp:
                if (selected > 0) {
                    selected--;
                    drawMenuUI(selected); // redraw without clearing the whole screen
                }
                break;

            // Right behaves like Down
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
                    // Backspace on selection screen: back to menu
                    clearScreen();
                    drawMenuUI(selected);
                    continue;
                } else if (r == -1) {
                    clearScreen();
                    if (ansiEnabled()) std::cout << "\x1b[?25h";
                    std::cout.flush();
                    return -1; // Esc: quit
                }
                break;
            }

            case KeyEsc:
                clearScreen();
                if (ansiEnabled()) std::cout << "\x1b[?25h";
                std::cout.flush();
                return -1;

            case KeyBackspace:
                // On the menu itself, Backspace does nothing.
                break;

            case KeyNone:
            default:
                break;
        }
    }
}

// Optional non-interactive render (if you keep this API in the header)
void showMenu()
{
    initTerminal();
    clearScreen();
    if (ansiEnabled()) std::cout << "\x1b[?25l";
    drawMenuUI(0);
    if (ansiEnabled()) std::cout << "\x1b[?25h";
    std::cout.flush();
}