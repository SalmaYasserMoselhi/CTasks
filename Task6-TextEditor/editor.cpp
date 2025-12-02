#include "editor.h"
#include "terminal_utils.h"

#include <iostream>
#include <cstring>
using namespace std;

static int promptForSize()
{
    disableRawMode();

    while (true)
    {
        clearScreen();
        drawText(5, 5, colorText("Enter text buffer size (1-10000): ", BrightCyan));
        cout.flush();

        char *input = new char[100];
        cin.getline(input, 100);

        if (strlen(input) == 0)
        {
            drawText(5, 7, colorText("Error: input cannot be empty!", BrightRed));
            cout.flush();
            delete[] input;
            delay(1500);
            continue;
        }

        char *endptr;
        long size = strtol(input, &endptr, 10); 

        if (*endptr != '\0' || endptr == input) // string
        {
            drawText(5, 7, colorText("Please enter a valid number!", BrightRed));
            cout.flush();
            delete[] input;
            delay(1500);
            continue;
        }

        if (size < 1 || size > 10000)
        {
            drawText(5, 7, colorText("Must be between 1 and 10000!", BrightRed));
            cout.flush();
            delete[] input;
            delay(1500);
            continue;
        }

        delete[] input;
        enableRawMode();
        return (int)size;
    }
}

static void drawEditorLine(const char *line, int cursor)
{
    clearScreen();
    drawText(5, 3, colorText("=== Text Editor ===", BrightBlue));
    drawText(5, 5, colorText("ESC = exit | ← → move | Backspace delete | type", BrightBlack));
    drawText(5, 7, line);

    int cx = 5 + cursor;  // cursor column
    cout << "\x1b[7;" << cx << "H" << flush;  // move cursor
}

static void saveToFile(const char *line)
{
    disableRawMode();
    clearScreen();

    drawText(5, 5, colorText("Enter filename (press Enter for default.txt): ", BrightCyan));
    cout.flush();

    char *filename = new char[256];
    cin.getline(filename, 256);

    if (strlen(filename) == 0)
        strcpy(filename, "default.txt");

    clearScreen();
    drawText(5, 5, colorText("Save Mode:", BrightBlue));
    drawText(5, 7, colorText("[1] Overwrite", BrightCyan));
    drawText(5, 9, colorText("[2] Append", BrightCyan));
    drawText(5, 11, colorText("Press 1 or 2: ", BrightBlack));
    cout.flush();

    enableRawMode();

    int mode = 1;
    while (true)
    {
        Key k = readKey();
        if (k == KeyNone)
        {
            delay(10);
            continue;
        }

        if (k == '1')
        {
            mode = 1;
            break;
        }
        else if (k == '2')
        {
            mode = 2;
            break;
        }
    }

    FILE *fp = fopen(filename, (mode == 1 ? "w" : "a"));

    if (!fp)
    {
        clearScreen();
        drawText(5, 8, colorText("Failed to open file!", BrightRed));
        drawText(5, 10, colorText("Press any key...", BrightBlack));
        cout.flush();
        waitForAnyKey();
        delete[] filename;
        return;
    }

    fputs(line, fp);  // Write in file
    fclose(fp);

    clearScreen();
    drawText(5, 8, colorText("Saved successfully to:", BrightGreen));
    drawText(5, 10, colorText(filename, BrightCyan));
    drawText(5, 13, colorText("Press any key...", BrightBlack));
    cout.flush();

    waitForAnyKey();
    delete[] filename;
}

static int showSaveDiscardMenu()
{
    int selected = 0;

    while (true)
    {
        clearScreen();
        drawText(10, 5, colorText("=== Save or Discard? ===", BrightBlue));

        Color c0 = (selected == 0) ? BrightCyan : Default;
        Color c1 = (selected == 1) ? BrightCyan : Default;

        drawText(12, 8, colorText("[ Save ]", c0));
        drawText(12, 10, colorText("[ Discard ]", c1));
        drawText(10, 13, colorText("↑↓ navigate | Enter select", BrightBlack));
        cout.flush();

        Key k = readKey();

        if (k == KeyUp && selected > 0)
            selected--;
        else if (k == KeyDown && selected < 1)
            selected++;
        else if (k == KeyEnter)
            return selected;
        else if (k == KeyEsc)
            return 1;
        else if (k == KeyNone)
            delay(10);
    }
}

int run_editor_new()
{
    enableRawMode();

    int maxSize = promptForSize();

    showCursor();

    char *line = new char[maxSize + 1];
    line[0] = '\0';
    int cursor = 0;

    drawEditorLine(line, cursor);

    while (true)
    {
        Key k = readKey();
        if (k == KeyNone)
        {
            delay(10);
            continue;
        }

        int len = strlen(line);

        if (k == KeyEsc)
            break;
        else if (k == KeyLeft && cursor > 0)
            cursor--;
        else if (k == KeyRight && cursor < len)
            cursor++;
        else if (k == KeyBackspace && cursor > 0)
        {
            for (int i = cursor - 1; i < len; i++)
                line[i] = line[i + 1];
            cursor--;
        }
        else if (k >= 32 && k < 127 && len < maxSize) // characters
        {
            for (int i = len; i >= cursor; i--)
                line[i + 1] = line[i];
            line[cursor] = (char)k;
            cursor++;
        }

        drawEditorLine(line, cursor);
    }

    int choice = showSaveDiscardMenu();

    if (choice == 0)
        saveToFile(line);
    else
    {
        clearScreen();
        drawText(10, 8, colorText("User chose: DISCARD ✗", BrightRed));
        drawText(10, 10, colorText("Press any key...", BrightCyan));
        cout.flush();
        waitForAnyKey();
    }

    delete[] line;
    disableRawMode();
    clearScreen();
    return 0;
}

int run_editor_display()
{
    disableRawMode();
    clearScreen();

    drawText(5, 5, colorText("Enter filename (press Enter for default.txt): ", BrightCyan));
    cout.flush();

    char *filename = new char[256];
    cin.getline(filename, 256);

    if (strlen(filename) == 0)
        strcpy(filename, "default.txt");

    clearScreen();

    FILE *fp = fopen(filename, "r");

    if (!fp)
    {
        drawText(5, 7, colorText("File not found!", BrightRed));
        drawText(5, 9, colorText("Press Enter to return...", BrightBlack));
        cout.flush();
        cin.get();  // wait for Enter
        delete[] filename;
        clearScreen();
        return 0;
    }

    char *buffer = new char[5000];  // for read
    size_t bytesRead = fread(buffer, 1, 4999, fp);
    buffer[bytesRead] = '\0';
    fclose(fp);

    // build header string
    char *header = new char[300];
    strcpy(header, "=== ");
    strcat(header, filename);
    strcat(header, " ===");

    drawText(5, 3, colorText(header, BrightBlue));
    drawText(5, 5, colorText("Content:", BrightCyan));
    drawText(5, 7, buffer);
    drawText(5, 22, colorText("Press Enter to return...", BrightBlack));
    cout.flush();

    delete[] buffer;
    delete[] filename;
    delete[] header;

    cin.get();
    clearScreen();
    return 0;
}