#ifndef SQUARE_H
#define SQUARE_H

#include "Rectangle.h"

class Square : private Rectangle
{
public:
    Square(double x) : Rectangle(x,x) {}
    void setDim(double x);
    double getDim();
    double calcArea() override;
};

#endif