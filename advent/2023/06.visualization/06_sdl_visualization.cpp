#include <SDL.h>
#include <SDL_image.h>

#include <vector>


int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("Advent 2023 - Day 6 - SDL Visualization", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 920, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Surface* surface = IMG_Load("background.jpg");
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    SDL_Surface* boat_surface = IMG_Load("boat.png");
    SDL_Texture* boat_texture = SDL_CreateTextureFromSurface(renderer, boat_surface);
    SDL_FreeSurface(boat_surface);

    SDL_Rect background_rect;
    background_rect.x = 0;
    background_rect.y = 0;
    background_rect.w = 1280;
    background_rect.h = 920;

    SDL_Rect basic_boat_rect;
    basic_boat_rect.x = 0;
    basic_boat_rect.y = 920 - 64;
    basic_boat_rect.w = 64;
    basic_boat_rect.h = 64;

    bool quit = false;
    SDL_Event event;

    const int STATE_SHOW = 1;
    const int STATE_MOVE = 2;
    const int STATE_FINAL = 3;

    const int DELAY = 20;
    const int STATE_SHOW_TRANSITION = 500;
    const int STATE_MOVE_TRANSITION = 32;
    const int STATE_FINAL_TRANSITION = 4000;

    int state = STATE_SHOW;
    int state_value = 0;
    int global_time = 0;

    const int TIME = 9;
    const int DISTANCE = 9;

    std::vector<int> boats_x_position(TIME, 0);
    std::vector<int> boats_velocity(TIME, 0);
    for (int i = 0; i < TIME; i++) {
        boats_velocity[i] = i;
    }

    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, texture, NULL, &background_rect);

        for (int i = 0; i < TIME; i++) {
            auto p = basic_boat_rect;
            p.x = boats_x_position[i];
            p.y = 920 - 64 * (i + 1);

            SDL_RenderCopy(renderer, boat_texture, NULL, &p);
        }

        SDL_RenderPresent(renderer);

        switch (state) {
            case STATE_SHOW: {
                state_value++;
                if (state_value * DELAY > STATE_SHOW_TRANSITION) {
                    state_value = 0;
                    state = STATE_MOVE;

                    /* update position */
                    global_time++;
                    if (global_time >= TIME) {
                        state = STATE_FINAL;
                    }
                }
                break;
            }

            case STATE_MOVE: {
                state_value++;
                if (state_value > STATE_MOVE_TRANSITION) {
                    state_value = 0;
                    if (global_time < TIME) {
                        state = STATE_SHOW;
                    }
                    else {
                        state = STATE_FINAL;
                    }
                }
                else {
                    for (int i = 0; i < global_time; i++) {
                        int v = boats_velocity[i];
                        if (v < global_time) {
                            boats_x_position[i] += v * 2;
                        }
                    }
                }
                break;
            }

            case STATE_FINAL: {
                state_value++;
                if (state_value * DELAY > STATE_FINAL_TRANSITION) {
                    state_value = 0;
                    state = STATE_SHOW;
                    global_time = 0;
                    boats_x_position = std::vector<int>(boats_x_position.size(), 0);
                }
                break;
            }
        }

        SDL_Delay(15);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}