#include "monster.h"


monster::monster(char p_id, const level_map* p_map, position p_position) :
    m_id(p_id),
    m_map(p_map),
    m_monster(p_position),
    m_prev_position(p_position)
{ }


char monster::get_id() const {
    return m_id;
}


position monster::get_previous_position() const {
    return m_prev_position;
}


position monster::get_current_position() const {
    return m_monster;
}


std::vector<position> monster::get_possible_steps(const position& p_pos) const {
    std::vector<position> positions = { };
    if ((*m_map)[p_pos.y][p_pos.x + 1] != '*') {
        positions.push_back({ p_pos.x + 1, p_pos.y });
    }

    if ((*m_map)[p_pos.y][p_pos.x - 1] != '*') {
        positions.push_back({ p_pos.x - 1, p_pos.y });
    }

    if ((*m_map)[p_pos.y + 1][p_pos.x] != '*') {
        positions.push_back({ p_pos.x, p_pos.y + 1 });
    }

    if ((*m_map)[p_pos.y - 1][p_pos.x] != '*') {
        positions.push_back({ p_pos.x, p_pos.y - 1 });
    }

    return positions;
}