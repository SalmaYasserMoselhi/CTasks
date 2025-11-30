#ifndef LIST_H
#define LIST_H

#include "datatype.h"
#define MAX_SIZE 10
#define INTI_SIZE 0

class List : public DataType{
private:
    DataType** items;
    int max_size;
    int current_size;

public:
    List();
    ~List();
    
    void append(DataType* element);
    void append(int value);
    void append(float value);
    void append(char value);
    void append(const char* value);
    void append(string value);
    void append(List* sublist);
    
    void display() override;
    int length();
};

#endif
