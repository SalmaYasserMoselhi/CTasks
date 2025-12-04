#include "GeoShape.h"

GeoShape::GeoShape()
{
    dim1 = 0;
    dim2 = 0;
};

GeoShape::GeoShape(double dim1, double dim2)
{
    this->dim1 = dim1;
    this->dim2 = dim2;
};

void GeoShape::setDim1(double dim1)
{
    this->dim1 = dim1;
}

void GeoShape::setDim2(double dim2)
{
    this->dim2 = dim2;
}

double GeoShape::getDim1()
{
    return dim1;
}

double GeoShape::getDim2()
{
    return dim2;
}
