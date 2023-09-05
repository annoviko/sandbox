#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <unordered_set>


int main() {
    std::unordered_set<std::string> used_words;

    for (std::string line; std::getline(std::cin, line);) {
        std::stringstream stream(line);

        std::string result;
        bool first_word = true;

        for (std::string word; stream >> word;) {
            const std::string original_word = word;

            std::transform(word.begin(), word.end(), word.begin(), [](unsigned char c) { 
                return std::tolower(c); 
            });

            auto iter = used_words.find(word);
            if (iter == used_words.cend()) {
                result += (first_word ? "" : " ") + original_word;
                used_words.insert(word);
                
            }
            else {
                result += (first_word ? "" : " ") + std::string(".");
            }

            first_word = false;
        }

        std::cout << result << std::endl;
    }

    std::cout << std::endl;

    return 0;
}