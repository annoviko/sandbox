#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>


struct statistics {
    std::uint64_t correct = 0;
    std::uint64_t incorrect = 0;

    std::uint64_t total() const {
        return correct + incorrect;
    }
};


class solution {
public:
    statistics solve(const std::vector<std::string>& sentence, const std::unordered_map<std::string, statistics>& stats) {
        std::uint64_t correct_total = 1;
        std::uint64_t total = 1;

        for (const auto& word : sentence) {
            const auto iter = stats.find(word);
            if (iter == stats.end()) {
                return { 0, 0 };
            }

            total *= iter->second.total();
            correct_total *= iter->second.correct;
        }

        std::uint64_t incorrect_total = total - correct_total;
        return { correct_total, incorrect_total };
    }
};


int main() {
    std::vector<std::string> sentence;
    std::unordered_map<std::string, statistics> stats;
    std::unordered_map<std::string, std::string> dict;

    int n = 0;
    std::cin >> n;

    for (int i = 0; i < n; i++) {
        std::string word;
        std::cin >> word;
        sentence.push_back(word);
    }

    std::cin >> n;
    for (int i = 0; i < n; i++) {
        std::string dutch, english, type;
        std::cin >> dutch >> english >> type;

        dict[dutch] = english;

        if (type == "correct") {
            stats[dutch].correct++;
        }
        else {
            stats[dutch].incorrect++;
        }
    }

    auto result = solution().solve(sentence, stats);
    if (result.total() == 1) {
        bool is_first = true;

        for (auto& w : sentence) {
            if (!is_first) {
                std::cout << ' ';
            }

            std::cout << dict[w];
            is_first = false;
        }

        std::cout << std::endl;
        std::cout << (result.correct > 0 ? "correct" : "incorrect") << std::endl;
    }
    else {
        std::cout << result.correct << " correct" << std::endl;
        std::cout << result.incorrect << " incorrect" << std::endl;
    }

    return 0;
}
