#ifndef RECT_H
#define RECT_H

#include "Point.h"
#include "Shape.h"
#include <SDL2/SDL.h>

class Rect : public Shape
{
    Point ul, lr;

public:
    Rect();
    Rect(int x1, int y1, int x2, int y2);
    Point getUl() const;
    Point getLr() const;
    void draw(SDL_Renderer *renderer) const;
};

#endif // RECT_H