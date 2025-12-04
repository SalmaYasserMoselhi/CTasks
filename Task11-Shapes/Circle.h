#ifndef CIRCLE_H
#define CIRCLE_H

#include "Shape.h"
#include "Point.h"
#include <SDL2/SDL.h>

class Circle : public Shape
{
    Point center;
    int radius;

public:
    Circle();
    Circle(int m, int n, int r);
    Point getCenter() const;
    int getRadius() const;
    void setRadius(int r);
    void draw(SDL_Renderer *renderer) const override;
};

#endif // CIRCLE_H