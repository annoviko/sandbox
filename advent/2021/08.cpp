#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>

#include "08.h"


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


class code_decoder {
private:
    code m_entry;

    std::unordered_set<std::string> m_patterns = { std::string(7, ' ') };
    std::unordered_set<std::size_t> m_positions;
    std::unordered_set<char> m_letters;

    std::vector<bool> m_handled_sizes = std::vector<bool>(8, false);

public:
    code_decoder(const code& p_entry) : m_entry(p_entry) { }

public:
    std::size_t decode() {
        const auto unique_words = get_unique_words_ascended();
        for (const auto& word : unique_words) {
            handle_unique_word(word);
        }

        match();
        return decode_output();
    }

private:
    std::size_t decode_output() {
        std::string result;

        auto position_map = get_letter_position_map(*m_patterns.begin());
        for (const std::string & word : m_entry.output) {
            std::unordered_set<std::size_t> actual_positions;
            for (const char letter : word) {
                const std::size_t position = position_map[letter];
                actual_positions.insert(position);
            }

            const auto digits = get_digits(word);
            for (const auto digit : digits) {
                const auto positions = get_digit_positions(digit);
                if (actual_positions == positions) {
                    result.push_back(std::to_string(digit)[0]);
                    break;
                }
            }
        }

        return std::stoull(result);
    }

    std::unordered_map<char, std::size_t> get_letter_position_map(const std::string& p_pattern) {
        std::unordered_map<char, std::size_t> result;
        for (std::size_t i = 0; i < p_pattern.size(); i++) {
            result[p_pattern[i]] = i;
        }

        return result;
    }

    std::vector<std::string> get_unique_words_ascended() {
        std::vector<std::string> result;
        std::vector<std::size_t> handled_length(8, false);

        for (const auto & word : m_entry.input) {
            if (is_unique(word) && !handled_length[word.size()]) {
                result.push_back(word);
                handled_length[word.size()] = true;
            }
        }

        std::sort(result.begin(), result.end(), [](const std::string & left, const std::string & right) {
            return left.size() < right.size();
        });
        return result;
    }

    bool is_unique(const std::string& p_word) {
        const auto iter = UNIQUE_DIGIT_LEN_MAP.find(p_word.size());
        return (iter != UNIQUE_DIGIT_LEN_MAP.cend());
    }

    void handle_unique_word(const std::string& p_word) {
        if (m_handled_sizes[p_word.size()]) {
            return;
        }

        std::unordered_set<char> word_letters;
        for (const auto value : p_word) { word_letters.insert(value); }
        word_letters = subtract(word_letters, m_letters);

        std::unordered_set<std::size_t> word_positions = get_unique_word_positions(p_word);
        word_positions = subtract(word_positions, m_positions);

        std::string sorted_letters;
        for (const auto letter : word_letters) { sorted_letters.push_back(letter); }
        std::sort(sorted_letters.begin(), sorted_letters.end());

        std::unordered_set<std::string> next_patterns = { };
        do {
            for (const auto & pattern : m_patterns) {
                std::string next_pattern = pattern;
                std::size_t index_letter = 0;
                for (const std::size_t position : word_positions) {
                    next_pattern[position] = sorted_letters[index_letter];
                    index_letter++;
                }

                next_patterns.insert(next_pattern);
            }
        }
        while (std::next_permutation(sorted_letters.begin(), sorted_letters.end()));

        m_patterns = std::move(next_patterns);
        m_positions = merge(word_positions, m_positions);
        m_letters = merge(word_letters, m_letters);
        m_handled_sizes[p_word.size()] = true;
    }

    std::unordered_set<std::size_t> get_unique_word_positions(const std::string& p_word) {
        switch(p_word.size()) {
            case 2: return { 1, 2 };
            case 3: return { 0, 1, 2 };
            case 4: return { 6, 5, 1, 2 };
            case 7: return { 0, 1, 2, 3, 4, 5, 6 };
        }

        throw std::exception("non-unique word is provided");
    }

    void match() {
        std::vector<std::string> to_analyse = m_entry.input;
        to_analyse.insert(to_analyse.end(), m_entry.output.begin(), m_entry.output.end());

        while(m_patterns.size() != 1) {
            for (const auto& word : to_analyse) {
                if (is_unique(word)) {
                    continue; /* all suitable for unique */
                }

                match_word_with_patterns(word);
                if (m_patterns.size() == 1) {
                    break;
                }

                if (m_patterns.empty()) {
                    throw std::exception("no patterns are suitable");
                }
            }
        }
    }

    void match_word_with_patterns(const std::string& p_word) {
        const auto digits = get_digits(p_word);

        std::vector<std::unordered_set<std::string>::const_iterator> to_remove;
        for (std::unordered_set<std::string>::const_iterator iter = m_patterns.begin(); iter != m_patterns.end(); iter++) {
            bool is_suitable = false;
            for (const std::size_t digit : digits) {
                const auto positions = get_digit_positions(digit);
                if (is_suitable_pattern(p_word, *iter, positions)) {
                    is_suitable = true;
                    break;
                }
            }

            if (!is_suitable) {
                to_remove.push_back(iter);
            }
        }

        m_handled_sizes[p_word.size()] = true;
        remove_patterns(to_remove);
    }

    void remove_patterns(const std::vector<std::unordered_set<std::string>::const_iterator>& p_iters) {
        for (const auto & iter : p_iters) {
            m_patterns.erase(iter);
        }
    }

    bool is_suitable_pattern(const std::string& p_word, const std::string& p_pattern, const std::unordered_set<std::size_t>& p_positions) {
        std::string actual_letters = p_word;
        std::sort(actual_letters.begin(), actual_letters.end());

        std::string expected_letters(p_word.size(), ' ');
        std::size_t index = 0;
        for (const std::size_t position : p_positions) {
            expected_letters[index] = p_pattern[position];
            index++;
        }

        std::sort(expected_letters.begin(), expected_letters.end());
        return expected_letters == actual_letters;
    }

    std::unordered_set<std::size_t> get_digit_positions(const std::size_t p_digit) {
        switch(p_digit) {
            case 0: return { 0, 1, 2, 3, 4, 6 };
            case 1: return { 1, 2 };
            case 2: return { 0, 1, 5, 4, 3 };
            case 3: return { 0, 1, 5, 2, 3 };
            case 4: return { 6, 5, 1, 2 };
            case 5: return { 0, 6, 5, 2, 3 };
            case 6: return { 0, 6, 5, 2, 3, 4 };
            case 7: return { 0, 1, 2 };
            case 8: return { 0, 1, 2, 3, 4, 5, 6 };
            case 9: return { 0, 1, 5, 6, 2, 3 };
        }

        throw std::exception("digit is out of range");
    }

    std::unordered_set<std::size_t> get_digits(const std::string& p_word) {
        switch (p_word.size()) {
            case 2: return { 1 };
            case 3: return { 7 };
            case 4: return { 4 };
            case 5: return { 2, 3, 5 };
            case 6: return { 0, 6, 9 };
            case 7: return { 8 };
        }

        throw std::exception("invalid digit size");
    }
};


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

    std::size_t count_output() {
        std::size_t result = 0;
        for (const auto & entry : m_sequence) {
            result += code_decoder(entry).decode();
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
    std::cout << "The sum of outputs: " << decoder(sequence).count_output() << std::endl;

    return 0;
}
