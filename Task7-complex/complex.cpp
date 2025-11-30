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


Complex Complex::operator+(const Complex& other) const {
    return Complex(real + other.real, imag + other.imag);
}

Complex Complex::operator+(int value) const {
    return Complex(real + value, imag);
}

// Non-member function
Complex operator+(int lhs, const Complex& rhs) {
    return Complex(lhs + rhs.real, rhs.imag);
}

Complex Complex::operator-(const Complex& other) const {
    return Complex(real - other.real, imag - other.imag);
}

Complex Complex::operator-(int value) const {
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


Complex Complex::operator-() const {
    return Complex(-real, -imag);
}

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


Complex& Complex::operator+=(const Complex& rhs) {
    real += rhs.real;
    imag += rhs.imag;
    return *this;
}

Complex& Complex::operator-=(const Complex& rhs) {
    real -= rhs.real;
    imag -= rhs.imag;
    return *this;
}


Complex& Complex::operator=(const Complex& other) {
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

