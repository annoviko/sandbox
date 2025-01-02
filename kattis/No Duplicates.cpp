#include <string>
#include <unordered_set>
#include <iostream>


int main() {
    std::unordered_set<std::string> unique_words;
    std::string word;

    while (std::cin >> word) {
        auto iter = unique_words.find(word);
        if (iter != unique_words.cend()) {
            std::cout << "no" << std::endl;
            return 0;
        }

        unique_words.insert(word);
    }

    std::cout << "yes" << std::endl;
    return 0;
}
