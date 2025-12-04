#ifndef STACK_H
#define STACK_H

#include <iostream>
using namespace std;

template <typename T>
class Stack {
private:
    T* arr;
    int capacity;
    int topIndex;

public:
    Stack(int size = 10);
    ~Stack();
    Stack(const Stack& other);
    Stack& operator=(const Stack& other);
    
    void push(T value);
    T pop();
    T top();
    bool isEmpty();
    bool isFull();
    int size();
    void display();
};


template <typename T>
Stack<T>::Stack(int cap) {
    capacity = cap;
    arr = new T[capacity];
    topIndex = -1;
}

template <typename T>
Stack<T>::~Stack() {
    delete[] arr;
}

// Copy Constructor - Deep Copy
template <typename T>
Stack<T>::Stack(const Stack& other) {
    capacity = other.capacity;
    topIndex = other.topIndex;
    arr = new T[capacity];
    for (int i = 0; i <= topIndex; i++) {
        arr[i] = other.arr[i];
    }
}

// Assignment Operator - Deep Copy
template <typename T>
Stack<T>& Stack<T>::operator=(const Stack& other) {
    if (this != &other) {
        delete[] arr;
        capacity = other.capacity;
        topIndex = other.topIndex;
        arr = new T[capacity];
        for (int i = 0; i <= topIndex; i++) {
            arr[i] = other.arr[i];
        }
    }
    return *this;
}

template <typename T>
void Stack<T>::push(T value) {
    if (isFull()) {
        cout << "Stack Overflow!" << endl;
        return;
    }
    topIndex++;
    arr[topIndex] = value;
}

template <typename T>
T Stack<T>::pop() {
    if (isEmpty()) {
        cout << "Stack Underflow!" << endl;
        return T(); // Return default value for type T
    }
    return arr[topIndex--];
}

template <typename T>
T Stack<T>::top() {
    if (isEmpty()) {
        cout << "Stack is empty!" << endl;
        return T(); // Return default value for type T
    }
    return arr[topIndex];
}

template <typename T>
bool Stack<T>::isEmpty() {
    return topIndex == -1;
}

template <typename T>
bool Stack<T>::isFull() {
    return topIndex == capacity - 1;
}

template <typename T>
int Stack<T>::size() {
    return topIndex + 1;
}

template <typename T>
void Stack<T>::display() {
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

#endif