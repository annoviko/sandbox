#include "monster_random.h"


position monster_random::move(const position& p_player) {
    (void)p_player;

    m_prev_position = m_monster;

    auto ways = get_possible_steps(m_monster);
    std::size_t index_way = rand() % ways.size();
    m_monster = ways[index_way];
    return m_monster;
}
