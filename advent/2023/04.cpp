#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <unordered_set>


struct card {
    std::vector<int> win;
    std::vector<int> actual;
};


card extract_card(const std::string& line) {
    card result;

    auto pos = line.find(":") + 1;
    std::string value;

    bool is_left_part = true;

    for (; pos < line.size(); pos++) {
        if (std::isdigit(line[pos])) {
            value.push_back(line[pos]);
            continue;
        }

        if (!value.empty()) {
            if (is_left_part) {
                result.win.push_back(std::stoi(value));
            }
            else {
                result.actual.push_back(std::stoi(value));
            }

            value.clear();
        }

        if (line[pos] == '|') {
            is_left_part = false;
        }
    }

    if (!value.empty()) {
        result.actual.push_back(std::stoi(value));
    }

    return result;
}


std::vector<card> read_cards() {
    std::vector<card> result;

    std::fstream stream("input.txt");
    for (std::string line; std::getline(stream, line);) {
        result.push_back(extract_card(line));
    }

    return result;
}


class solution {
    std::vector<card> m_cards;

public:
    solution(const std::vector<card>& p_cards) : m_cards(p_cards) { }

public:
    int count_total_points() const {
        int sum = 0;
        for (const auto& card : m_cards) {
            sum += get_win_score(card);
        }

        return sum;
    }


    int count_scratchcards() const {
        std::vector<int> stats(m_cards.size(), 1);

        for (int i = 0; i < stats.size(); i++) {
            int current_cards_number = stats[i];
            int win_numbers = count_win_numbers(m_cards[i]);
            for (int j = 0; j < win_numbers; j++) {
                stats[i + j + 1] += current_cards_number;
            }
        }

        int sum = 0;
        for (auto score : stats) {
            sum += score;
        }

        return sum;
    }

private:
    int count_win_numbers(const card& p_card) const {
        std::unordered_set<int> win_cards;
        for (auto win_card : p_card.win) {
            win_cards.insert(win_card);
        }

        int numbers = 0;
        for (auto actual : p_card.actual) {
            auto iter = win_cards.find(actual);

            if (iter != win_cards.end()) {
                numbers++;
            }
        }

        return numbers;
    }


    int get_win_score(const card& p_card) const {
        int win_numbers = count_win_numbers(p_card);
        return win_numbers == 0 ? 0 : std::pow(2, win_numbers - 1);
    }
};


int main() {
    auto input = read_cards();

    std::cout << "Points in total: " << solution(input).count_total_points() << std::endl;
    std::cout << "Total scratchcards: " << solution(input).count_scratchcards() << std::endl;

    return 0;
}
