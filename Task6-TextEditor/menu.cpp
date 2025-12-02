#include "menu.h"
#include "terminal_utils.h"
#include "editor.h"

#include <iostream>
using namespace std;

#define COL 12
#define ROW_NEW 7
#define ROW_DISPLAY 9
#define ROW_EXIT 11

const int RETURNTOMENU = -2;
const int EXITPROGRAM  = -1;

static void drawBtn(int y, const char *label, bool sel)
{
    Color c = sel ? BrightCyan : Default;
    drawText(COL, y, colorText(label, c));
}

static void drawMenuUI(int selected)
{
    drawText(10, 4, colorText("=== Main Menu ===", Default));
    drawBtn(ROW_NEW,     "[ New ]",     selected == 0);
    drawBtn(ROW_DISPLAY, "[ Display ]", selected == 1);
    drawBtn(ROW_EXIT,    "[ Exit ]",    selected == 2);
    drawText(10, 14, colorText("Up/Down or Left/Right = navigate | Enter = select | Esc = quit", BrightBlack));
    cout << "\x1b[1000;1H" << flush; 
}

static int showSelectionScreen(int selected)
{
    disableRawMode();
    showCursor();
    clearScreen();

    if (selected == 0)
        run_editor_new();
    else if (selected == 1)
        run_editor_display();
    else
        return EXITPROGRAM;

    clearScreen();
    return RETURNTOMENU;
}

static void cleanupAndExit()
{
    disableRawMode();
    showCursor();
    resetColor();
    clearScreen();
    cout.flush();
}

int showMenuInteractive()
{
    showCursor();
    resetColor();
    clearScreen();

    int selected = 0;
    drawMenuUI(selected);

    enableRawMode();

    while (true)
    {
        Key k = readKey();

        switch (k)
        {
        case KeyLeft:
        case KeyUp:
            if (selected > 0) { selected--; drawMenuUI(selected); }
            break;
        case KeyRight:
        case KeyDown:
            if (selected < 2) { selected++; drawMenuUI(selected); }
            break;
        case KeyEnter:
        {
            disableRawMode();
            int r = showSelectionScreen(selected);
            enableRawMode();

            if (r == RETURNTOMENU)
            {
                clearScreen();
                drawMenuUI(selected);
                continue;
            }
            else if (r == EXITPROGRAM)
            {
                cleanupAndExit();
                return -1;
            }
            break;
        }
        case KeyEsc:
        case KeyBackspace:
            cleanupAndExit();
            return -1;

        case KeyNone:
            delay(10);
            break;

        default:
            break;
        }
    }
}