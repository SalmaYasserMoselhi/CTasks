#include "menu.h"
#include "terminal_utils.h"
#include "employees.h"

#include <iostream>
using namespace std;

#if defined(_WIN32)
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
// #include <sys/select.h>
// #include <cstdio>
#endif
#include <limits>


#define COL 12
#define ROW_NEW 7
#define ROW_DISPLAY 9
#define ROW_EXIT 11

#if !defined(_WIN32)
static termios orig_termios;
static bool raw_mode_enabled = false;

static void enableRawMode()
{
    if (raw_mode_enabled)
        return; // already enabled
        
    if (tcgetattr(STDIN_FILENO, &orig_termios) == 0)
    {
        termios raw = orig_termios;  // copy original settings
        
        // disable canonical mode and echo
        raw.c_lflag &= ~(ICANON | ECHO);
        raw.c_cc[VMIN] = 1;
        raw.c_cc[VTIME] = 0;
        
        if (tcsetattr(STDIN_FILENO, TCSANOW, &raw) == 0)
        {
            raw_mode_enabled = true;
            setvbuf(stdin, nullptr, _IONBF, 0);
        }
    }
}

static void disableRawMode()
{
    if (raw_mode_enabled)
    {
        tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
        setvbuf(stdin, nullptr, _IOLBF, 0);
        raw_mode_enabled = false;
    }
}
#endif

void drawBtn (int y, const char *label, bool sel){
        Color c = sel ? BrightCyan : Default;
        drawText(COL, y, colorText(label, c));
}

// draw the menu vertically with a selected index
static void drawMenuUI(int selected)
{
    drawText(10, 4, colorText("=== Main Menu ===", Default));

    drawBtn(ROW_NEW, "[ New ]", selected == 0);
    drawBtn(ROW_DISPLAY, "[ Display ]", selected == 1);
    drawBtn(ROW_EXIT, "[ Exit ]", selected == 2);

    drawText(10, 14, colorText("Left=Up  Right=Down  Enter=select  Esc=quit  Backspace=back", BrightBlack));
    

    // Park cursor away from UI
    std::cout << "\x1b[1000;1H";  // hide cursor
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

static bool stdinHasData(int timeoutUsec)
{
    fd_set rfds;  // file descriptor set
    FD_ZERO(&rfds);  // clear the set
    FD_SET(STDIN_FILENO, &rfds);  // add stdin to the set
    timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = timeoutUsec;
    int r = select(STDIN_FILENO + 1, &rfds, nullptr, nullptr, &tv);  // wait for data
    return r == 1;  // if 1, data is available
}

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

static int showSelectionScreen(int selected)
{
    clearScreen();
    
    if (selected == 0) // New
    {
#if !defined(_WIN32)
        disableRawMode();
#endif
        std::cout << "\x1b[?25h";  
        std::cout.flush();
        
        addEmployee();
        
        std::cout << "\nPress Enter to continue...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
        
#if !defined(_WIN32)
        enableRawMode();
#endif

        std::cout << "\x1b[?25l";  
        std::cout.flush();
        
        return -2; // go back to menu
    }
    else if (selected == 1) // Display
    {
    #if !defined(_WIN32)
        disableRawMode();
    #endif
        std::cout << "\x1b[?25h";  
        std::cout.flush();

        displayEmployees();   // show employees

        std::cout << "\nPress Enter to continue...";
        std::cin.get(); // wait for Enter

    #if !defined(_WIN32)
        enableRawMode();
    #endif
        std::cout << "\x1b[?25l";  
        std::cout.flush();

        return -2; // go back to menu
    }

    else if (selected == 2) // Exit
    {
        drawText(10, 6, colorText("You chose: Exit", BrightGreen));
        drawText(10, 8, colorText("Press Backspace to go back, or Esc to quit.", BrightBlack));
        std::cout << "\x1b[1000;1H";
        std::cout.flush();

        while (true)
        {
            Key k = readKey();
            if (k == KeyBackspace)
                return -2;
            else if (k == KeyEsc)
                return -1;
        }
    }

    return -2;
}


// Returns: -1 on Esc to exit. Otherwise loops allowing backspace to return.
int showMenuInteractive()
{
#if !defined(_WIN32)
    enableRawMode();
#endif

    std::cout << "\x1b[?25l";  
    cout.flush();

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
#if !defined(_WIN32)
                disableRawMode();
#endif
                clearScreen();
                std::cout << "\x1b[?25h";
                std::cout.flush();
                return -1;
            }
            break;
        }

        case KeyEsc:
#if !defined(_WIN32)
            disableRawMode();
#endif
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