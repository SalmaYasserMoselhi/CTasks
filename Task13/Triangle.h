#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Triangle.h"
#include "GeoShape.h"

class Triangle : public GeoShape
{
public:
    Triangle();
    Triangle(double base, double height);
    double calcArea() override;
};

#endif