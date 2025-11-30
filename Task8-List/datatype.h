#ifndef DATATYPE_H
#define DATATYPE_H

#include <iostream>
#include <string>
using namespace std;

class DataType {
public:
    virtual void display() = 0;
};

class Int : public DataType {
    int num;
public:
    Int(int n);
    void display();
};

class FLoat : public DataType {
    float num;
public:
    FLoat(float n);
    void display();
};

class Char : public DataType {
    char ch;
public:
    Char(char c);
    void display();
};

class String : public DataType {
    string str;
public:
    String(const char* s);
    String(string s);
    void display();
};

DataType* getDataType(string input);

#endif