#ifndef SQUARE_H
#define SQUARE_H

#include "Square.h"
#include "Rectangle.h"

class Square : private Rectangle
{
public:
    Square();
    Square(double x);
    void setDim(double x);
    double getDim();
    double calcArea() override;
};

#endif