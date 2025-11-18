#include "menu.h"
#include "terminal_utils.h"
#include <iostream>

#if defined(_WIN32)
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#include <sys/select.h>
#include <cstdio>
#endif


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
    const int yNew = 7;
    const int yDisplay = 9;
    const int yExit = 11;

    auto btn = [&](int y, const char *label, bool sel)
    {
        Color c = sel ? BrightCyan : Default;
        drawText(x, y, colorText(label, c));
        clearToEol();
    };

    btn(yNew, "[ New ]", selected == 0);
    btn(yDisplay, "[ Display ]", selected == 1);
    btn(yExit, "[ Exit ]", selected == 2);

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

    if (ch == 27)
        return KeyEsc; // ESC
    if (ch == '\r')
        return KeyEnter; // Enter
    if (ch == 8)
        return KeyBackspace; // Backspace

    if (ch == 0 || ch == 224)
    {
        int c2 = _getch();
        switch (c2)
        {
        case 72:
            return KeyUp;
        case 80:
            return KeyDown;
        case 75:
            return KeyLeft;
        case 77:
            return KeyRight;
        default:
            return KeyNone;
        }
    }
    return KeyNone;
}

#else
// Raw mode with blocking getchar() reads (no read()).
class TerminalRawMode
{
private:
    termios orig;
    bool enabled;

public:
    TerminalRawMode() : enabled(false)
    {
        if (tcgetattr(STDIN_FILENO, &orig) == 0)
        {
            termios raw = orig;

            // Raw-ish: disable canonical mode and echo so getchar() returns immediately per key
            raw.c_lflag &= ~(ICANON | ECHO);
            raw.c_cc[VMIN] = 1;
            raw.c_cc[VTIME] = 0;

            if (tcsetattr(STDIN_FILENO, TCSANOW, &raw) == 0)
            {
                enabled = true;
                // Make stdin unbuffered to ensure getchar() returns per keystroke
                setvbuf(stdin, nullptr, _IONBF, 0);
            }
        }
    }

    ~TerminalRawMode()
    {
        if (enabled)
        {
            tcsetattr(STDIN_FILENO, TCSANOW, &orig);
            setvbuf(stdin, nullptr, _IOLBF, 0);
        }
    }
};

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
    if (c == EOF)
        return KeyNone;

    if (c == 27)
    { // ESC
        // Tiny lookahead to see if it's an escape sequence
        if (stdinHasData(30000))
        { // 30 ms
            int b1 = std::getchar();
            if (b1 == '[')
            {
                if (stdinHasData(30000))
                {
                    int b2 = std::getchar();
                    switch (b2)
                    {
                    case 'A':
                        return KeyUp;
                    case 'B':
                        return KeyDown;
                    case 'C':
                        return KeyRight;
                    case 'D':
                        return KeyLeft;
                    default:
                        return KeyNone;
                    }
                }
                return KeyNone;
            }
            return KeyNone;
        }
        else
        {
            return KeyEsc;
        }
    }

    if (c == '\n' || c == '\r')
        return KeyEnter;
    if (c == 127 || c == 8)
        return KeyBackspace;

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
    const char *msg = (selected == 0)   ? "You chose: New"
                      : (selected == 1) ? "You chose: Display"
                                        : "You chose: Exit";
    drawText(10, 6, colorText(msg, BrightGreen));
    drawText(10, 8, colorText("Press Backspace to go back, or Esc to quit.", BrightBlack));
    std::cout << "\x1b[1000;1H";
    std::cout.flush();

#if !defined(_WIN32)
    TerminalRawMode _rawModeGuard;
#endif

    while (true)
    {
        Key k = readKey();
        if (k == KeyBackspace)
        {
            return -2; // go back to menu
        }
        else if (k == KeyEsc)
        {
            return -1; // exit app
        }
        else if (k == KeyEnter)
        {
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

    while (true)
    {
        Key k = readKey();

        switch (k)
        {
        case KeyLeft:
        case KeyUp:
            if (selected > 0)
            {
                selected--;
                drawMenuUI(selected);
            }
            break;

        case KeyRight:
        case KeyDown:
            if (selected < 2)
            {
                selected++;
                drawMenuUI(selected);
            }
            break;

        case KeyEnter:
        {
            int r = showSelectionScreen(selected);
            if (r == -2)
            {
                clearScreen();
                drawMenuUI(selected);
                continue;
            }
            else if (r == -1)
            {
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
            break;

        case KeyNone:
        default:
            break;
        }
    }
}