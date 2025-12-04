#ifndef SHAPE_H
#define SHAPE_H

#include <SDL2/SDL.h>

class Shape
{

public:
    virtual void draw(SDL_Renderer *renderer) const = 0;
    virtual ~Shape() {}
};

#endif // SHAPE_H