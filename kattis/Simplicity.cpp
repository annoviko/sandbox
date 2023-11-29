#include <algorithm>
#include <vector>
#include <iostream>


int main() {
    std::vector<int> freq('z' + 1, 0);

    std::string word;
    std::cin >> word;

    int simplicity = 0;
    for (auto ch : word) {
        if (freq[ch] == 0) {
            simplicity++;
        }

        freq[ch]++;
    }

    if (simplicity <= 2) {
        std::cout << 0 << std::endl;
        return 0;
    }

    std::sort(freq.begin(), freq.end(), [](int p1, int p2) {
        return p1 < p2;
    });

    int letters_to_erase = 0;
    for (int i = 0; (simplicity > 2) && (i < freq.size()); i++) {
        if (freq[i] == 0) {
            continue;
        }

        letters_to_erase += freq[i];
        simplicity--;
    }

    std::cout << letters_to_erase << std::endl;
    return 0;
}