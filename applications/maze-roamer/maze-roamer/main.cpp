#include <SDL.h>

#include <iostream>

#include "maze.h"


int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window;
    window = SDL_CreateWindow("An SDL2 window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

    if (window == NULL) {
        std::cout << "Could not create window: " << SDL_GetError() << '\n';
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    maze m("level01.txt", renderer);
    SDL_SetWindowSize(window, m.get_width(), m.get_height());
    m.initialize();

    bool is_running = true;
    SDL_Event event;

    while (is_running && m.is_running()) {
        SDL_WaitEvent(&event);
        switch (event.type) {
        case SDL_QUIT:
            is_running = false;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_RIGHT:
                m.move_right();
                break;

            case SDLK_LEFT:
                m.move_left();
                break;

            case SDLK_UP:
                m.move_up();
                break;

            case SDLK_DOWN:
                m.move_down();
                break;
            }

            break;

        default:
            break;
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}