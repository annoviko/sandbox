#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>


int main() {
    for (std::string line; std::getline(std::cin, line);) {
        if (line.empty()) {
            break;
        }

        std::stringstream sstream(line);

        std::vector<int> ruler;
        for (int value = -1; sstream >> value;) {
            ruler.push_back(value);
        }

        std::sort(ruler.begin(), ruler.end());

        std::unordered_map<int, int> dist_freq;

        for (int i = 0; i < ruler.size(); i++) {
            for (int j = i + 1; j < ruler.size(); j++) {
                int dist = ruler[j] - ruler[i];
                dist_freq[dist]++;
            }
        }

        bool is_ruler = true;
        int largest_mark = ruler.back();
        std::vector<int> missing_numbers;

        for (int i = 1; i <= largest_mark; i++) {
            auto iter = dist_freq.find(i);
            if (iter == dist_freq.cend()) {
                missing_numbers.push_back(i);   // ? distance, not number
            }
            else if (iter->second > 1) {
                is_ruler = false;
                break;
            }
        }

        if (!is_ruler) {
            std::cout << "not a ruler" << std::endl;
        }
        else if (!missing_numbers.empty()) {
            std::cout << "missing";

            for (auto v : missing_numbers) {
                std::cout << ' ' << v;
            }

            std::cout << std::endl;
        }
        else {
            std::cout << "perfect" << std::endl;
        }
    }

    return 0;
}