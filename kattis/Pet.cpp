#include <iostream>


int main() {
    int id = -1;
    int max_score = 0;

    for (int i = 0; i < 5; i++) {
        int candidate_score = 0;
        for (int j = 0; j < 4; j++) {
            int score = 0;
            std::cin >> score;

            candidate_score += score;
        }

        if (candidate_score > max_score) {
            id = i + 1;
            max_score = candidate_score;
        }
    }

    std::cout << id << " " << max_score << std::endl;

    return 0;
}