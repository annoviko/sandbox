#include "maze.h"

#include <fstream>

#include <SDL_image.h>

#include "monster_random.h"
#include "monster_bfs.h"


maze::maze(const std::string& p_filepath, SDL_Renderer * p_renderer) :
    m_renderer(p_renderer)
{
    std::ifstream stream(p_filepath);

    int row = 0;
    for (std::string line; std::getline(stream, line); ) {
        m_maze.push_back(line);

        std::size_t col = line.find('P');
        if (col != std::string::npos) {
            m_player = { (int) col, row };
        }

        row++;
    }

    m_texture_coin = IMG_LoadTexture(m_renderer, "img/coin.png");
    SDL_SetTextureBlendMode(m_texture_coin, SDL_BLENDMODE_BLEND);

    m_texture_player = IMG_LoadTexture(m_renderer, "img/player.png");
    SDL_SetTextureBlendMode(m_texture_player, SDL_BLENDMODE_BLEND);

    m_texture_monster = IMG_LoadTexture(m_renderer, "img/monster.png");
    SDL_SetTextureBlendMode(m_texture_monster, SDL_BLENDMODE_BLEND);

    m_texture_monster_bfs = IMG_LoadTexture(m_renderer, "img/monster-bfs.png");
    SDL_SetTextureBlendMode(m_texture_monster_bfs, SDL_BLENDMODE_BLEND);
}


maze::~maze() {
    m_is_running = false;
    m_monster_thread.join();
}


void maze::initialize() {
    int x = 0, y = 0;

    std::vector<position> stupid_monsters;
    std::vector<position> clever_monsters;

    for (int i = 0; i < m_maze.size(); i++) {
        for (int j = 0; j < m_maze[0].size(); j++) {
            auto& value = m_maze[i][j];

            render_object(value, x, y, false);
            x += OBJECT_SIZE;

            if (value == 'S') {
                stupid_monsters.push_back({ j, i });
            }

            if (value == 'C') {
                clever_monsters.push_back({ j, i });
            }

            if (value != '*' && value != ' ' && value != '$') {
                value = ' '; // clean dynamic objects.
            }
        }

        x = 0;
        y += OBJECT_SIZE;
    }

    SDL_RenderPresent(m_renderer);

    for (const auto& pos : stupid_monsters) {
        m_monsters.push_back(std::make_shared<monster_random>('S', &m_maze, pos));
    }

    for (const auto& pos : clever_monsters) {
        m_monsters.push_back(std::make_shared<monster_bfs>('C', &m_maze, pos));
    }

    m_monster_thread = std::thread(&maze::thread_monster, this);
}


void maze::render_object(const char p_obj_id, const int p_x, const int p_y, const bool p_render_static_only = false) {
    SDL_Rect rect{ p_x, p_y, OBJECT_SIZE, OBJECT_SIZE };

    switch (p_obj_id) {
        case '*': {
            SDL_SetRenderDrawColor(m_renderer, 0, 220, 220, 220);
            SDL_RenderFillRect(m_renderer, &rect);
            break;
        }
        case ' ': {
            SDL_SetRenderDrawColor(m_renderer, 0, 255, 255, 255);
            SDL_RenderFillRect(m_renderer, &rect);
            break;
        }
        case '$': {
            SDL_SetRenderDrawColor(m_renderer, 0, 255, 255, 255);
            SDL_RenderFillRect(m_renderer, &rect);
            SDL_RenderCopy(m_renderer, m_texture_coin, NULL, &rect);
            break;
        }
        case 'P': {
            if (p_render_static_only) {
                break;
            }

            SDL_SetRenderDrawColor(m_renderer, 0, 255, 255, 255);
            SDL_RenderFillRect(m_renderer, &rect);
            SDL_RenderCopy(m_renderer, m_texture_player, NULL, &rect);
            break;
        }
        case 'S': {
            if (p_render_static_only) {
                break;
            }

            SDL_SetRenderDrawColor(m_renderer, 0, 255, 255, 255);
            SDL_RenderFillRect(m_renderer, &rect);
            SDL_RenderCopy(m_renderer, m_texture_monster, NULL, &rect);
            break;
        }
        case 'C': {
            if (p_render_static_only) {
                break;
            }

            SDL_SetRenderDrawColor(m_renderer, 0, 255, 255, 255);
            SDL_RenderFillRect(m_renderer, &rect);
            SDL_RenderCopy(m_renderer, m_texture_monster_bfs, NULL, &rect);
            break;
        }
    }
}


void maze::thread_monster() {
    while (m_is_running) {
        {
            std::lock_guard<std::mutex> guard(m_mutex);

            if (!m_is_running) {
                continue;
            }

            std::vector<position> old_positions;
            std::vector<std::pair<position, char>> new_positions;

            for (auto& monster_ptr : m_monsters) {
                position new_position = monster_ptr->move(m_player);

                old_positions.push_back(monster_ptr->get_previous_position());
                new_positions.push_back({ new_position, monster_ptr->get_id() });

                /* TODO: player is dead? */
                if (new_position == m_player) {
                    game_over();
                }
            }

            for (const auto& pos : old_positions) {
                int x = pos.x * OBJECT_SIZE;
                int y = pos.y * OBJECT_SIZE;

                render_object(m_maze[pos.y][pos.x], x, y, true);
            }

            for (const auto& pos : new_positions) {
                int x = pos.first.x * OBJECT_SIZE;
                int y = pos.first.y * OBJECT_SIZE;

                render_object(pos.second, x, y, false);
            }

            SDL_RenderPresent(m_renderer);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}


bool maze::is_running() const {
    return m_is_running;
}


void maze::game_over() {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over", "Game Over", nullptr);
    m_is_running = false;
}


void maze::render_player_movement(const position& p_prev, const position& p_cur) {
    int x = p_prev.x * OBJECT_SIZE;
    int y = p_prev.y * OBJECT_SIZE;

    SDL_Rect rect{ x, y, OBJECT_SIZE, OBJECT_SIZE };

    SDL_SetRenderDrawColor(m_renderer, 0, 255, 255, 255);
    SDL_RenderFillRect(m_renderer, &rect);

    rect = { p_cur.x * OBJECT_SIZE, p_cur.y * OBJECT_SIZE, OBJECT_SIZE, OBJECT_SIZE };

    SDL_RenderFillRect(m_renderer, &rect);
    SDL_RenderCopy(m_renderer, m_texture_player, NULL, &rect);
    SDL_RenderPresent(m_renderer);
}


bool maze::is_inside(const position& p_pos) const {
    return ((p_pos.x >= 0 && p_pos.x < m_maze.front().size()) &&
        (p_pos.y >= 0 && p_pos.y < m_maze.size()));
}


bool maze::is_wall(const position& p_pos) const {
    if (!is_inside(p_pos)) {
        return false;
    }

    return m_maze[p_pos.y][p_pos.x] == '*';
}


void maze::move_right() {
    position next_pos = { m_player.x + 1, m_player.y };
    move(m_player, next_pos);
}


void maze::move_left() {
    position next_pos = { m_player.x - 1, m_player.y };
    move(m_player, next_pos);
}


void maze::move_up() {
    position next_pos = { m_player.x, m_player.y - 1 };
    move(m_player, next_pos);
}


void maze::move_down() {
    position next_pos = { m_player.x, m_player.y + 1 };
    move(m_player, next_pos);
}


void maze::move(const position& p_prev, const position& p_next) {
    std::lock_guard<std::mutex> guard(m_mutex);

    if (is_wall(p_next)) {
        return;
    }

    for (auto& monster : m_monsters) {
        if (p_next == monster->get_current_position()) {
            game_over();
            return;
        }
    }

    render_player_movement(p_prev, p_next); // update
    m_player = p_next;
}



int maze::get_height() const {
    return OBJECT_SIZE * m_maze.size();
}


int maze::get_width() const {
    return OBJECT_SIZE * m_maze.front().size();
}
