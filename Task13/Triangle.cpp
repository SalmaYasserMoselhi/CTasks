#include "Triangle.h"

Triangle::Triangle() : GeoShape() {}
Triangle::Triangle(double base, double height) : GeoShape(base, height) {}

double Triangle::calcArea() {
    return 0.5 * dim1 * dim2;
}
