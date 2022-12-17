#include <fstream>
#include <set>
#include <string>
#include <vector>
#include <unordered_set>


std::vector<std::string> read_rucksacks() {
    std::vector<std::string> content;

    std::ifstream stream("input.txt");
    for (std::string line; std::getline(stream, line); ) {
        content.push_back(line);
    }

    return content;
}


class rucksacks_analyser {
private:
    std::vector<std::string> m_content;

public:
    rucksacks_analyser(const std::vector<std::string>& p_content) :
        m_content(p_content)
    { }

public:
    int sum_rearrange_priorities() const {
        int priority = 0;
        for (const auto& bag : m_content) {
            int len = bag.size() / 2;
            priority += get_rearrange_priority(bag.substr(0, len), bag.substr(len));
        }

        return priority;
    }

    int sum_badge_prorities() const {
        int priority = 0;
        for (int i = 3; i <= m_content.size(); i += 3) {
            priority += get_badge_priority(i - 3, i);
        }

        return priority;
    }

private:
    int get_rearrange_priority(const std::string& p_compartment1, const std::string& p_compartment2) const {
        std::set<char> unique_bag_content;
        for (const char item : p_compartment1) {
            unique_bag_content.insert(item);
        }

        char wrong_item = -1;
        bool wrong_item_found = false;
        for (const char item : p_compartment2) {
            auto iter = unique_bag_content.find(item);
            if (iter != unique_bag_content.end()) {
                wrong_item = *iter;
                wrong_item_found = true;
                break;
            }
        }

        if (!wrong_item_found) {
            throw std::invalid_argument("Wrong item is not found!");
        }

        if (std::isupper(wrong_item)) {
            return wrong_item - 'A' + 27;
        }

        return wrong_item - 'a' + 1;
    }

    int get_badge_priority(const int p_begin, const int p_end) const {
        std::unordered_set<char> prev_common_elements;

        for (const char item : m_content[p_begin]) {
            prev_common_elements.insert(item);
        }

        for (int bag_idx = p_begin + 1; bag_idx < p_end; bag_idx++) {
            std::unordered_set<char> next_common_elements;
            for (const char item : m_content[bag_idx]) {
                auto iter = prev_common_elements.find(item);
                if (iter != prev_common_elements.cend()) {
                    next_common_elements.insert(*iter);
                }
            }

            prev_common_elements = std::move(next_common_elements);
        }

        if (prev_common_elements.size() != 1) {
            throw std::invalid_argument("Badge is not found!");
        }

        const char badge = *(prev_common_elements.begin());
        if (std::isupper(badge)) {
            return badge - 'A' + 27;
        }

        return badge - 'a' + 1;
    }
};


#include <iostream>


int main() {
    const auto bags = read_rucksacks();

    std::cout << "The sum of rearrange priorities: " << rucksacks_analyser(bags).sum_rearrange_priorities() << std::endl;
    std::cout << "The sum of badge priorities: " << rucksacks_analyser(bags).sum_badge_prorities() << std::endl;

    return 0;
}
