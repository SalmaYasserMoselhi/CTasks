#ifndef LINE_H
#define LINE_H

#include "Point.h"
#include <SDL2/SDL.h>
#include "Shape.h"

class Line : public Shape
{
    Point start, end;

public:
    Line();
    Line(int x1, int y1, int x2, int y2);
    Point getStart() const;
    Point getEnd() const;
    void draw(SDL_Renderer *renderer) const;
};

#endif // LINE_H