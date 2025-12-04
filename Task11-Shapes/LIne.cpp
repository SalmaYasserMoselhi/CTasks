// Line.cpp
#include "Line.h"

Line::Line() : start(), end() {}
Line::Line(int x1, int y1, int x2, int y2) : start(x1, y1), end(x2, y2) {}

Point Line::getStart() const
{
    return start;
}

Point Line::getEnd() const
{
    return end;
}

void Line::draw(SDL_Renderer *renderer) const
{
    SDL_RenderDrawLine(renderer,
                       start.getX(), start.getY(),
                       end.getX(), end.getY());
}
