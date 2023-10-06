#include <iostream>
#include <vector>


int main() {
    std::vector<int> freq(7, 0);
    std::vector<int> value_to_participant(7, -1);

    int n;
    std::cin >> n;

    for (int i = 0; i < n; i++) {
        int value;
        std::cin >> value;

        freq[value]++;
        value_to_participant[value] = i + 1;
    }

    int index_winner = -1;
    for (int i = freq.size() - 1; i >= 0; i--) {
        if (freq[i] != 1) {
            continue;
        }

        index_winner = value_to_participant[i];
        break;
    }

    if (index_winner != -1) {
        std::cout << index_winner << std::endl;
    }
    else {
        std::cout << "none" << std::endl;
    }

    return 0;
}