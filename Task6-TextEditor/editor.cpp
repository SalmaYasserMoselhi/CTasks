#include "editor.h"
#include "terminal_utils.h"

#include <iostream>
#include <cstring>
#include <cstdio>

// Helper function to get user input in raw mode
static char* getInputInRawMode(int x, int y, const char* prompt, bool numbersOnly = false, int maxLen = 50)
{
    drawText(x, y, colorText(prompt, BrightCyan));
    std::cout << "\x1b[" << y << ";" << (x + strlen(prompt)) << "H" << std::flush;
    
    char* input = new char[maxLen + 1];
    int len = 0;
    input[0] = '\0';
    
    showCursor();
    
    while (true)
    {
        Key k = readKey();
        if (k == KeyNone) { delay(10); continue; }
        
        if (k == KeyEnter)
            break;
        else if (k == KeyBackspace && len > 0)
        {
            len--;
            input[len] = '\0';
            int cx = x + strlen(prompt);
            std::cout << "\x1b[" << y << ";" << cx << "H" 
                      << std::string(maxLen, ' ') 
                      << "\x1b[" << y << ";" << cx << "H" 
                      << input << std::flush;
        }
        else if (k == KeyEsc)
        {
            input[0] = '\0';
            break;
        }
        else if (numbersOnly && k >= '0' && k <= '9' && len < maxLen)
        {
            input[len++] = (char)k;
            input[len] = '\0';
            std::cout << (char)k << std::flush;
        }
        else if (!numbersOnly && k >= 32 && k < 127 && len < maxLen)
        {
            input[len++] = (char)k;
            input[len] = '\0';
            std::cout << (char)k << std::flush;
        }
    }
    
    return input;
}

static int promptForSize()
{
    while (true)
    {
        clearScreen();
        char* input = getInputInRawMode(5, 5, "Enter text buffer size (1-10000): ", true, 5);
        
        if (strlen(input) == 0)
        {
            delete[] input;
            drawText(5, 7, colorText("✗ Error: input cannot be empty!", BrightRed));
            std::cout.flush();
            delay(1500);
            continue;
        }

        int size = atoi(input);
        delete[] input;
        
        if (size < 1 || size > 10000)
        {
            drawText(5, 7, colorText("✗ Must be between 1 and 10000!", BrightRed));
            std::cout.flush();
            delay(1500);
            continue;
        }

        return size;
    }
}

static void drawEditorLine(const char* line, int cursor, int maxSize)
{
    clearScreen();
    drawText(5, 3, colorText("=== Text Editor ===", BrightBlue));
    drawText(5, 5, colorText("ESC = exit | ← → move | Backspace delete | type", BrightBlack));

    char counter[50];
    sprintf(counter, "( %d / %d )", (int)strlen(line), maxSize);
    drawText(5, 6, colorText(counter, BrightCyan));
    drawText(5, 8, line);

    int cx = 5 + cursor;
    std::cout << "\x1b[8;" << cx << "H" << std::flush;
}

static void saveToFile(const char* line)
{
    clearScreen();
    
    // Get filename using heap-allocated buffer
    char* filename = getInputInRawMode(5, 5, "Enter filename: ", false, 50);
    if (strlen(filename) == 0)
    {
        delete[] filename;
        filename = new char[13];
        strcpy(filename, "default.txt");
    }

    // Get save mode
    clearScreen();
    drawText(5, 5, colorText("Save Mode:", BrightBlue));
    drawText(5, 7, colorText("[1] Overwrite", BrightCyan));
    drawText(5, 9, colorText("[2] Append", BrightCyan));
    drawText(5, 11, colorText("Press 1 or 2: ", BrightBlack));
    std::cout.flush();

    int mode = 1;
    while (true)
    {
        Key k = readKey();
        if (k == KeyNone) { delay(10); continue; }
        
        if (k == '1') { mode = 1; break; }
        else if (k == '2') { mode = 2; break; }
    }

    // Open file and write
    FILE* fp = fopen(filename, (mode == 1 ? "w" : "a"));

    if (!fp)
    {
        clearScreen();
        drawText(5, 8, colorText("✗ Failed to open file!", BrightRed));
        drawText(5, 10, colorText("Press any key...", BrightBlack));
        std::cout.flush();
        delete[] filename;
        waitForAnyKey();
        return;
    }

    fputs(line, fp);
    fclose(fp);

    clearScreen();
    drawText(5, 8, colorText("✓ Saved successfully to:", BrightGreen));
    drawText(5, 10, colorText(filename, BrightCyan));
    drawText(5, 13, colorText("Press any key...", BrightBlack));
    std::cout.flush();

    delete[] filename;
    waitForAnyKey();
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
        std::cout.flush();

        Key k = readKey();

        if (k == KeyUp && selected > 0) selected--;
        else if (k == KeyDown && selected < 1) selected++;
        else if (k == KeyEnter) return selected;
        else if (k == KeyEsc) return 1;
        else if (k == KeyNone) delay(10);
    }
}

int run_editor_new()
{
    enableRawMode();
    
    int maxSize = promptForSize();
    if (maxSize <= 0)
    {
        disableRawMode();
        return 0;
    }

    showCursor();
    
    char* line = new char[maxSize + 1];
    line[0] = '\0';
    int cursor = 0;

    drawEditorLine(line, cursor, maxSize);

    while (true)
    {
        Key k = readKey();
        if (k == KeyNone) { delay(10); continue; }

        int len = strlen(line);

        if (k == KeyEsc)
            break;
        else if (k == KeyLeft && cursor > 0)
            cursor--;
        else if (k == KeyRight && cursor < len)
            cursor++;
        else if (k == KeyBackspace && cursor > 0)
        {
            // Shift characters left
            for (int i = cursor - 1; i < len; i++)
                line[i] = line[i + 1];
            cursor--;
        }
        else if (k >= 32 && k < 127 && len < maxSize)
        {
            // Shift characters right to make space
            for (int i = len; i >= cursor; i--)
                line[i + 1] = line[i];
            line[cursor] = (char)k;
            cursor++;
        }

        drawEditorLine(line, cursor, maxSize);
    }

    int choice = showSaveDiscardMenu();

    if (choice == 0)
        saveToFile(line);
    else
    {
        clearScreen();
        drawText(10, 8, colorText("User chose: DISCARD ✗", BrightRed));
        drawText(10, 10, colorText("Press any key...", BrightCyan));
        std::cout.flush();
        waitForAnyKey();
    }

    delete[] line;
    disableRawMode();
    clearScreen();
    return 0;
}

int run_editor_display()
{
    enableRawMode();
    clearScreen();
    showCursor();

    char* filename = getInputInRawMode(5, 5, "Enter filename: ", false, 50);
    
    if (strlen(filename) == 0)
    {
        delete[] filename;
        filename = new char[13];
        strcpy(filename, "default.txt");
    }

    clearScreen();
    
    FILE* fp = fopen(filename, "r");
    
    if (!fp)
    {
        drawText(5, 7, colorText("✗ File not found!", BrightRed));
        drawText(5, 9, colorText("Press any key...", BrightBlack));
        std::cout.flush();
        delete[] filename;
        waitForAnyKey();
        disableRawMode();
        clearScreen();
        return 0;
    }

    // Allocate buffer for file content on heap
    char* buffer = new char[5000];
    size_t bytesRead = fread(buffer, 1, 4999, fp);
    buffer[bytesRead] = '\0';
    fclose(fp);

    drawText(5, 3, colorText(std::string("=== ") + filename + " ===", BrightBlue));
    drawText(5, 5, colorText("Content:", BrightCyan));
    
    std::cout << "\x1b[7;5H" << buffer << std::flush;
    
    drawText(5, 22, colorText("Press any key to return...", BrightBlack));
    std::cout.flush();

    delete[] buffer;
    delete[] filename;
    
    waitForAnyKey();
    disableRawMode();
    clearScreen();
    return 0;
}