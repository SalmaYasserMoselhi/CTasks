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

    // Destructor
    ~String();

    // Assignment operators
    String& operator=(const String& other);

    // Accessors
    int length() const;
    bool isEmpty() const;

    // Element access
    char& operator[](int index);
    const char& operator[](int index) const;

    // Modifiers
    void append(const String& str);
    void append(const char* str);
    void append(char ch);

    // Comparison
    bool operator==(const String& other) const;
    bool operator!=(const String& other) const;

    // Concatenation
    String operator+(const String& other) const;
    String operator+(const char* str) const;
    String operator+(char ch) const;
    String& operator+=(const String& other);
    String& operator+=(const char* str);
    String& operator+=(char ch);

    // Stream operators
    friend std::ostream& operator<<(std::ostream& os, const String& str);

    // Static utility functions (can be used in main!)
    static int length(const char* str);
    static char* strcpy(char* dest, const char* src);
    static char* strcat(char* dest, const char* src);

    // Utility functions
    String toUpper() const;
    String toLower() const;
    void reverse();
};

#endif