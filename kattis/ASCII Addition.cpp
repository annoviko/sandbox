#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>


int main() {
    std::unordered_map<std::string, char> patterns = {
        { "xxxxxx...xx...xx...xx...xx...xxxxxx", '0' },
        { "....x....x....x....x....x....x....x", '1' },
        { "xxxxx....x....xxxxxxx....x....xxxxx", '2' },
        { "xxxxx....x....xxxxxx....x....xxxxxx", '3' },
        { "x...xx...xx...xxxxxx....x....x....x", '4' },
        { "xxxxxx....x....xxxxx....x....xxxxxx", '5' },
        { "xxxxxx....x....xxxxxx...xx...xxxxxx", '6' },
        { "xxxxx....x....x....x....x....x....x", '7' },
        { "xxxxxx...xx...xxxxxxx...xx...xxxxxx", '8' },
        { "xxxxxx...xx...xxxxxx....x....xxxxxx", '9' },
        { ".......x....x..xxxxx..x....x.......", '+' }
    };

    std::unordered_map<char, std::string> digit_to_pattern;
    for (const auto& pair : patterns) {
        digit_to_pattern[pair.second] = pair.first;
    }

    const int HEIGHT = 7;
    const int WIDTH = 5;

    std::vector<std::string> content;
    for (int i = 0; i < HEIGHT; i++) {
        std::string line;
        std::getline(std::cin, line);

        content.push_back(line);
    }

    std::vector<std::string> input_patterns;
    for (int col = 0; col < content[0].size(); col += WIDTH + 1) {
        std::string input;
        for (int row = 0; row < HEIGHT; row++) {
            input += content[row].substr(col, WIDTH);
        }

        input_patterns.push_back(std::move(input));
    }

    std::string str_l, str_r;
    bool is_first = true;

    for (const auto & pattern : input_patterns) {
        auto iter = patterns.find(pattern);
        if (iter != patterns.cend()) {
            if (iter->second == '+') {
                is_first = false;
                continue;
            }

            if (is_first) {
                str_l.push_back(iter->second);
            }
            else {
                str_r.push_back(iter->second);
            }
        }
    }

    std::string result = std::to_string(std::stoi(str_l) + std::stoi(str_r));
    std::vector<std::string> output_field(HEIGHT, std::string(result.size() * WIDTH + result.size() - 1, '.'));

    int col = 0;
    for (int i = 0; i < result.size(); i++) {
        std::string pattern = digit_to_pattern[result[i]];

        for (int row = 0; row < HEIGHT; row++) {
            int pos = row * WIDTH;
            
            for (int k = 0; k < WIDTH; k++) {
                output_field[row][k + col] = pattern[pos + k];
            }
        }

        col += WIDTH + 1;
    }

    for (auto& row : output_field) {
        std::cout << row << std::endl;
    }

    return 0;
}