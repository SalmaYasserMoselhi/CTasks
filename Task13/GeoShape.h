#ifndef GEOSHAPE_H
#define GEOSHAPE_H

#include "GeoShape.h"

class GeoShape
{
protected:
    double dim1, dim2;

public:
    GeoShape();
    GeoShape(double dim1, double dim2);

    void setDim1(double dim1);
    void setDim2(double dim1);

    double getDim1();
    double getDim2();

    virtual double calcArea() = 0;
};

#endif