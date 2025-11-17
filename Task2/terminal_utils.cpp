// terminal_utils.cpp
#include "terminal_utils.h"

#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <cstdlib>

#if defined(_WIN32)
#ifndef NOMINMAX
#define NOMINMAX  // Prevent Windows headers from defining min/max macros
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
        case Black:         return "\x1b[30m";   // Escape code for black
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
        // Get a handle to the console's standard output
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hOut == INVALID_HANDLE_VALUE || hOut == nullptr)
            return false;

        // Read the current console mode (settings)
        DWORD dwMode = 0;
        if (!GetConsoleMode(hOut, &dwMode))
            return false;

        // Add the flag that enables ANSI escape sequence (VT processing)
        DWORD desired = dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING;

        // Apply the updated mode back to the console
        if (!SetConsoleMode(hOut, desired))
        {
            return false;
        }

        // Success: console now supports ANSI escape codes
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
        g_ansiEnabled = true; // Assume ANSI on POSIX
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
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);   // Get handle to the standard output (console)

    if (hOut == INVALID_HANDLE_VALUE || hOut == nullptr)
        return false; // If handle is invalid, fail

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (!GetConsoleScreenBufferInfo(hOut, &csbi))
    {
        // If unable to get console info, fallback to ANSI escape codes if enabled
        if (ansiEnabled())
        {
            std::cout << "\x1b[" << y << ";" << x << "H" << text;
            std::cout.flush();
            return true;
        }
        return false;
    }

    // Convert coordinates to Windows COORD (0-based indexing)
    COORD pos;
    pos.X = static_cast<SHORT>(x - 1);
    pos.Y = static_cast<SHORT>(y - 1);

    // Try to move the cursor to the desired position
    if (!SetConsoleCursorPosition(hOut, pos))
    {
        // If cursor positioning fails, fallback to ANSI escape codes
        if (ansiEnabled())
        {
            std::cout << "\x1b[" << y << ";" << x << "H" << text;
            std::cout.flush();
            return true;
        }
        return false;
    }

    // Write text directly to the console at the cursor position
    DWORD written = 0;
    if (!WriteConsoleA(hOut, text.c_str(),
                       static_cast<DWORD>(text.size()),
                       &written, nullptr))
    {
        // If writing fails, just print normally
        std::cout << text;
        std::cout.flush();
        return false;
    }
    return true;

#else  // If compiling on Linux/Unix (POSIX systems)
    // Use ANSI escape codes to move cursor and print text
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
    }
#endif
}

void delay(int milliseconds)
{
#if defined(_WIN32)
    Sleep(milliseconds); // Windows: Sleep with milliseconds
#else
    usleep(milliseconds * 1000);
#endif
}


// clear to end of the current line (after drawText positioned cursor)
static void clearToEol()
{
    if (ansiEnabled()) std::cout << "\x1b[K";
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

    auto btn = [&](int y, const char* label, bool sel) {  // auto can be used for lambda functions
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
#include <conio.h>   // console input/output for Windows

// Blocking key read
static Key readKey()
{
    int ch = _getch(); // reads only one character from console

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
private:
    termios orig;   // Original terminal settings
    bool enabled;   // Flag to check if raw mode is active
public:
    TerminalRawMode() : enabled(false) {
        // Save the original terminal settings
        if (tcgetattr(STDIN_FILENO, &orig) == 0) {
            termios raw = orig; // Copy the original settings

            // Enable Raw mode: terminal sends each keystroke immediately, no buffering, no echo.
            raw.c_lflag &= ~(ICANON | ECHO);  

            // VMIN = 1 → block until at least 1 byte is read
            raw.c_cc[VMIN]  = 1;
            // VTIME = 0 → no timeout, wait indefinitely
            raw.c_cc[VTIME] = 0;

            // Apply the new settings immediately
            if (tcsetattr(STDIN_FILENO, TCSANOW, &raw) == 0) {
                enabled = true; // Raw mode successfully enabled
            }
        }
    }

    ~TerminalRawMode() {
        // When the object is destroyed, restore the original settings
        if (enabled) tcsetattr(STDIN_FILENO, TCSANOW, &orig);
    }

};

// POSIX: reliably detect bare ESC with a tiny timeout
static Key readKey()
{
    unsigned char c;
    // Read one byte from keyboard
    ssize_t n = read(STDIN_FILENO, &c, 1);
    if (n <= 0) return KeyNone; // No input

    // If the key is ESC (ASCII 27)
    if (c == 27) {
        // Wait briefly (30 ms) to see if another byte follows (escape sequence)
        fd_set rfds;
        FD_ZERO(&rfds);
        FD_SET(STDIN_FILENO, &rfds);
        timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 30000; // 30 ms

        int r = select(STDIN_FILENO + 1, &rfds, nullptr, nullptr, &tv);
        if (r == 1) {
            // Another byte is available: read it
            unsigned char b1;
            if (read(STDIN_FILENO, &b1, 1) == 1) {
                // If it's '[', then it's the start of an arrow key sequence
                if (b1 == '[') {
                    unsigned char b2;
                    if (read(STDIN_FILENO, &b2, 1) == 1) {
                        // Map the sequence to arrow keys
                        switch (b2) {
                            case 'A': return KeyUp;    // Up arrow
                            case 'B': return KeyDown;  // Down arrow
                            case 'C': return KeyRight; // Right arrow
                            case 'D': return KeyLeft;  // Left arrow
                            default:  return KeyNone;  // Unknown sequence
                        }
                    }
                }
            }
            return KeyNone; // Sequence not recognized
        } else {
            // No extra byte → treat as plain ESC key
            return KeyEsc;
        }
    }

    // Enter key (LF=10 or CR=13)
    if (c == '\n' || c == '\r') return KeyEnter;

    // Backspace key (DEL=127 or BS=8)
    if (c == 127 || c == 8)     return KeyBackspace;

    // Any other key not handled
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

#if !defined(_WIN32)   // windows uses conio.h to read keys
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
    // initTerminal();

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
