// Circle.cpp
#include "Circle.h"
#include <cmath>

Circle::Circle() : center(), radius(0) {}
Circle::Circle(int m, int n, int r) : center(m, n), radius(r) {}

Point Circle::getCenter() const
{
    return center;
}

int Circle::getRadius() const
{
    return radius;
}
void Circle::setRadius(int r)
{
    radius = r;
}

void Circle::draw(SDL_Renderer *renderer) const
{
    for (int w = 0; w < radius * 2; w++)
    {
        for (int h = 0; h < radius * 2; h++)
        {
            int dx = radius - w;
            int dy = radius - h;
            if ((dx * dx + dy * dy) <= (radius * radius))
            {
                SDL_RenderDrawPoint(renderer,
                                    center.getX() + dx,
                                    center.getY() + dy);
            }
        }
    }
}
