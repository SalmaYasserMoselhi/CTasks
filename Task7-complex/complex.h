#ifndef COMPLEX_H
#define COMPLEX_H

#include <iostream>
#include <string>
#include <cmath>
#include <stdexcept>
using namespace std;

class Complex {
private:
    int real;
    int imag;

public:
    Complex(int r = 0, int i = 0); // Parameterized constructor with default values

    ~Complex();  // Destructor

    // Setters and Getters
    void setReal(int r);
    void setImag(int i);
    int getReal() const;
    int getImag() const;
    void display() const;
    
    // Overloaded Operators
    Complex operator+(const Complex& other) const; // Complex + Complex
    Complex operator+(int value) const; //  Complex(real) + value
    friend Complex operator+(int lhs, const Complex& rhs); // value + Complex
    Complex operator-(const Complex& other) const; // Complex - Complex
    Complex operator-(int value) const; // Complex(real) - value
    Complex operator*(const Complex& other) const; // Complex * Complex
    Complex operator/(const Complex& other) const;

    
    Complex operator-() const; // minus 
    
    // io operators
    friend std::ostream& operator<<(std::ostream& os, const Complex& c);
    friend std::istream& operator>>(std::istream& is, Complex& c);
    
    Complex& operator+=(const Complex& rhs); // complex += complex
    Complex& operator-=(const Complex& rhs);
    Complex& operator=(const Complex& other); // Assignment operator
    
    // Comparison Operators
    bool operator==(const Complex& other) const;
    bool operator!=(const Complex& other) const;
    
    Complex& operator++();      // Prefix ++
    Complex operator++(int);    // Postfix ++

    // Subscript operator
    int& operator[](int index);
    double operator()() const;

    // NOT operator
    bool operator!() const;

    // Casting operator
    operator int() const;
};

#endif // COMPLEX_H
