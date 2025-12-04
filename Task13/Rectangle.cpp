#include "Rectangle.h"

Rectangle::Rectangle() : GeoShape() {}
Rectangle::Rectangle(double x, double y) : GeoShape(x, y) {}

double Rectangle::calcArea() {
    return dim1 * dim2;
}
