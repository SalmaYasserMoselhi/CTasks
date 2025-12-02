#ifndef STRING_H
#define STRING_H

#include <iostream>

class String {
private:
    char* data;
    int len;

public:
    // Constructors
    String();
    String(const char* str);
    String(const String& other);

    ~String();

    String& operator=(const String& other);

    int length() const;
    bool isEmpty() const;

    char& operator[](int index);

    void append(const String& str);
    void append(const char* str);
    void append(char ch);

    bool operator==(const String& other) const;
    bool operator!=(const String& other) const;

    String operator+(const String& other) const;
    String operator+(const char* str) const;
    String operator+(char ch) const;
    String& operator+=(const String& other);
    String& operator+=(const char* str);
    String& operator+=(char ch);

    friend std::ostream& operator<<(std::ostream& os, const String& str);

    static int length(const char* str);
    static char* strcpy(char* dest, const char* src);
    static char* strcat(char* dest, const char* src);

    String toUpper() const;
    String toLower() const;
    void reverse();
};

#endif