#include "stack.h"


Stack::Stack(int cap) {
    capacity = cap;
    arr = new int[capacity];
    topIndex = -1;
}

Stack::~Stack() {
    delete[] arr;
}

// Constructor - Deep Copy
Stack::Stack(const Stack& other) {
    capacity = other.capacity; // Copy capacity value
    topIndex = other.topIndex; 
    arr = new int[capacity]; // Allocate new arr
    for (int i = 0; i <= topIndex; i++) {
        arr[i] = other.arr[i]; // Copy each element
    }
}

// Assignment operator - Deep Copy - (S1 = S2 = S3) 
Stack& Stack::operator=(const Stack& other) {
    if (this != &other) {  // if S1 == S2 already, skip
        delete[] arr; // Delete old arr of this
        capacity = other.capacity;
        topIndex = other.topIndex;
        arr = new int[capacity];
        for (int i = 0; i <= topIndex; i++) {
            arr[i] = other.arr[i]; 
        }
    }
    return *this;
}

void Stack::push(int value) {
    if (isFull()) {
        cout << "Stack Overflow!" << endl;
        return;
    }
    topIndex++;
    arr[topIndex] = value;
}

int Stack::pop() {
    if (isEmpty()) {
        cout << "Stack Underflow!" << endl;
        return -1;
    }
    return arr[topIndex--];
}

int Stack::top() {
    if (isEmpty()) {
        cout << "Stack is empty!" << endl;
        return -1;
    }
    return arr[topIndex];
}

bool Stack::isEmpty() {
    return topIndex == -1;
}

bool Stack::isFull() {
    return topIndex == capacity - 1;
}

int Stack::size() {
    return topIndex + 1;
}

void Stack::display() {
    if (isEmpty()) {
        cout << "Stack is empty!" << endl;
        return;
    }
    cout << "Stack elements (top to bottom): ";
    for (int i = topIndex; i >= 0; i--) {
        cout << arr[i] << " ";
    }
    cout << endl;
}