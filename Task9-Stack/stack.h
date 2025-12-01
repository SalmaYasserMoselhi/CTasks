#ifndef STACK_H
#define STACK_H

#include <iostream>
using namespace std;

class Stack {
private:
    int* arr;
    int capacity; // Stack max Size 
    int topIndex;  // Index of top element

public:
    Stack(int size = 10);
    ~Stack();
    Stack(const Stack& other);
    Stack& operator=(const Stack& other);
    
    void push(int value);
    int pop();
    int top();
    bool isEmpty();
    bool isFull();
    int size();
    void display();
};

#endif