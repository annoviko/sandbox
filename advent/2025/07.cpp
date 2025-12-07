#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>


class solution {
private:
    std::vector<std::string> room;

public:
    solution(const std::vector<std::string>& p_room) : room(p_room) { }

public:
    int count_splits() {
        int counter = 0;

        std::unordered_set<int> beams;
        beams.insert(room[0].find('S'));

        for (int i = 1; i < room.size(); i++) {
            std::unordered_set<int> next_beams;

            for (int b : beams) {
                if (room[i][b] == '^') {
                    next_beams.insert(b - 1);
                    next_beams.insert(b + 1);
                    counter++;
                }
                else {
                    next_beams.insert(b);
                }
            }

            beams = std::move(next_beams);
        }

        return counter;
    }

    std::uint64_t count_timelines() {
        std::vector<std::uint64_t> dp(room[0].size(), 0);
        dp[room[0].find('S')] = 1;

        for (int i = 1; i < room.size(); i++) {
            std::vector<std::uint64_t> dp_next(dp.size(), 0);

            for (int k = 0; k < dp.size(); k++) {
                if (room[i][k] == '^') {
                    dp_next[k - 1] += dp[k];
                    dp_next[k + 1] += dp[k];
                }
                else {
                    dp_next[k] += dp[k];
                }
            }

            dp = std::move(dp_next);
        }

        std::uint64_t counter = 0;
        for (std::uint64_t v : dp) {
            counter += v;
        }

        return counter;
    }
};


std::vector<std::string> read_input() {
    std::fstream stream("input.txt");
    std::vector<std::string> content;

    for (std::string line; std::getline(stream, line);) {
        content.push_back(line);
    }

    return content;
}


int main() {
    const auto input = read_input();

    std::cout << "Number of times the beam will be split: " << solution(input).count_splits() << std::endl;
    std::cout << "Number of timelines: " << solution(input).count_timelines() << std::endl;

    return 0;
}
