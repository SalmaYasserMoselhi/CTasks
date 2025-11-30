#include "editor.h"
#include "terminal_utils.h"

#include <iostream>
#include <string>
#include <limits>
#include <termios.h>
#include <unistd.h>
#include <cstdio>


static int promptForSize()
{
    disableRawMode();
    delay(100);
    
    while (true)
    {
        clearScreen();
        drawText(5, 5, colorText("Enter text buffer size (1-10000): ", BrightCyan));
        std::cout << "\x1b[5;38H" << std::flush;
        
        std::string input;
        
        enableRawMode();
        showCursor();
        
        while (true)
        {
            Key k = readKey();
            if (k == KeyNone) { delay(10); continue; }
            
            if (k == KeyEnter)
                break;
            else if (k == KeyBackspace && !input.empty())
            {
                input.pop_back();
                std::cout << "\x1b[5;38H" << std::string(20, ' ') 
                          << "\x1b[5;38H" << input << std::flush;
            }
            else if (k >= '0' && k <= '9')
            {
                if (input.size() < 5)  // max 5 digits (10000)
                {
                    input += (char)k;
                    std::cout << (char)k << std::flush;
                }
            }
            else if (k == KeyEsc)
            {
                disableRawMode();
                return -1;
            }
        }

        if (input.empty())
        {
            drawText(5, 7, colorText("✗ Error: input cannot be empty!", BrightRed));
            std::cout.flush();
            delay(1500);
            // ✅ لازم تطفي raw mode قبل اللوب يعيد
            disableRawMode();
            delay(50);
            continue;
        }

        int size = std::stoi(input);
        if (size < 1 || size > 10000)
        {
            drawText(5, 7, colorText("✗ Must be between 1 and 10000!", BrightRed));
            std::cout.flush();
            delay(1500);
            // ✅ لازم تطفي raw mode قبل اللوب يعيد
            disableRawMode();
            delay(50);
            continue;
        }

        return size;
    }
}

static void drawEditorLine(const std::string &line, int cursor, int maxSize)
{
    clearScreen();
    drawText(5, 3, colorText("=== Text Editor ===", BrightBlue));
    drawText(5, 5, colorText("ESC = exit | ← → move | Backspace delete | type", BrightBlack));

    std::string counter = "( " + std::to_string(line.size()) +
                          " / " + std::to_string(maxSize) + " )";

    drawText(5, 6, colorText(counter, BrightCyan));

    drawText(5, 8, line);

    int cx = 5 + cursor;
    std::cout << "\x1b[8;" << cx << "H" << std::flush;
}


static void saveToFile(const std::string &line)
{
    // Stay in raw mode but show cursor
    showCursor();
    clearScreen();

    // Get filename in raw mode
    drawText(5, 5, colorText("Enter filename: ", BrightCyan));
    std::cout << "\x1b[5;24H" << std::flush;
    
    std::string filename;
    while (true)
    {
        Key k = readKey();
        if (k == KeyNone) { delay(10); continue; }
        
        if (k == KeyEnter)
            break;
        else if (k == KeyBackspace && !filename.empty())
        {
            filename.pop_back();
            std::cout << "\x1b[5;24H" << std::string(filename.size() + 10, ' ') 
                      << "\x1b[5;24H" << filename << std::flush;
        }
        else if (k >= 32 && k < 127)
        {
            filename += (char)k;
            std::cout << (char)k << std::flush;
        }
    }

    if (filename.empty())
        filename = "default.txt";

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

    // heap buffer
    int size = line.size();
    char *buffer = new char[size + 1];
    for (int i = 0; i < size; i++)
        buffer[i] = line[i];
    buffer[size] = '\0';

    FILE *fp = nullptr;
    fp = fopen(filename.c_str(), (mode == 1 ? "w" : "a"));

    if (!fp)
    {
        drawText(5, 14, colorText("✗ Failed to open file!", BrightRed));
        std::cout.flush();
        delete[] buffer;
        delay(1500);
        return;
    }

    fputs(buffer, fp);
    fclose(fp);
    delete[] buffer;

    clearScreen();
    drawText(5, 8, colorText("✓ Saved successfully to:", BrightGreen));
    drawText(5, 10, colorText(filename, BrightCyan));
    drawText(5, 13, colorText("Press any key...", BrightBlack));
    std::cout.flush();

    // Now waitForAnyKey works because we're STILL in raw mode!
    waitForAnyKey();
}


static int showSaveDiscardMenu()
{
    int selected = 0;
    enableRawMode();

    while (true)
    {
        clearScreen();
        drawText(10, 5, colorText("=== Save or Discard? ===", BrightBlue));

        Color c0 = (selected == 0) ? BrightCyan : Default;
        Color c1 = (selected == 1) ? BrightCyan : Default;

        drawText(12, 8, colorText("[ Save ]", c0));
        drawText(12, 10, colorText("[ Discard ]", c1));
        drawText(10, 13, colorText("↑↓ navigate | Enter select", BrightBlack));

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
    int maxSize = promptForSize();
    if (maxSize <= 0)
        return 0;

    enableRawMode();
    showCursor();

    std::string line;
    int cursor = 0;

    drawEditorLine(line, cursor, maxSize);

    while (true)
    {
        Key k = readKey();
        if (k == KeyNone) { delay(10); continue; }

        if (k == KeyEsc)
            break;
        else if (k == KeyLeft && cursor > 0)
            cursor--;
        else if (k == KeyRight && cursor < (int)line.size())
            cursor++;
        else if (k == KeyBackspace && cursor > 0)
        {
            line.erase(cursor - 1, 1);
            cursor--;
        }
        else if (k >= 32 && k < 127)
        {
            if ((int)line.size() < maxSize)
            {
                line.insert(line.begin() + cursor, (char)k);
                cursor++;
            }
        }

        drawEditorLine(line, cursor, maxSize);
    }

    int choice = showSaveDiscardMenu();

    if (choice == 0)
        saveToFile(line);  // Still in raw mode!
    else
    {
        clearScreen();
        drawText(10, 8, colorText("User chose: DISCARD ✗", BrightRed));
        drawText(10, 10, colorText("Press any key...", BrightCyan));
        std::cout.flush();
        waitForAnyKey();  // Still in raw mode!
    }

    disableRawMode();
    showCursor();
    clearScreen();
    return 0;
}


int run_editor_display()
{
    disableRawMode();
    delay(100);
    
    clearScreen();
    showCursor();

    drawText(5, 5, colorText("Enter filename: ", BrightCyan));
    std::cout << "\x1b[5;24H" << std::flush;
    
    std::string filename;
    enableRawMode();
    
    while (true)
    {
        Key k = readKey();
        if (k == KeyNone) { delay(10); continue; }
        
        if (k == KeyEnter)
            break;
        else if (k == KeyBackspace && !filename.empty())
        {
            filename.pop_back();
            std::cout << "\x1b[5;24H" << std::string(50, ' ') 
                      << "\x1b[5;24H" << filename << std::flush;
        }
        else if (k == KeyEsc)
        {
            disableRawMode();
            clearScreen();
            return 0;
        }
        else if (k >= 32 && k < 127)
        {
            filename += (char)k;
            std::cout << (char)k << std::flush;
        }
    }

    if (filename.empty())
        filename = "default.txt";

    FILE *fp = fopen(filename.c_str(), "r");
    
    clearScreen();
    
    if (!fp)
    {
        drawText(5, 7, colorText("✗ File not found!", BrightRed));
        drawText(5, 9, colorText("Press any key...", BrightBlack));
        std::cout.flush();
        waitForAnyKey();
        disableRawMode();
        clearScreen();
        return 0;
    }

    char buffer[5000];
    size_t bytesRead = fread(buffer, 1, sizeof(buffer) - 1, fp);
    buffer[bytesRead] = '\0';
    fclose(fp);

    drawText(5, 3, colorText("=== " + filename + " ===", BrightBlue));
    drawText(5, 5, colorText("Content:", BrightCyan));
    
    std::cout << "\x1b[7;5H" << buffer << std::flush;
    
    drawText(5, 22, colorText("Press any key to return...", BrightBlack));
    std::cout.flush();

    waitForAnyKey();
    disableRawMode();
    clearScreen();
    return 0;
}