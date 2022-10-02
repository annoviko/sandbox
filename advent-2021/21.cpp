#include <fstream>
#include <iostream>
#include <regex>
#include <vector>
#include <unordered_map>


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


struct player_wins {
    std::uint64_t win1 = 0;
    std::uint64_t win2 = 0;

    bool is_not_empty() {
        return win1 != 0 && win2 != 0;
    }

    player_wins& operator+=(const player_wins& p_other) {
        win1 += p_other.win1;
        win2 += p_other.win2;
        return *this;
    }
};


class dirac_dice {
private:
    std::size_t m_init_pos1 = 0;
    std::size_t m_init_pos2 = 0;

    /*                 pos1                              pos2                              score1                            score2                            turn */
    std::unordered_map<std::uint64_t, std::unordered_map<std::uint64_t, std::unordered_map<std::uint64_t, std::unordered_map<std::uint64_t, std::unordered_map<std::uint64_t, player_wins>>>>> m_dp;


public:
    dirac_dice(const std::size_t p_pos1, const std::size_t p_pos2) :
        m_init_pos1(p_pos1), m_init_pos2(p_pos2)
    { }

public:
    std::size_t play() {
        auto win_stat = player_turn(m_init_pos1, m_init_pos2, 0, 0, 1);
        return std::max(win_stat.win1, win_stat.win2);
    }

private:
    player_wins player_turn(const std::size_t p_pos1, const std::size_t p_pos2, const std::size_t p_score1, const std::size_t p_score2, const std::size_t p_player) {
        if (m_dp[p_pos1][p_pos2][p_score1][p_score2][p_player].is_not_empty()) {
            return m_dp[p_pos1][p_pos2][p_score1][p_score2][p_player];
        }

        if (p_score1 >= 21) { 
            return { 1, 0 }; 
        }

        if (p_score2 >= 21) {
            return { 0, 1 };
        }

        player_wins win_stat;

        for (std::size_t d1 = 1; d1 <= 3; d1++)
        for (std::size_t d2 = 1; d2 <= 3; d2++)
        for (std::size_t d3 = 1; d3 <= 3; d3++) {
            std::size_t player_pos = p_player == 1 ? p_pos1 : p_pos2;

            std::size_t step = d1 + d2 + d3;
            std::size_t next_pos = get_pos(step, player_pos);

            if (p_player == 1) {
                std::size_t next_pos1 = next_pos;
                std::size_t next_score1 = p_score1 + next_pos;
                win_stat += player_turn(next_pos1, p_pos2, next_score1, p_score2, 2);
            }
            else {
                std::size_t next_pos2 = next_pos;
                std::size_t next_score2 = p_score2 + next_pos;
                win_stat += player_turn(p_pos1, next_pos2, p_score1, next_score2, 1);
            }
        }

        m_dp[p_pos1][p_pos2][p_score1][p_score2][p_player] = win_stat;
        return win_stat;
    }

    std::size_t get_pos(const std::size_t p_step, const std::size_t p_pos) {
        std::size_t pos = (p_pos + p_step) % 10;
        if (pos == 0) {
            pos = 10;
        }
        return pos;
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
    std::cout << "Trial game. Loser score: " << game(initial_position[0], initial_position[1]).play() << std::endl;
    std::cout << "Dirac game. Number of wins of the winner: " << dirac_dice(initial_position[0], initial_position[1]).play() << std::endl;
    return 0;
}
