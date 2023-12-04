#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>


const std::unordered_map<std::string, int> config = {
    { "red", 12 },
    { "green", 13 },
    { "blue", 14 }
};


struct analysis_result {
    bool is_possible;
    int id;
    int power;
};


analysis_result is_game_possible(const std::string& level, const std::unordered_map<std::string, int>& config) {
    analysis_result result;
    std::stringstream stream(level);

    std::string str_val;
    int int_val;

    stream >> str_val >> result.id >> str_val;

    bool end_of_round = false;
    std::unordered_map<std::string, int> freq;
    std::unordered_map<std::string, int> optimal_config;

    while (stream.good()) {
        stream >> int_val >> str_val;

        end_of_round = (str_val.back() == ';');
        if (str_val.back() == ';' || str_val.back() == ',') {
            str_val.pop_back();
        }

        freq[str_val] = int_val;
        auto iter = config.find(str_val);
        if (freq[str_val] > iter->second) {
            result.is_possible = false;
        }

        optimal_config[str_val] = std::max(optimal_config[str_val], freq[str_val]);

        if (end_of_round) {
            freq.clear();
        }
    }

    result.power = 1;
    for (auto c : optimal_config) {
        result.power *= c.second;
    }

    return result;
}


int count_possible_games() {
    std::ifstream stream("input.txt");

    int sum = 0;
    for (std::string level; std::getline(stream, level);) {
        auto result = is_game_possible(level, config);
        if (result.is_possible) {
            sum += result.id;
        }
    }

    return sum;
}


int count_suitable_power_of_sets() {
    std::ifstream stream("input.txt");

    int sum = 0;
    for (std::string level; std::getline(stream, level);) {
        auto result = is_game_possible(level, config);
        sum += result.power;
    }

    return sum;
}


int main() {
    std::cout << "The sum of the IDs of possible games: " << count_possible_games() << std::endl;
    std::cout << "the sum of the power of the sets: " << count_suitable_power_of_sets() << std::endl;
    return 0;
}