#include "Square.h"

Square::Square() : Rectangle() {}
Square::Square(double x) : Rectangle(x, x) {}

void Square::setDim(double x) {
    dim1 = dim2 = x;
}

double Square::getDim() {
    return dim1;
}

double Square::calcArea() {
    return Rectangle::calcArea();
}
