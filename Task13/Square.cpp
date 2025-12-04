#include "Square.h"

void Square::setDim(double x) {
    dim1 = dim2 = x;
}

double Square::getDim() {
    return dim1;
}

double Square::calcArea() {
    return dim1 * dim2;
}
