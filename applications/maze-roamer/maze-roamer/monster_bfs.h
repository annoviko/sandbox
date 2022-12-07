#pragma once

#include "monster.h"

#include <list>


class monster_bfs : public monster {
private:
    std::list<position> m_path_to_player; // cache it
    position m_player_position = { -1, -1 };

public:
    monster_bfs(char p_id, const level_map* p_map, position p_position) :
        monster(p_id, p_map, p_position)
    { }

public:
    position move(const position& p_player) override;

private:
    std::list<position> get_path_to_player(const position& p_player);
};
#pragma once
