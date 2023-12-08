#include <SDL.h>
#include <SDL_image.h>

#include <cmath>
#include <vector>


void draw_filled_circle(SDL_Renderer* renderer, int center_x, int center_y, int radius) {
    int x = radius;
    int y = 0;
    int radius_error = 1 - x;

    while (x >= y) {
        SDL_RenderDrawLine(renderer, center_x - x, center_y + y, center_x + x, center_y + y);
        SDL_RenderDrawLine(renderer, center_x - x, center_y - y, center_x + x, center_y - y);

        if (x != y) {
            SDL_RenderDrawLine(renderer, center_x - y, center_y + x, center_x + y, center_y + x);
            SDL_RenderDrawLine(renderer, center_x - y, center_y - x, center_x + y, center_y - x);
        }

        y++;

        if (radius_error < 0) {
            radius_error += 2 * y + 1;
        }
        else {
            x--;
            radius_error += 2 * (y - x + 1);
        }
    }
}


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

    const int STATE_FINAL_PARABOLA_DELAY = 300;
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
                else {
                    int total_steps = STATE_FINAL_TRANSITION / DELAY;
                    int steps_to_draw = total_steps >> 1;
                    int boats_per_step = std::ceil(steps_to_draw / boats_x_position.size());

                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

                    for (int i = 1; i <= state_value; i++) {
                        if (i >= boats_x_position.size()) {
                            continue;
                        }

                        int x1 = boats_x_position[i - 1] + 32;
                        int x2 = boats_x_position[i] + 32;

                        int y1 = 920 - 64 * i + 32;
                        int y2 = 920 - 64 * (i + 1) + 32;

                        draw_filled_circle(renderer, x1, y1, 10);
                        draw_filled_circle(renderer, x2, y2, 10);

                        for (int k = 0; k < 2; k++) {
                            SDL_RenderDrawLine(renderer, x1, y1 + 1, x2, y2 + 1);
                            SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
                            SDL_RenderDrawLine(renderer, x1, y1 - 1, x2, y2 - 1);
                        }
                    }
                }
                break;
            }
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(15);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}