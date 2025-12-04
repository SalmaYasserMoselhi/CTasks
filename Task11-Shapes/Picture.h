#ifndef PICTURE_H
#define PICTURE_H

#include "Circle.h"
#include "Rect.h"
#include "Line.h"
#include <SDL2/SDL.h>

class Picture
{
    int cNum, rNum, lNum;
    Circle *pCircles;
    Rect *pRects;
    Line *pLines;

public:
    Picture();
    Picture(int cn, int rn, int ln, Circle *pC, Rect *pR, Line *pL);

    void draw(SDL_Renderer *renderer) const;
};

#endif // PICTURE_H