#include <SDL2/SDL.h>
#include "Line.h"
#include "Rect.h"
#include "Circle.h"
#include "Picture.h"

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Lamp Drawing",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          800, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    Circle circles[1] = {
        Circle(400, 270, 35)  // اللمبة
    };

    Rect rects[1] = {
        Rect(350, 400, 450, 470)  // القاعدة
    };

    Line lines[4] = {
        Line(250, 250, 400, 100),    // topLeft
        Line(400, 100, 550, 250),    // topRight
        Line(250, 250, 550, 250),    // shadeBottom
        Line(400, 305, 400, 400)     // stem
    };

    Picture lamp(1, 1, 4, circles, rects, lines);

    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = false;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); 

        lamp.draw(renderer);  // 🎉

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}