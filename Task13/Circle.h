#ifndef CIRCLE_H
#define CIRCLE_H

#include "GeoShape.h"

class Circle : public GeoShape
{
public:
    Circle(double r) : GeoShape(r, r) {} 
    void setRadius(double r);
    double getRadius();
    double calcArea() override;
};

#endif