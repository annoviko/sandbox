#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <unordered_map>
#include <unordered_set>


using positive_answers = std::unordered_set<char>;
using answer_statistic = std::unordered_map<char, std::size_t>;


std::list<positive_answers> read_positive_answers() {
    std::list<positive_answers> result;

    std::ifstream stream("input.txt");

    positive_answers current_group;
    for (std::string answer_entry; std::getline(stream, answer_entry); ) {
        if (answer_entry.empty()) {
            result.push_back(std::move(current_group));
        }

        for (auto question_id : answer_entry) {
            current_group.insert(question_id);
        }
    }

    result.push_back(std::move(current_group));

    return result;
}


std::list<positive_answers> read_common_positive_answers() {
    std::list<positive_answers> result;

    std::ifstream stream("input.txt");

    auto get_answer_intersection = [](const answer_statistic & p_stats, const std::size_t p_group_size) {
        positive_answers answers;
        for (const auto & pair : p_stats) {
            if (pair.second == p_group_size) {
                answers.insert(pair.first);
            }
        }

        return answers;
    };

    answer_statistic statistics;
    std::size_t current_group_size = 0;

    for (std::string answer_entry; std::getline(stream, answer_entry); ) {
        if (answer_entry.empty()) {
            positive_answers current_group = get_answer_intersection(statistics, current_group_size);
            result.push_back(std::move(current_group));

            statistics.clear();
            current_group_size = 0;
        }
        else {
            current_group_size++;
            for (const auto marker : answer_entry) {
                statistics[marker]++;
            }
        }
    }

    positive_answers current_group = get_answer_intersection(statistics, current_group_size);
    result.push_back(std::move(current_group));

    return result;
}


std::size_t get_amount_positive_answers(const std::list<positive_answers> & p_positive_answers) {
    std::size_t result = 0;
    for (const auto & answer : p_positive_answers) {
        result += answer.size();
    }

    return result;
}



int main() {
    std::cout << "The sum of unique amount of positive answers within each group: " << get_amount_positive_answers(read_positive_answers()) << std::endl;
    std::cout << "The sum of unique amount of common positive answers within each group: " << get_amount_positive_answers(read_common_positive_answers()) << std::endl;

    return 0;
}
