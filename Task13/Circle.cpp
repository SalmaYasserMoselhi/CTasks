#include "Circle.h"


void Circle::setRadius(double r)
{
    dim1 = dim2 = r;
}

double Circle::getRadius()
{
    return dim1;
}

double Circle::calcArea()
{
    return 3.14159 * dim1 * dim2;
}