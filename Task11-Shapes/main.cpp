#include <SDL2/SDL.h>
#include "Line.h"
#include "Rect.h"
#include "Circle.h"

int main()
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Lamp Drawing",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          800, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // === شكل الأباجورة المكبر ===
    Line topLeft(250, 250, 400, 100);
    Line topRight(400, 100, 550, 250);
    Line shadeBottom(250, 250, 550, 250);

    Circle bulb(400, 270, 35);
    Line stem(400, 305, 400, 400);
    Rect base(350, 400, 450, 470);

    bool running = true;
    SDL_Event event;
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                running = false;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); 

        topLeft.draw(renderer);
        topRight.draw(renderer);
        shadeBottom.draw(renderer);
        bulb.draw(renderer);
        stem.draw(renderer);
        base.draw(renderer);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
