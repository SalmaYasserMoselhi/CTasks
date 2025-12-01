#include "list.h"

List::List() {
    max_size = MAX_SIZE;
    current_size = INTI_SIZE;
    items = new DataType*[max_size];
}

List::~List() {
    for(int i = 0; i < current_size; i++) {
        delete items[i];
    }
    delete[] items;
}

void List::append(DataType* element) {
    if(!element) {
        cout << "Error: Cannot append null element!" << endl;
        return;
    }
    
    if(current_size == max_size) {
        max_size = max_size * 2;
        DataType** new_items = new DataType*[max_size];
        
        for(int i = 0; i < current_size; i++) {
            new_items[i] = items[i];
        }
        
        delete[] items;
        items = new_items;
    }
    
    items[current_size] = element;
    current_size++;
}

void List::append(int value) {
    DataType* element = new Int(value);
    append(element);
}

void List::append(float value) {
    DataType* element = new Float(value);
    append(element);
}

void List::append(char value) {
    DataType* element = new Char(value);
    append(element);
}

void List::append(const char* value) {
    DataType* element = new String(value);
    append(element);
}

void List::append(string value) {
    DataType* element = getDataType(value);
    append(element);
}

void List::append(List* sublist) {
    append((DataType*)sublist);
}

void List::display() {
    cout << "[";
    for(int i = 0; i < current_size; i++) {
        items[i]->display();
        if(i < current_size - 1) {
            cout << ", ";
        }
    }
    cout << "]";
}

int List::length() {
    return current_size;
}

