#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "GeoShape.h"

class Triangle : public GeoShape
{
public:
    Triangle(double base, double height) : GeoShape(base, height) {}
    double calcArea() override;
};

#endif