#include <fstream>
#include <iostream>
#include <list>
#include <regex>
#include <string>
#include <vector>
#include <unordered_map>


using formula = std::string;
using insertion_rules = std::unordered_map<std::string, char>;


class polymer_builder_optimized {
private:
    std::unordered_map<std::string, std::uint64_t> m_polymer;
    std::unordered_map<char, std::uint64_t> m_elem_stats;
    insertion_rules m_rules;

public:
    polymer_builder_optimized(const formula& p_formula, const insertion_rules& p_rules) :
        m_rules(p_rules)
    {
        std::string pair = p_formula.substr(0, 2);
        m_polymer[pair]++;

        m_elem_stats[pair[0]]++;
        m_elem_stats[pair[1]]++;

        for (std::size_t i = 2; i < p_formula.size(); i++) {
            pair[0] = pair[1];
            pair[1] = p_formula[i];

            m_elem_stats[p_formula[i]]++;
            m_polymer[pair]++;
        }
    }


private:
    void apply_rule() {
        std::unordered_map<std::string, std::uint64_t> new_polymer = m_polymer;

        for (const auto& rule : m_rules) {
            auto iter = m_polymer.find(rule.first);
            if (iter == m_polymer.end()) {
                continue;
            }

            if (iter->second == 0) {
                continue;   // pair does not exist anymore, nothing to split
            }

            // split pair
            const std::string pair_to_split = iter->first;
            std::string left_pair = pair_to_split, right_pair = pair_to_split;

            left_pair[1] = rule.second;
            right_pair[0] = rule.second;

            const std::uint64_t amount_pairs_to_split = iter->second;
            new_polymer[pair_to_split] -= amount_pairs_to_split;
            new_polymer[left_pair] += amount_pairs_to_split;
            new_polymer[right_pair] += amount_pairs_to_split;

            m_elem_stats[rule.second] += amount_pairs_to_split;
        }

        m_polymer = std::move(new_polymer);
    }


    void get_min_max(std::uint64_t& p_min, std::uint64_t& p_max) {
        std::uint64_t max = 0, min = std::numeric_limits<std::uint64_t>::max();
        for (const auto& element_stat : m_elem_stats) {
            if (element_stat.second > max) {
                max = element_stat.second;
            }

            if (element_stat.second < min) {
                min = element_stat.second;
            }
        }

        p_min = min;
        p_max = max;
    }


public:
    std::uint64_t apply_rules(const std::uint64_t p_times) {
        for (std::uint64_t i = 0; i < p_times; i++) {
            apply_rule();
        }

        std::uint64_t min = 0, max = 0;
        get_min_max(min, max);

        return max - min;
    }
};


void read_input(formula& p_formula, insertion_rules& p_rules) {
    std::ifstream stream("input.txt");
    std::string str_row;

    std::getline(stream, p_formula);
    std::getline(stream, str_row); // skip

    while (std::getline(stream, str_row)) {
        std::regex pattern("(.*) -> (.)");
        std::smatch result;

        if (!std::regex_search(str_row, result, pattern)) {
            throw std::invalid_argument("incorrect file format");
        }

        p_rules.insert({ result[1].str(), result[2].str()[0] });
    }
}


int main() {
    formula input_formula;
    insertion_rules input_rules;
    read_input(input_formula, input_rules);

    std::cout << "The difference between min and max the element quantity after 10 steps: " << polymer_builder_optimized(input_formula, input_rules).apply_rules(10) << std::endl;
    std::cout << "The difference between min and max the element quantity after 40 steps: " << polymer_builder_optimized(input_formula, input_rules).apply_rules(40) << std::endl;

    return 0;
}
