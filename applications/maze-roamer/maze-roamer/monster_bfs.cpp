#include "monster_bfs.h"

#include <queue>


position monster_bfs::move(const position& p_player) {
    if (p_player == m_monster) {
        return m_monster;
    }

    if (p_player != m_player_position) {
        m_path_to_player = get_path_to_player(p_player);
        m_player_position = p_player;
    }
    else {
        if (!m_path_to_player.empty()) {
            m_path_to_player.pop_front();
        }
    }

    position next_step = m_path_to_player.empty() ? p_player : m_path_to_player.front();
    m_prev_position = m_monster;
    m_monster = next_step;

    return next_step;
}


std::list<position> monster_bfs::get_path_to_player(const position& p_player) {
    position not_visited = { -1, -1 };
    position initial = { 0, 0 };

    std::vector<std::vector<position>> path_back(m_map->size(), std::vector<position>(m_map->front().size(), not_visited));

    std::queue<position> to_process;
    to_process.push({ m_monster.x, m_monster.y });
    path_back[m_monster.y][m_monster.x] = initial;

    bool player_found = false;

    while (!to_process.empty() && !player_found) {
        position cur = to_process.front();
        to_process.pop();

        auto possible_ways = get_possible_steps(cur);
        for (const auto& way : possible_ways) {
            if (path_back[way.y][way.x] != not_visited) {
                continue;
            }

            path_back[way.y][way.x] = cur;

            if (way == p_player) {
                player_found = true;
                break;
            }

            to_process.push(way);
        }
    }

    if (!player_found) {
        throw std::exception("No way to player");
    }

    std::list<position> result = { };
    for (position p = p_player; p != m_monster; p = path_back[p.y][p.x]) {
        result.push_front(p);
    }

    return result;
}