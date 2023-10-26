#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>


struct instruction {
    char direction;
    int steps;
};


struct position {
    int x = 0;
    int y = 0;

    bool operator==(const position& p_other) const {
        return x == p_other.x && y == p_other.y;
    }
};


std::vector<instruction> read_instructions() {
    std::vector<instruction> result;

    std::ifstream stream("input.txt");
    for (std::string line; std::getline(stream, line);) {
        std::stringstream strstream(line);

        instruction instr;
        strstream >> instr.direction;
        strstream >> instr.steps;

        result.push_back(instr);
    }

    return result;
}


#include <unordered_map>
#include <unordered_set>


class rope {
private:
    std::unordered_map<int, std::unordered_set<int>> m_map;
    position m_head = { 0, 0 };

    position m_tail = { 0, 0 };
    std::vector<position> m_tails;

public:
    int count_positions_for_simple_rope(const std::vector<instruction>& p_instr_list) {
        reset();

        for (const instruction& instr : p_instr_list) {
            for (int i = 0; i < instr.steps; i++) {
                handle_step_with_one_knot(instr.direction);
            }
        }

        return count();
    }


    int count_positions_for_complex_rope(const std::vector<instruction>& p_instr_list) {
        reset();

        for (const instruction& instr : p_instr_list) {
            for (int i = 0; i < instr.steps; i++) {
                handle_step_with_multiple_knots(instr.direction);
            }
        }

        return count();
    }


private:
    void reset() {
        m_map.clear();

        m_head = { 0, 0 };
        m_tail = { 0, 0 };

        m_tails = std::vector<position>(9, { 0, 0 });

        m_map[0].insert(0);
    }


    int count() const {
        int result = 0;
        for (const auto& row : m_map) {
            result += row.second.size();
        }

        return result;
    }


    void handle_step_with_one_knot(const char p_direction) {
        bool is_diagonal = is_tail_diagonal(m_head, m_tail);
        move_knot(p_direction, m_head);
        move_tail_knot(m_head, m_tail);
        m_map[m_tail.y].insert(m_tail.x);
    }


    void handle_step_with_multiple_knots(const char p_direction) {
        move_knot(p_direction, m_head);

        position cur_head = m_head;

        for (int i = 0; i < m_tails.size(); i++) {
            move_tail_knot(cur_head, m_tails[i]);
            cur_head = m_tails[i];
        }

        m_map[m_tails.back().y].insert(m_tails.back().x);
    }


    void move_tail_knot(const position& p_head, position& p_tail) {
        if (is_tail_adjacent(p_head, p_tail)) {
            return; /* tail still next to head */
        }

        if (!is_tail_diagonal(p_head, p_tail)) {
            char direction = find_direction(p_head, p_tail);
            move_knot(direction, p_tail); /* if tail was not on diagonal - just move it in the same way */
        }
        else {
            if (p_head.x < p_tail.x) {
                p_tail.x--;
            }
            else {
                p_tail.x++;
            }

            if (p_head.y < p_tail.y) {
                p_tail.y--;
            }
            else {
                p_tail.y++;
            }
        }
    }


    void move_knot(const char p_direction, position& p_knot) {
        switch (p_direction) {
        case 'R':
            p_knot.x++;
            break;

        case 'L':
            p_knot.x--;
            break;

        case 'U':
            p_knot.y++;
            break;

        case 'D':
            p_knot.y--;
            break;
        }
    }

    char find_direction(const position& p_head, const position& p_tail) const {
        if (std::abs(p_head.x - p_tail.x) > 1) {
            if (p_head.x > p_tail.x) {
                return 'R';
            }
            else {
                return 'L';
            }
        }
        else if (std::abs(p_head.y - p_tail.y) > 1) {
            if (p_head.y > p_tail.y) {
                return 'U';
            }
            else {
                return 'D';
            }
        }

        return '0';
    }

    bool is_tail_adjacent(const position& p_head, const position& p_tail) const {
        return (std::abs(p_head.x - p_tail.x) <= 1 && std::abs(p_head.y - p_tail.y) <= 1);
    }

    bool is_tail_diagonal(const position& p_head, const position& p_tail) const {
        return (std::abs(p_head.x - p_tail.x) >= 1 && std::abs(p_head.y - p_tail.y) >= 1);
    }
};


int main() {
    auto instructions = read_instructions();

    std::cout << "The number of visited positions during walking: " << rope().count_positions_for_simple_rope(instructions) << std::endl;
    std::cout << "The number of visited positions during falling: " << rope().count_positions_for_complex_rope(instructions) << std::endl;

    return 0;
}