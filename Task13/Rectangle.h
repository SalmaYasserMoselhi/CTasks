#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "GeoShape.h"

class Rectangle : public GeoShape
{
public:
    Rectangle(double x, double y) : GeoShape(x,y) {}
    double calcArea() override;
};

#endif