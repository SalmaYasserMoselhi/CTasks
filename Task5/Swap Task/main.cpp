#include <iostream>
using namespace std;

void swapAddSub(int &a, int &b) {
    a = a + b;
    b = a - b;
    a = a - b;
}

void swapMulDiv(int &a, int &b) {
    a = a * b;
    b = a / b;
    a = a / b;
}

void swapXOR(int &a, int &b) {
    a = a ^ b;
    b = a ^ b;
    a = a ^ b;
}


int main() {
    int x = 5, y = 10;

    cout << "x=" << x << " y=" << y << endl;

    swapAddSub(x, y);
    cout << "swap using Add & Sub: x=" << x << " y=" << y << endl;

    swapMulDiv(x, y);
    cout << "swap using Mul & Div: x=" << x << " y=" << y << endl;

    swapXOR(x, y);
    cout << "swap using XOR: x=" << x << " y=" << y << endl;

    return 0;
}


/*
0010
0100
----- XOR
0110
----- XOR
0010
----- XOR
0100
*/