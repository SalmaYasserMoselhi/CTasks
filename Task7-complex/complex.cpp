#include "complex.h"

Complex::Complex(int r, int i) {
    real = r;
    imag = i;
    // cout << "Creating Complex object: " << *this << endl;
}

Complex::~Complex() {
    // cout << "Destroying Complex object: " << *this << endl;
}

int Complex::getReal() const {
    return real;
}

int Complex::getImag() const {
    return imag;
}

void Complex::setReal(int r) {
    real = r;
}

void Complex::setImag(int i) {
    imag = i;
}

void Complex::display() const {
    cout << real << " + " << imag << "i" << endl;
}


Complex Complex::operator+(const Complex& other) const { // c + c1
    return Complex(real + other.real, imag + other.imag);
}

Complex Complex::operator+(int value) const { // c + 5
    return Complex(real + value, imag);
}

// Non-member function
Complex operator+(int lhs, const Complex& rhs) { // 5 + c
    return Complex(lhs + rhs.real, rhs.imag);
}

Complex Complex::operator-(const Complex& other) const {  // c - c1
    return Complex(real - other.real, imag - other.imag);
}

Complex Complex::operator-(int value) const { // c - 5
    return Complex(real - value, imag);
}

// (a + bi)*(c + di) = (ac - bd) + (ad + bc)i
Complex Complex::operator*(const Complex& other) const {
    int newReal = (real * other.real) - (imag * other.imag);
    int newImag = (real * other.imag) + (imag * other.real);
    return Complex(newReal, newImag);
}

// (a + bi)/(c + di) = ((ac + bd)/(c*c + d*d)) + ((bc - ad)/(c*c + d*d))i
Complex Complex::operator/(const Complex& other) const {
    int denominator = (other.real * other.real) + (other.imag * other.imag);
    if (denominator == 0) {
        throw runtime_error("Error: Division by zero complex number");
    }

    int newReal = (real * other.real + imag * other.imag) / denominator;
    int newImag = (imag * other.real - real * other.imag) / denominator;

    return Complex(newReal, newImag);
}


Complex Complex::operator-() const { // -c
    return Complex(-real, -imag);
}

// Non-member functions
ostream& operator<<(ostream& os, const Complex& c) {
    os << "(" << c.real;
    if (c.imag >= 0)
        os << " + " << c.imag << "i)";
    else
        os << " - " << abs(c.imag) << "i)";
    return os;
}

istream& operator>>(istream& is, Complex& c) {
    cout << "Enter real: ";
    is >> c.real;
    cout << "Enter imaginary: ";
    is >> c.imag;
    return is;
}


Complex& Complex::operator+=(const Complex& rhs) { // c += c1
    real += rhs.real;
    imag += rhs.imag;
    return *this;
}

Complex& Complex::operator-=(const Complex& rhs) { // c -= c1
    real -= rhs.real;
    imag -= rhs.imag;
    return *this;
}


Complex& Complex::operator=(const Complex& other) { // c = c1
    if (this == &other) return *this;
    real = other.real;
    imag = other.imag;
    return *this;
}

bool Complex::operator==(const Complex& other) const {
    return (real == other.real) && (imag == other.imag);
}

bool Complex::operator!=(const Complex& other) const {
    return !(*this == other);
}


// returns real part for [0], imaginary for [1]
int& Complex::operator[](int index) {
    if (index == 0) {
        return real;
    } else if (index == 1) {
        return imag;
    } else {
        throw out_of_range("Index must be 0 (real) or 1 (imaginary)");
    }
}

// Prefix increment: ++c
Complex& Complex::operator++() {
    ++real;
    return *this;
}

// Postfix increment: c++
Complex Complex::operator++(int) {
    Complex temp(*this);
    ++real;
    return temp;
}

// Logical NOT operator - returns true if complex number is zero
bool Complex::operator!() const {
    return (real == 0 && imag == 0);
}

