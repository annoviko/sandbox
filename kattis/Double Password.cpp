#include <iostream>
#include <string>


class solution {
public:
    std::uint64_t get_num_combinations(const std::string& p1, const std::string& p2) {
        std::uint64_t result = 1;
        for (int i = 0; i < p1.size(); i++) {
            if (p1[i] != p2[i]) {
                result *= 2;
            }
        }
        return result;
    }
};


int main() {
    std::string p1, p2;
    std::cin >> p1 >> p2;

    std::cout << solution().get_num_combinations(p1, p2) << std::endl;

    return 0;
}