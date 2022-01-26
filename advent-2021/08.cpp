#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>


std::unordered_multimap<std::size_t, std::size_t> DIGIT_LEN_MAP = {
    { 6, 0 },
    { 2, 1 },
    { 5, 2 },
    { 5, 3 },
    { 4, 4 },
    { 5, 5 },
    { 6, 6 },
    { 3, 7 },
    { 7, 8 },
    { 6, 9 }
};


std::unordered_multimap<std::size_t, std::size_t> UNIQUE_DIGIT_LEN_MAP = { 
    { 2, 1 },
    { 4, 4 },
    { 3, 7 },
    { 7, 8 }
};


using digit_sequence = std::vector<std::string>;


struct code {
    digit_sequence input;
    digit_sequence output;
};


using signal_sequence = std::vector<code>;


signal_sequence read_input() {
    signal_sequence result;

    std::ifstream stream("input.txt");

    for (std::string line; std::getline(stream, line);) {
        code current_code;

        std::istringstream sstream(line);
        std::string digit_string;
        while(sstream >> digit_string) {
            if (digit_string == "|") {
                break;
            }

            current_code.input.push_back(digit_string);
        }

        while(sstream >> digit_string) {
            current_code.output.push_back(digit_string);
        }

        result.push_back(current_code);
    }

    return result;
}


class decoder {
private:
    signal_sequence m_sequence;

public:
    decoder(const signal_sequence& p_sequence) : m_sequence(p_sequence) { }

public:
    std::size_t count_output_unique() {
        std::size_t result = 0;
        for (const auto & entry : m_sequence) {
            result += count_output_unique(entry);
        }

        return result;
    }

private:
    std::size_t count_output_unique(const code& p_entry) {
        std::size_t result = 0;

        for (const auto& word : p_entry.output) {
            if (is_unique(word)) {
                result++;
            }
        }

        return result;
    }

    bool is_unique(const std::string& p_word) {
        const auto iter = UNIQUE_DIGIT_LEN_MAP.find(p_word.size());
        return (iter != UNIQUE_DIGIT_LEN_MAP.cend());
    }
};


int main() {
    signal_sequence sequence = read_input();

    std::cout << "The amount of unique output digits: " << decoder(sequence).count_output_unique() << std::endl;

    return 0;
}
