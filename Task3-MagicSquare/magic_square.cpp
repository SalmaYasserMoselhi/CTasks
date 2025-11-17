#include "magic_square.h"
#include "terminal_utils.h"
#include <iostream>

void drawMagicSquare(int n, int startX, int startY, int cellWidth) {
    clearScreen();

    int row = 0;
    int col = n / 2;

    drawText(startX + col * cellWidth, startY + row * 2, colorText("1", BrightBlue));
    delay(500);

    for (int k = 2; k <= n * n; k++) {
        if ((k - 1) % n == 0) {
            row = (row + 1) % n; // move down
        } else {
            row = (row - 1 + n) % n; // move up
            col = (col + 1) % n;     // move right
        }

        int x = startX + col * cellWidth;
        int y = startY + row * 2;
        drawText(x, y, colorText(std::to_string(k), BrightBlue));
        delay(500);
    }

    int sum = magicConstant(n);
    std::string msg = "Magic square completed! Magic sum = " + std::to_string(sum) + '\n';

    int msgY = startY + n * 2 + 1;
    int msgX = startX + (n * cellWidth) / 2 - (msg.length() / 2);

    drawText(msgX, msgY, colorText(msg, BrightGreen));
    delay(5000);
}

int magicConstant(int n) {
    return n * (n * n + 1) / 2;
}

int askUserForN() {
    int n;
    while (true) {
        std::cout << "Enter an odd number for magic square size: ";
        std::cin >> n;
        if (n > 0 && n % 2 == 1) {
            return n;
        } else {
            std::cout << "Invalid input! You must enter an odd number.\n";
        }
    }
}
