#include <algorithm>
#include <fstream>
#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>


const std::unordered_map<char, int> score_error_table = {
    { ')', 3 },
    { ']', 57 },
    { '}', 1197 },
    { '>', 25137 }
};


const std::unordered_map<char, int> score_completion_table = {
    { ')', 1 },
    { ']', 2 },
    { '}', 3 },
    { '>', 4 }
};


const std::unordered_map<char, char> close_to_open_bracket = {
    { ')', '(' },
    { ']', '[' },
    { '}', '{' },
    { '>', '<' }
};


const std::unordered_map<char, char> open_to_close_bracket = {
    { '(', ')' },
    { '[', ']' },
    { '{', '}' },
    { '<', '>' }
};


class bracket_stack {
private:
    std::stack<char> m_stack;

public:
    bracket_stack() = default;

public:
    bool is_empty() {
        return m_stack.empty();
    }


    char push(const char p_value) {
        if (m_stack.empty()) {
            m_stack.push(p_value);
            return 0;
        }

        const auto iter = close_to_open_bracket.find(p_value);
        if (iter == close_to_open_bracket.cend()) {
            m_stack.push(p_value);
            return 0;
        }

        const char expected_open_bracker = iter->second;
        if (m_stack.top() != expected_open_bracker) {
            return p_value;
        }

        m_stack.pop();
        return 0;
    }


    std::string get_completion() {
        std::string result;
        while (!m_stack.empty()) {
            const char close_bracket = open_to_close_bracket.find(m_stack.top())->second;
            result.push_back(close_bracket);
            m_stack.pop();
        }

        return result;
    }
};


using input_sequence = std::vector<std::string>;


input_sequence read_input() {
    std::ifstream stream("input.txt");

    input_sequence result;
    for (std::string line; std::getline(stream, line);) {
        result.emplace_back(std::move(line));
    }

    return result;
}


class systax_processor {
private:
    input_sequence m_sequence;

    std::unordered_map<char, int> m_error_stats;
    std::vector<std::string> m_completion;

    int m_syntax_error_score = 0;
    std::uint64_t m_completion_score = 0;

public:
    systax_processor(const input_sequence& p_sequence) :
        m_sequence(p_sequence)
    { }

private:
    void evaluate_chunk(const std::string& p_chunk) {
        bracket_stack stack;
        for (char p_value : p_chunk) {
            if (stack.push(p_value) == 0) {
                continue;
            }

            m_error_stats[p_value]++;
            return;
        }

        std::string completion = stack.get_completion();
        if (!completion.empty()) {
            m_completion.push_back(completion);
        }
    }


    void evaluate_sequence() {
        for (const auto& chunk : m_sequence) {
            evaluate_chunk(chunk);
        }
    }


    void calculate_total_error() {
        for (const auto& pair : close_to_open_bracket) {
            const char close_bracker = pair.first;
            m_syntax_error_score += m_error_stats[close_bracker] * score_error_table.at(close_bracker);
        }
    }


    std::uint64_t calculate_completion_score(const std::string& p_completion) {
        std::uint64_t result = 0;
        for (const char p_value : p_completion) {
            result = result * std::uint64_t(5) + static_cast<std::uint64_t>(score_completion_table.at(p_value));
        }
        return result;
    }


    void calculate_completion_middle_score() {
        std::vector<std::uint64_t> scores;
        for (const auto& completion : m_completion) {
            const std::uint64_t score = calculate_completion_score(completion);
            scores.push_back(score);
        }

        std::sort(scores.begin(), scores.end());
        std::size_t middle_index = scores.size() >> 1;
        m_completion_score = scores[middle_index];
    }

public:
   systax_processor & handle() {
        evaluate_sequence();

        calculate_total_error();
        calculate_completion_middle_score();

        return *this;
    }


    int get_syntax_error_score() {
        return m_syntax_error_score;
    }


    std::uint64_t get_completion_middle_score() {
        return m_completion_score;
    }
};






int main() {
    const auto sequence = read_input();
    systax_processor proc = systax_processor(sequence).handle();

    std::cout << "The total syntax error score: " << proc.get_syntax_error_score() << std::endl;
    std::cout << "The middle completion score: " << proc.get_completion_middle_score() << std::endl;

    return 0;
}
