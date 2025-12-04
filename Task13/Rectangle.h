#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "Rectangle.h"
#include "GeoShape.h"

class Rectangle : public GeoShape
{
public:
    Rectangle();
    Rectangle(double x, double y);
    double calcArea() override;
};

#endif