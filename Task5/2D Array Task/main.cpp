#include <iostream>
using namespace std;

#define MAX_ROWS 3
#define MAX_COLS 3

void manipulateArr(int arr[][MAX_COLS], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            arr[i][j] = i * j;
        }
    }
}

int main() {
    int arr[MAX_ROWS][MAX_COLS]; 

    manipulateArr(arr, MAX_ROWS, MAX_COLS);

    for (int i = 0; i < MAX_ROWS; i++) {
        for (int j = 0; j < MAX_COLS; j++) {
            cout << arr[i][j] << "\t";
        }
        cout << "\n";
    }

    return 0;
}
