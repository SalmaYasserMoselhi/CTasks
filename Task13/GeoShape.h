#ifndef GEOSHAPE_H
#define GEOSHAPE_H

class GeoShape
{
protected:
    double dim1, dim2;

public:
    GeoShape(double dim1 = 0, double dim2 = 0);

    void setDim1(double dim1);
    void setDim2(double dim2);

    double getDim1();
    double getDim2();

    virtual double calcArea() = 0;
};

#endif