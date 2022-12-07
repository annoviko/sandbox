#pragma once

#include "monster.h"


class monster_random : public monster {
public:
    monster_random(char id, const level_map* p_map, position p_position) :
        monster(id, p_map, p_position)
    { }

public:
    position move(const position& p_player) override;
};
