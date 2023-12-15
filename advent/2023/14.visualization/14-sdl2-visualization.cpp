#include <SDL.h>
#include <SDL_image.h>


#include <iostream>
#include <fstream>
#include <random>
#include <vector>
#include <string>
#include <unordered_map>

#include <conio.h>


const int TILE_SIZE = 32;


SDL_Window* g_window = nullptr;
SDL_Renderer* g_renderer = nullptr;


struct position {
    int x;
    int y;
};


class texture_manager {
private:
    std::vector<SDL_Texture*> stone;
    SDL_Texture* tile = nullptr;
    SDL_Texture* block = nullptr;

    std::mt19937 generator;

public:
    texture_manager() {
        std::random_device device;
        generator = std::mt19937(device());
    }

    ~texture_manager() {
        for (auto texture : stone) {
            SDL_DestroyTexture(texture);
        }

        SDL_DestroyTexture(tile);
        SDL_DestroyTexture(block);
    }

public:
    void load() {
        for (int i = 1; i <= 7; i++) {
            auto pointer = load("rock-" + std::to_string(i) + ".png");
            stone.push_back(pointer);
        }

        tile = load("tile.jpg");
        block = load("block.jpg");
    }


    SDL_Texture* get_stone() {
       std::uniform_int_distribution<int> distribution(0, 6);
       int stone_index = distribution(generator);

       return stone[stone_index];
    }

    SDL_Texture* get_tile() {
        return tile;
    }

    SDL_Texture* get_block() {
        return block;
    }

private:
    SDL_Texture* load(const std::string& path) {
        SDL_Surface* surface = IMG_Load(path.c_str());
        SDL_Texture* texture = SDL_CreateTextureFromSurface(g_renderer, surface);
        SDL_FreeSurface(surface);

        return texture;
    }
};


struct base_object {
public:
    SDL_Texture* texture = nullptr;

    position current;

public:
    void init(SDL_Texture* _texture) {
        texture = _texture;
    }

    virtual void render() {
        SDL_Rect texture_rect;
        texture_rect.x = current.x;
        texture_rect.y = current.y;
        texture_rect.w = TILE_SIZE;
        texture_rect.h = TILE_SIZE;

        SDL_RenderCopy(g_renderer, texture, NULL, &texture_rect);
    }
};


struct block_object : public base_object { };


struct tile_object : public base_object { };


struct stone_state : public base_object {
    position destination;

    int velocity = 1;

public:
    bool update() {
        bool is_moving = update_coordinate(current.x, destination.x);
        is_moving |= update_coordinate(current.y, destination.y);

        return is_moving;
    }

private:
    bool update_coordinate(int& cur, int dst) {
        bool is_moving = false;

        if (cur < dst) {
            is_moving = true;

            cur += velocity;
            if (cur > dst) {
                cur = dst;
            }
        }
        else if (cur > dst) {
            is_moving = true;

            cur -= velocity;
            if (cur < dst) {
                cur = dst;
            }
        }

        return is_moving;
    }
};


std::vector<std::string> read_input(const int limit_rows, const int limit_cols) {
    std::fstream stream("input.txt");

    std::vector<std::string> result;
    int row_counter = 0;
    for (std::string line; std::getline(stream, line);) {
        row_counter++;

        if (line.size() <= limit_cols) {
            result.push_back(line);
        }
        else {
            result.push_back(line.substr(0, limit_cols));
        }

        if (row_counter >= limit_rows) {
            return result;
        }
    }

    return result;
}


class solution {
    std::vector<std::string> map;

    std::vector<base_object> static_objects;
    std::vector<stone_state> dynamic_objects;

    std::vector<std::vector<stone_state*>> coord_to_dynamic_object = std::vector<std::vector<stone_state*>>(map.size(), std::vector<stone_state*>(map[0].size(), nullptr));

    texture_manager texture_controller;

public:
    solution(const std::vector<std::string>& _plt) :
        map(_plt)
    {
        texture_controller.load();
        dynamic_objects.reserve(1000);

        for (int r = 0; r < map.size(); r++) {
            for (int c = 0; c < map[r].size(); c++) {
                if (map[r][c] == 'O') {
                    int x = TILE_SIZE * c;
                    int y = TILE_SIZE * r;

                    stone_state st;
                    st.current = { x, y };
                    st.destination = { x, y };
                    st.init(texture_controller.get_stone());

                    dynamic_objects.push_back(st);
                    coord_to_dynamic_object[r][c] = &(dynamic_objects[dynamic_objects.size() - 1]);

                    tile_object obj;
                    obj.current = { c * TILE_SIZE, r * TILE_SIZE };
                    obj.init(texture_controller.get_tile());
                    static_objects.push_back(obj);
                }
                else if (map[r][c] == '.') {
                    tile_object obj;
                    obj.current = { c * TILE_SIZE, r * TILE_SIZE };
                    obj.init(texture_controller.get_tile());
                    static_objects.push_back(obj);
                }
                else if (map[r][c] == '#') {
                    block_object obj;
                    obj.current = { c * TILE_SIZE, r * TILE_SIZE };
                    obj.init(texture_controller.get_block());
                    static_objects.push_back(obj);
                }
            }
        }
    }

public:
    void simulate() {
        render();
        SDL_RenderPresent(g_renderer);

        int tilt = 0;

        bool quit = false;
        SDL_Event event;

        SDL_Delay(5000);

        while (!quit) {
            while (SDL_PollEvent(&event) != 0) {
                if (event.type == SDL_QUIT) {
                    quit = true;
                }
            }

            render();

            if (!update()) {
                switch (tilt) {
                case 0:
                    tilt_north();
                    break;

                case 1:
                    tilt_west();
                    break;

                case 2:
                    tilt_south();
                    break;

                case 3:
                    tilt_east();
                    break;
                }

                tilt++;
                if (tilt > 3) {
                    tilt = 0;
                }
            }

            SDL_RenderPresent(g_renderer);
            SDL_Delay(2);
        }
    }

private:
    bool update() {
        bool is_moving = false;

        for (auto& st : dynamic_objects) {
            is_moving |= st.update();
        }

        return is_moving;
    }

    void render() {
        static int counter = 0;
        for (auto& obj : static_objects) {
            obj.render();
        }

        for (auto& obj : dynamic_objects) {
            obj.render();
        }
    }

    stone_state* get_stone_state(const int r, const int c) {
        return coord_to_dynamic_object[r][c];
    }

    void tilt_north() {
        for (int r = 0; r < map.size(); r++) {
            for (int c = 0; c < map[r].size(); c++) {
                if (map[r][c] == 'O') {
                    map[r][c] = '.';

                    stone_state* st = get_stone_state(r, c);
                    int current_free = r;
                    for (int i = current_free; i >= 0 && map[i][c] != '#'; i--) {
                        if (map[i][c] == '.') {
                            current_free = i;
                        }
                    }

                    st->destination.y = current_free * TILE_SIZE;
                    coord_to_dynamic_object[r][c] = nullptr;
                    coord_to_dynamic_object[current_free][c] = st;

                    map[current_free][c] = 'O';
                }
            }
        }
    }

    void tilt_south() {
        for (int r = map.size() - 1; r >= 0; r--) {
            for (int c = 0; c < map[r].size(); c++) {
                if (map[r][c] == 'O') {
                    map[r][c] = '.';

                    stone_state* st = coord_to_dynamic_object[r][c];
                    int current_free = r;
                    for (int i = current_free; i < map.size() && map[i][c] != '#'; i++) {
                        if (map[i][c] == '.') {
                            current_free = i;
                        }
                    }

                    st->destination.y = current_free * TILE_SIZE;
                    coord_to_dynamic_object[r][c] = nullptr;
                    coord_to_dynamic_object[current_free][c] = st;

                    map[current_free][c] = 'O';
                }
            }
        }
    }

    void tilt_west() {
        for (int c = 0; c < map[0].size(); c++) {
            for (int r = 0; r < map.size(); r++) {
                if (map[r][c] == 'O') {
                    map[r][c] = '.';

                    stone_state* st = get_stone_state(r, c);
                    int current_free = c;
                    for (int i = current_free; i >= 0 && map[r][i] != '#'; i--) {
                        if (map[r][i] == '.') {
                            current_free = i;
                        }
                    }

                    st->destination.x = current_free * TILE_SIZE;   /* col */
                    coord_to_dynamic_object[r][c] = nullptr;
                    coord_to_dynamic_object[r][current_free] = st;

                    map[r][current_free] = 'O';
                }
            }
        }
    }

    void tilt_east() {
        for (int c = map[0].size() - 1; c >= 0; c--) {
            for (int r = 0; r < map.size(); r++) {
                if (map[r][c] == 'O') {
                    map[r][c] = '.';

                    stone_state* st = get_stone_state(r, c);
                    int current_free = c;
                    for (int i = current_free; i < map[0].size() && map[r][i] != '#'; i++) {
                        if (map[r][i] == '.') {
                            current_free = i;
                        }
                    }

                    st->destination.x = current_free * TILE_SIZE;
                    coord_to_dynamic_object[r][c] = nullptr;
                    coord_to_dynamic_object[r][current_free] = st;

                    map[r][current_free] = 'O';
                }
            }
        }
    }
};


int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    auto input = read_input(28, 28);

    const int window_width = input[0].size() * TILE_SIZE;
    const int window_height = input.size() * TILE_SIZE;

    g_window = SDL_CreateWindow("Advent 2023 - Day 14 - SDL Visualization", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width, window_height, SDL_WINDOW_SHOWN);
    g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);

    {
        solution(input).simulate();
    }

    SDL_DestroyRenderer(g_renderer);
    SDL_DestroyWindow(g_window);
    SDL_Quit();

    return 0;
}

