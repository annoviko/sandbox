#include <algorithm>
#include <cstdint>
#include <fstream>
#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <numeric>
#include <vector>
#include <string>
#include <sstream>
#include <bitset>


using maze_map = std::vector<std::string>;


maze_map read_map() {
    maze_map map;

    std::ifstream stream("input.txt");
    std::string line;
    while (std::getline(stream, line)) {
        map.push_back(std::move(line));
    }

    stream.close();
    return map;
}


struct position {
    int x = 0;
    int y = 0;
};


struct branch {
    position m_robot;
    std::set<char> m_keys;
    std::set<char> m_remaining;
    std::size_t m_steps;
    std::vector<std::vector<bool>> m_visited;
    std::bitset<26> m_pattern;
};


class explorer {
private:
    maze_map m_map;

    position m_initial;
    std::set<char> m_keys;
    std::set<char> m_remaining;
    std::vector<std::vector<std::set<std::uint64_t>>> m_repeat;


public:
    explorer(const maze_map& p_map) : m_map(p_map) { }

public:
    std::size_t explore() {
        find_objects();
        m_repeat = std::vector<std::vector<std::set<std::uint64_t>>>(m_map.size(), std::vector<std::set<std::uint64_t>>(m_map[0].size()));
        return explore_area({ m_initial, m_keys, m_remaining, 0, build_visit_map() });
    }

    std::size_t explore_by_four() {
        find_objects();

        m_map[m_initial.y][m_initial.x] = '#';
        m_map[m_initial.y + 1][m_initial.x] = '#';
        m_map[m_initial.y - 1][m_initial.x] = '#';
        m_map[m_initial.y][m_initial.x + 1] = '#';
        m_map[m_initial.y][m_initial.x - 1] = '#';

        std::vector<position> positions = {
            { m_initial.x - 1, m_initial.y - 1 },
            { m_initial.x - 1, m_initial.y + 1 },
            { m_initial.x + 1, m_initial.y - 1 },
            { m_initial.x + 1, m_initial.y + 1 }
        };

        std::vector<branch> branches;
        for (std::size_t i = 0; i < positions.size(); i++) {
            branches.push_back(create_branch(positions[i]));
        }

        std::size_t result = 0;
        for (const auto& single_branch : branches) {
            m_repeat = std::vector<std::vector<std::set<std::uint64_t>>>(m_map.size(), std::vector<std::set<std::uint64_t>>(m_map[0].size()));
            result += explore_area(single_branch);
        }

        return result;
    }

private:
    std::vector<std::vector<bool>> build_visit_map() {
        return std::vector<std::vector<bool>>(m_map.size(), std::vector<bool>(m_map[0].size(), false));
    }

    branch create_branch(const position& p_coordinate) {
        std::set<char> keys;
        std::set<char> remaining;
        std::bitset<26> pattern;

        auto visit_map = build_visit_map();

        std::list<position> to_traverse;
        to_traverse.push_back(p_coordinate);

        while (!to_traverse.empty()) {
            position cur = to_traverse.front();
            to_traverse.pop_front();

            const char object = m_map[cur.y][cur.x];
            if (std::isalpha(object) && std::islower(object)) {
                remaining.insert(object);       /* the key is reachable */
            }

            std::vector<position> candidates = { 
                { cur.x - 1, cur.y },
                { cur.x + 1, cur.y },
                { cur.x, cur.y - 1 },
                { cur.x, cur.y + 1 }
            };

            for (const auto pos : candidates) {
                if (pos.x < 0 || pos.x >= m_map[0].size()) {
                    continue;
                }

                if (pos.y < 0 || pos.y >= m_map.size()) {
                    continue;
                }

                if (visit_map[pos.y][pos.x]) {
                    continue;
                }

                if (m_map[pos.y][pos.x] == '#') {
                    continue;
                }

                visit_map[pos.y][pos.x] = true;
                to_traverse.push_back(pos);
            }
        }

        /* add keys from other locations */
        for (const auto key : m_remaining) {
            if (remaining.find(key) == remaining.end()) {
                /* the key does not belong to the current location */
                keys.insert(key);
                pattern.set(key - 'a', true);
            }
        }

        return { p_coordinate, keys, remaining, 0, build_visit_map(), pattern };
    }

    std::size_t explore_area(const branch& p_initial) {
        std::list<branch> branches;
        branches.push_back(p_initial);

        while (!branches.empty()) {
            auto cur = branches.front();
            branches.pop_front();

            /* if there was path with the same route */
            auto & pattern_point = m_repeat[cur.m_robot.y][cur.m_robot.x];
            auto pattern_to_search = cur.m_pattern.to_ullong();
            if (pattern_point.find(pattern_to_search) != pattern_point.end()) {
                continue;
            }

            pattern_point.insert(pattern_to_search);

            if (is_found_key(cur)) {
                const char key = m_map[cur.m_robot.y][cur.m_robot.x];
                cur.m_keys.insert(key);
                cur.m_remaining.erase(key);

                if (cur.m_remaining.empty()) {
                    return cur.m_steps;
                }

                cur.m_visited = std::move(build_visit_map());
                cur.m_visited[cur.m_robot.y][cur.m_robot.x] = true;
                cur.m_pattern.set(key - 'a', true);

                branches.push_back(std::move(cur));

                continue;
            }

            auto ways = get_positions(cur);
            for (const auto& way : ways) {
                branches.push_back({ way, cur.m_keys, cur.m_remaining, cur.m_steps + 1, cur.m_visited, cur.m_pattern });
                branches.back().m_visited[way.y][way.x] = true;
            }
        }

        throw std::exception();
    }

    std::vector<position> get_positions(const branch & context) {
        std::vector<position> res;

        const position & p_pos = context.m_robot;

        if (is_valid_step({ p_pos.x - 1, p_pos.y }, context)) {
            res.push_back({ p_pos.x - 1, p_pos.y });
        }

        if (is_valid_step({ p_pos.x + 1, p_pos.y }, context)) {
            res.push_back({ p_pos.x + 1, p_pos.y });
        }

        if (is_valid_step({ p_pos.x, p_pos.y - 1 }, context)) {
            res.push_back({ p_pos.x, p_pos.y - 1 });
        }

        if (is_valid_step({ p_pos.x, p_pos.y + 1 }, context)) {
            res.push_back({ p_pos.x, p_pos.y + 1 });
        }

        return res;
    }

    bool is_found_key(const branch& context) {
        const position & p_pos = context.m_robot;
        const char object = m_map[p_pos.y][p_pos.x];

        return std::isalpha(object) && std::islower(object) && (context.m_keys.find(object) == context.m_keys.end());
    }

    bool is_valid_step(const position& p_pos, const branch& context) {
        if (p_pos.x < 0 || p_pos.x >= m_map[0].size()) {
            return false;   /* out of range */
        }

        if (p_pos.y < 0 || p_pos.y >= m_map.size()) {
            return false;   /* out of range */
        }

        const char object = m_map[p_pos.y][p_pos.x];

        if (object == '#') {
            return false;   /* can't go though the wall */
        }

        if (context.m_visited[p_pos.y][p_pos.x]) {
            return false;   /* already visited */
        }

        if (object == '.') {
            return true;
        }

        /* check if it is a door */
        if (std::isupper(object)) {
            /* check that we have a key */
            if (context.m_keys.find(std::tolower(object)) == context.m_keys.end()) {
                return false;   /* no key - no way */
            }

            /* can go through */
        }

        return true;
    }

    void find_objects() {
        for (std::size_t i = 0; i < m_map.size(); i++) {
            for (std::size_t j = 0; j < m_map.at(i).size(); j++) {
                const char sym = m_map[i][j];

                switch (sym) {
                case '@':
                    m_initial.x = (int) j;
                    m_initial.y = (int) i;
                    m_map[i][j] = '.';
                    break;

                case '#':
                case '.':
                    break;

                default:
                    if (std::islower(sym)) {
                        m_remaining.insert(sym);
                    }
                    break;
                }
            }
        }
    }
};


int main() {
    auto map = read_map();

    explorer robot1(map);
    std::cout << robot1.explore() << std::endl;

    explorer robot2(map);
    std::cout << robot2.explore_by_four() << std::endl;

    return 0;
}
