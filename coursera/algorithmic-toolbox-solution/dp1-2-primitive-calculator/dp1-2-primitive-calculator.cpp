#include <iostream>
#include <vector>
#include <algorithm>

using std::vector;

vector<std::size_t> optimal_sequence(int n) {
    std::vector<int> dp(n + 1, 0);
    std::vector<std::size_t> path(n + 1, 0);

    for (std::size_t i = 1; i < dp.size(); i++) {
        int candidate = dp[i - 1] + 1;
        std::size_t from = i - 1;

        if (i % 2 == 0) {
            if (candidate > dp[i / 2] + 1) {
                from = i / 2;
                candidate = dp[from] + 1;
            }
        }
        if (i % 3 == 0) {
            if (candidate > dp[i / 3] + 1) {
                from = i / 3;
                candidate = std::min(candidate, dp[from] + 1);
            }
        }

        dp[i] = candidate;
        path[i] = from;
    }

    std::vector<std::size_t> sequence;
    sequence.reserve(dp.size());

    for (std::size_t i = path.size() - 1; i != 0; i = path[i]) {
        sequence.push_back(i);
    }

    return sequence;
}

int main() {
    int n;
    std::cin >> n;
    vector<std::size_t> sequence = optimal_sequence(n);
    std::cout << sequence.size() - 1 << std::endl;
    for (auto it = sequence.rbegin(); it != sequence.rend(); it++) {
        std::cout << *it << " ";
    }
}
