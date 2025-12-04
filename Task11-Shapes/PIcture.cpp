// Picture.cpp
#include "Picture.h"

Picture::Picture() : cNum(0), rNum(0), lNum(0), pCircles(nullptr), pRects(nullptr), pLines(nullptr) {}
Picture::Picture(int cn, int rn, int ln, Circle *pC, Rect *pR, Line *pL)
    : cNum(cn), rNum(rn), lNum(ln), pCircles(pC), pRects(pR), pLines(pL) {}

void Picture::draw(SDL_Renderer *renderer) const
{
    for (int i = 0; i < cNum; i++)
        pCircles[i].draw(renderer);

    for (int i = 0; i < rNum; i++)
        pRects[i].draw(renderer);

    for (int i = 0; i < lNum; i++)
        pLines[i].draw(renderer);
}
