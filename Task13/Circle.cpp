#include "Circle.h"

Circle::Circle() : GeoShape() {}
Circle::Circle(double r) : GeoShape(r, r) {}

double Circle::calcArea() {
    return 0.5 * dim1 * dim2;
}
