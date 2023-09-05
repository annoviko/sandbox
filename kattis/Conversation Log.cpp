#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>


int main() {
    std::unordered_map<std::string, std::unordered_map<std::string, int>> freq;

    std::string line;
    std::getline(std::cin, line);

    int n = std::stoi(line);

    for (int i = 0; i < n; i++) {
        std::getline(std::cin, line);
        std::stringstream stream(line);

        std::string name;
        stream >> name;

        for (std::string word; stream >> word;) {
            freq[name][word]++;
        }
    }

    std::unordered_set<std::string> common_words;
    bool first_iteration = true;

    for (auto & user_dict : freq) {
        if (first_iteration) {
            for (auto& word_freq : user_dict.second) {
                common_words.insert(word_freq.first);
            }

            first_iteration = false;
            continue;
        }

        std::vector<std::string> to_delete;
        for (auto& common_word : common_words) {
            auto iter = user_dict.second.find(common_word);
            if (iter == user_dict.second.end()) {
                to_delete.push_back(common_word);
            }
        }

        for (const auto& word_to_delete : to_delete) {
            common_words.erase(word_to_delete);
        }
    }

    using freq_entry = std::pair<std::string, int>;
    std::vector<freq_entry> common_freq;
    for (auto& word : common_words) {
        common_freq.push_back({ word, 0 });

        for (auto& user_dict : freq) {
            common_freq.back().second += user_dict.second[word];
        }
    }

    std::sort(common_freq.begin(), common_freq.end(), [](const freq_entry& l, const freq_entry& r) {
        if (l.second == r.second) {
            return l.first < r.first;
        }

        return l.second > r.second;
    });

    if (common_freq.empty()) {
        std::cout << "ALL CLEAR" << std::endl;
    }

    for (freq_entry& entry : common_freq) {
        std::cout << entry.first << std::endl;
    }

    return 0;
}
