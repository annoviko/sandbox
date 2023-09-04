#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>


int main() {
    int n = 0;
    std::cin >> n;

    std::unordered_map<char, std::string> dict = {
        { '2', "ac" },
        { '3', "df" },
        { '4', "gi" },
        { '5', "jl" },
        { '6', "mo" },
        { '7', "ps" },
        { '8', "tv" },
        { '9', "wz" }
    };

    std::vector<std::string> words;
    for (int i = 0; i < n; i++) {
        std::string word;
        std::cin >> word;

        words.push_back(word);
    }

    std::string digits;
    std::cin >> digits;

    int result = 0;
    for (const std::string& word : words) {
        if (word.size() != digits.size()) {
            continue;
        }

        bool suitable_word = true;
        for (int i = 0; (i < word.size()) && suitable_word; i++) {
            const char digit = digits[i];
            const std::string & interval = dict[digit];
            if (word[i] < interval[0] || word[i] > interval[1]) {
                suitable_word = false;
                break;
            }
        }

        if (suitable_word) {
            result++;
        }
    }

    std::cout << result << std::endl;

    return 0;
}