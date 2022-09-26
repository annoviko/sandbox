#include <fstream>
#include <iostream>
#include <regex>
#include <vector>


class game {
private:
    std::size_t m_pos1 = 0;
    std::size_t m_score1 = 0;
    std::size_t m_pos2 = 0;
    std::size_t m_score2 = 0;
    std::size_t m_rolls = 0;
    std::size_t m_dice = 1;

public:
    game(const std::size_t p_pos1, const std::size_t p_pos2) :
        m_pos1(p_pos1), m_pos2(p_pos2)
    { }

public:
    std::size_t play() {
        while(!round()) { }
        return std::min(m_score1, m_score2) * m_rolls;
    }

private:
    bool round() {
        auto steps = roll();
        m_pos1 = move_pawn(m_pos1, steps);
        m_score1 += m_pos1;

        if (m_score1 >= 1000) {
            return true;
        }

        steps = roll();
        m_pos2 = move_pawn(m_pos2, steps);
        m_score2 += m_pos2;

        return m_score2 >= 1000;
    }

    std::size_t move_pawn(std::size_t p_position, std::size_t p_steps) {
        p_position += p_steps;
        p_position = p_position % 10;
        if (p_position == 0) {
            p_position = 10;
        }
        return p_position;
    }

    std::size_t roll() {
        std::size_t steps = 0;
        for (std::size_t i = 0; i < 3; i++) {
            steps += m_dice;
            m_dice++;
            if (m_dice > 100) {
                m_dice = 1;
            }
        }

        m_rolls += 3;

        return steps;
    }
};


std::size_t extract_initial_position(std::ifstream& stream) {
    std::string line;
    std::regex pattern("Player \\d+ starting position: (\\d+)");

    std::getline(stream, line);

    std::smatch matches;
    if (std::regex_search(line, matches, pattern)) {
        return std::stoull(matches[1].str());
    }

    throw std::exception("invalid file format");
}


std::vector<std::size_t> read_positions() {
    std::ifstream stream("input.txt");
    return { extract_initial_position(stream), extract_initial_position(stream) };
}


int main() {
    auto initial_position = read_positions();
    std::cout << "Game score: " << game(initial_position[0], initial_position[1]).play() << std::endl;
    return 0;
}
