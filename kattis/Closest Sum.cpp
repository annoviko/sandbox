#include <iostream>
#include <climits>
#include <cmath>
#include <vector>


int main() {
    int c = 1;

    while (true) {
        int n;
        if (!(std::cin >> n)) {
            break;
        }

        std::vector<int> nums(n, 0);
        for (int i = 0; i < n; i++) {
            std::cin >> nums[i];
        }

        int m;
        std::cin >> m;

        std::cout << "Case " << c << ':' << std::endl;

        for (int i = 0; i < m; i++) {
            int target;
            std::cin >> target;

            int delta = INT_MAX;
            int closest_sum = -1;

            for (int k = 0; k < nums.size(); k++) {
                for (int j = 0; j < nums.size(); j++) {
                    if (k == j) {
                        continue;
                    }

                    int candidate = nums[k] + nums[j];
                    int candidate_delta = std::abs(candidate - target);

                    if (candidate_delta < delta) {
                        closest_sum = candidate;
                        delta = candidate_delta;
                    }
                }
            }

            std::cout << "Closest sum to " << target << " is " << closest_sum << "." << std::endl;
        }

        c++;
    }
}
