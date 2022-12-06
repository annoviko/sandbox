// 2014 Virginia Tech High School Programming Contest
// Beekeeper

#include <cmath>
#include <vector>
#include <set>
#include <string>


const std::set<char> VOWELS = { 'a', 'e', 'i', 'o', 'u', 'y' };


class vowels_counter {
public:
    std::string find_longest(const std::vector<std::string>& p_words) {
        std::string result = p_words.front();
        std::size_t score = 0;

        for (std::size_t k = 0; k < p_words.size(); k++) {
            const auto& word = p_words[k];
            std::size_t cur_score = 0;
            for (std::size_t i = 1; i < word.size(); i++) {
                if (VOWELS.find(word[i]) == VOWELS.end()) {
                    continue;
                }

                // no word in the input will have three or more of the same vowel consecutively
                if (word[i] == word[i - 1]) {
                    cur_score++;
                }
            }

            if (cur_score > score) {
                result = word;
                score = cur_score;
            }
        }

        return result;
    }
};


#include <iostream>

int main() {
    while (true) {
        int n;
        std::cin >> n;

        if (n == 0) {
            break;
        }

        std::vector<std::string> words;
        for (std::size_t i = 0; i < n; i++) {
            std::string word;
            std::cin >> word;
            words.push_back(word);
        }

        std::cout << vowels_counter().find_longest(words) << std::endl;
    }
}