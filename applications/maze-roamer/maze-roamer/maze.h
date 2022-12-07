#pragma once

#include <memory>
#include <mutex>
#include <string>
#include <vector>
#include <thread>

#include <SDL.h>

#include "level_map.h"
#include "position.h"

#include "monster.h"


class maze {
private:
    static constexpr int OBJECT_SIZE = 32;

private:
    level_map m_maze;

    std::mutex m_mutex;
    SDL_Renderer * m_renderer;

    SDL_Texture* m_texture_coin;
    SDL_Texture* m_texture_player;
    SDL_Texture* m_texture_monster;
    SDL_Texture* m_texture_monster_bfs;

    std::vector<std::shared_ptr<monster>> m_monsters;
    std::thread m_monster_thread;

    bool m_is_running = true;
    position m_player;

public:
    maze(const std::string& p_filepath, SDL_Renderer * p_renderer);

    ~maze();

    void move_right();

    void move_left();

    void move_up();

    void move_down();

    void move(const position& p_prev, const position& p_next);

    int get_height() const;

    int get_width() const;

    bool is_running() const;

public:
    void initialize();

private:
    void game_over();

    void thread_monster();

    void render_object(const char p_obj_id, const int p_x, const int p_y, const bool p_render_static_only);

    bool is_inside(const position& p_pos) const;

    bool is_wall(const position& p_pos) const;

    void render_player_movement(const position& p_prev, const position& p_cur);
};