#pragma once


#include <vector>
#include <string>

#include "level_map.h"
#include "position.h"


class monster {
protected:
    const level_map* m_map;

    position m_monster;
    position m_prev_position = { -1, -1 };

    char m_id;

public:
    monster(char p_id, const level_map* p_map, position p_position);

public:
    virtual char get_id() const;

    virtual position get_previous_position() const;

    virtual position get_current_position() const;

    virtual std::vector<position> get_possible_steps(const position& p_pos) const;

    virtual position move(const position& p_player) = 0;
};