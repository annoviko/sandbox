#include <array>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>
#include <unordered_map>


struct bingo_node {
    std::size_t row = 0;
    std::size_t col = 0;
};


constexpr std::size_t bingo_side_length = 5;

using bingo_field = std::unordered_map<std::size_t, bingo_node>;
using bingo_stats = std::array<std::size_t, bingo_side_length>;


enum class bingo_strategy {
    win_first = 0,
    win_last
};


class bingo_board {
public:
    using ptr = std::shared_ptr<bingo_board>;

private:
    bingo_field m_numbers;
    bingo_stats m_stats_row = { 0 };
    bingo_stats m_stats_col = { 0 };
    std::size_t m_sum = 0;
    bool m_won = false;

public:
    static bingo_board parse(const std::vector<std::string>& p_board) {
        bingo_board result;
        std::size_t row = 0;

        for (const std::string & line : p_board) {
            std::stringstream stream;
            stream << line;

            for (std::size_t i = 0, col = 0; i < bingo_side_length; i++, col++) {
                std::size_t value = 0;
                stream >> value;

                result.m_sum += value;
                result.m_numbers.insert({ value, { row, col } });
            }

            row++;
        }

        return result;
    }

public:
    bool mark(const std::size_t p_value) {
        const auto iter_number = m_numbers.find(p_value);
        if (iter_number == m_numbers.end()) {
            return false;
        }

        m_sum -= p_value;
        const std::size_t score_row = ++m_stats_row[iter_number->second.row];
        const std::size_t score_col = ++m_stats_col[iter_number->second.col];

        m_numbers.erase(iter_number);

        m_won = ((score_row >= 5) || (score_col >= 5));
        return m_won;
    }

    bool is_won() const {
        return m_won;
    }

    std::size_t get_sum() const {
        return m_sum;
    }
};


class bingo_database {
private:
    std::vector<bingo_board> m_database;

public:
    void insert(bingo_board&& p_board) {
        m_database.push_back(std::move(p_board));
    }

    std::size_t mark(const std::size_t p_value, bingo_strategy p_strategy) {
        std::size_t score = 0;
        for (bingo_board & board : m_database) {
            if (board.is_won() && (p_strategy == bingo_strategy::win_last)) {
                continue;
            }

            if (board.mark(p_value)) {
                score = p_value * board.get_sum();
                if (p_strategy == bingo_strategy::win_first) {
                    return score;
                }
            }
        }

        return score;
    }
};


class bingo_game {
private:
    std::vector<std::size_t> m_values;
    bingo_database m_database;

public:
    static bingo_game read() {
        bingo_game game;

        std::ifstream stream("input.txt");

        std::string sequence;
        std::getline(stream, sequence);

        std::stringstream string_stream;
        string_stream << sequence;

        for (std::string string_value; std::getline(string_stream, string_value, ',');) {
            game.m_values.push_back(std::stoul(string_value));
        }

        std::vector<std::string> string_buffer;
        for (std::string line; std::getline(stream, line);) {
            if (line.empty()) {
                if (!string_buffer.empty()){
                    game.m_database.insert(bingo_board::parse(string_buffer));
                    string_buffer.clear();
                }
            }
            else {
                string_buffer.push_back(line);
            }
        }

        if (!string_buffer.empty()) {
            game.m_database.insert(bingo_board::parse(string_buffer));
        }

        return game;
    }

public:
    std::size_t run(const bingo_strategy p_strategy) {
        std::size_t result = 0;
        for (const auto value : m_values) {
            const std::size_t iteration_result = m_database.mark(value, p_strategy);
            if (iteration_result != 0) {
                result = iteration_result;
                if (p_strategy == bingo_strategy::win_first) {
                    return result;
                }
            }
        }

        if (result != 0) {
            return result;
        }

        throw std::exception("winner board does not exist");
    }
};


int main() {
    bingo_game game = bingo_game::read();

    std::cout << "Sum of all unmarked numbers (win first strategy): " << game.run(bingo_strategy::win_first) << std::endl;
    std::cout << "Sum of all unmarked numbers (win last strategy): " << game.run(bingo_strategy::win_last) << std::endl;

    return 0;
}