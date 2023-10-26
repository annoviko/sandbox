#include <cmath>
#include <cstdint>
#include <vector>


using matrix = std::vector<std::vector<std::int64_t>>;


class solution {
public:
    bool is_symetric(const matrix& p_matrix) {
        int middle = (int)std::ceil(p_matrix.size() / 2.0);
        for (int i = 0; i < p_matrix.size(); i++) {
            for (int j = 0; j < p_matrix.size(); j++) {
                const auto current = p_matrix[i][j];
                const auto opposite = p_matrix[p_matrix.size() - i - 1][p_matrix.size() - j - 1];

                if ((current < 0) || (current != opposite)) {
                    return false;
                }
            }
        }

        return true;
    }
};


#include <iostream>

int main() {
    int cases = 0;
    std::cin >> cases;

    for (int c = 0; c < cases; c++) {
        std::string ignore;
        std::cin >> ignore;
        std::cin >> ignore;

        int n = 0;
        std::cin >> n;

        matrix mtrx;
        for (int i = 0; i < n; i++) {
            std::vector<std::int64_t> row;
            for (int j = 0; j < n; j++) {
                std::int64_t value;
                std::cin >> value;

                row.push_back(value);
            }

            mtrx.push_back(row);
        }

        std::cout << "Test #" << (c + 1) << ": " << (solution().is_symetric(mtrx) ? "Symmetric." : "Non-symmetric.") << std::endl;
    }
}