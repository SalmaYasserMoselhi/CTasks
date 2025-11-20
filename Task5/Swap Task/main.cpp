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
    cout << "swapAddSub: x=" << x << " y=" << y << endl;

    swapMulDiv(x, y);
    cout << "swapMulDiv: x=" << x << " y=" << y << endl;

    swapXOR(x, y);
    cout << "swapXOR: x=" << x << " y=" << y << endl;

    return 0;
}
