#include <iostream>
#include <string>
#include <unordered_set>


int main() {
    int n = 0;
    std::cin >> n;

    std::unordered_set<std::string> unique_words;

    std::string current_word;
    std::cin >> current_word;

    unique_words.insert(current_word);

    int i = 1;
    for (; i < n; i++) {
        std::string next_word;
        std::cin >> next_word;

        if (next_word.front() != current_word.back()) {
            std::cout << "Player " << ((i % 2 == 0) ? "1" : "2") << " lost" << std::endl;
            return 0;
        }

        if (!unique_words.insert(next_word).second) {
            std::cout << "Player " << ((i % 2 == 0) ? "1" : "2") << " lost" << std::endl;
            return 0;
        }

        current_word = std::move(next_word);
    }

    std::cout << "Fair Game" << std::endl;
    return 0;
}
