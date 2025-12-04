#ifndef CIRCLE_H
#define CIRCLE_H

#include "Circle.h"
#include "GeoShape.h"

class Circle : public GeoShape
{
public:
    Circle();
    Circle(double r);
    void setRadius(double r);
    double getRadius();
    double calcArea() override;
};

#endif