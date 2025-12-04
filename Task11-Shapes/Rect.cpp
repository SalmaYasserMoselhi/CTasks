// Rect.cpp
#include "Rect.h"

Rect::Rect() : ul(), lr() {}
Rect::Rect(int x1, int y1, int x2, int y2) : ul(x1, y1), lr(x2, y2) {}
Point Rect::getUl() const
{
    return ul;
}
Point Rect::getLr() const
{
    return lr;
}

void Rect::draw(SDL_Renderer *renderer) const
{
    SDL_Rect rect;
    rect.x = ul.getX();
    rect.y = ul.getY();
    rect.w = lr.getX() - ul.getX();
    rect.h = lr.getY() - ul.getY();

    SDL_RenderDrawRect(renderer, &rect);
}
