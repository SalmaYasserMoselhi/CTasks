#include <iostream>
using namespace std;
#define MAX 100

void Input2D(int arr[][MAX], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cin >> arr[i][j];
        }
    }
}

void Display2D(int arr[][MAX], int rows, int cols) {
    cout << "The 2D Array is:" << endl;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << arr[i][j] << "\t";
        }
        cout << endl;
    }
}

int main() {
    int rows, cols;
    cout << "Enter number of rows: ";
    cin >> rows;
    cout << "Enter number of columns: ";
    cin >> cols;

    int arr[MAX][MAX];

    Input2D(arr, rows, cols);
    Display2D(arr, rows, cols);

    return 0;
}
