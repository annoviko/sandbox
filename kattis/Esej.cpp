#include <iostream>
#include <string>


const int NUM_LETTERS = 26;
const int MAX_WORD_SIZE = 15;


std::string word_by_value(int value) {
    std::string word(1, 'a' + (value % NUM_LETTERS));
    int remain = value / NUM_LETTERS;

    while (remain > 0) {
        if (word.size() == MAX_WORD_SIZE) {
            word.clear();
        }

        word.append(1, 'a' + (remain % NUM_LETTERS));
        remain /= NUM_LETTERS;
    }

    return word;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);

    int a, b;
    std::cin >> a >> b;

    for (int i = 0; i < b; i++) {
        std::cout << word_by_value(i) << ' ';
    }

    std::cout << '\n';

    return 0;
}