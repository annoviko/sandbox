#include <iostream>
#include <vector>
#include <unordered_map>


struct result {
public:
    std::vector<int> subset1;
    std::vector<int> subset2;

public:
    bool is_empty() const {
        return subset1.empty() && subset2.empty();
    }
};


std::ostream& operator<<(std::ostream& stream, const std::vector<int>& vals) {
    bool is_first = true;
    for (int i = 0; i < vals.size(); i++) {
        if (!is_first) {
            std::cout << ' ';
        }

        is_first = false;
        stream << vals[i];
    }

    return stream;
}


class solution {
public:
    result find_equal_subsets(const int n, const std::vector<int>& ar) {
        std::unordered_map<int, std::vector<int>> hash;     /* [sum] -> [subset] */

        for (std::uint32_t i = 1; i < std::uint32_t(1 << n); i++) {
            std::vector<int> subset = { };
            int sum = 0;

            for (std::uint32_t bitmask = i, cur_index = 0; bitmask != 0; bitmask = (bitmask >> 1), cur_index++) {
                if ((bitmask & std::uint64_t(1)) == 0) {
                    continue;
                }

                subset.push_back(ar[cur_index]);
                sum += ar[cur_index];
            }

            auto iter = hash.find(sum);
            if (iter != hash.end()) {
                return { iter->second, subset };
            }
            else {
                hash[sum] = std::move(subset);
            }
        }

        return { };
    }
};


int main() {
    int t = 0;
    std::cin >> t;

    for (int i = 0; i < t; i++) {
        int n = 0;
        std::cin >> n;

        std::vector<int> set;
        for (int j = 0; j < n; j++) {
            int val = 0;
            std::cin >> val;

            set.push_back(val);
        }

        auto res = solution().find_equal_subsets(n, set);
        std::cout << "Case #" << i + 1 << ":" << std::endl;
        if (res.is_empty()) {
            std::cout << "Impossible" << std::endl;
        }
        else {
            std::cout << res.subset1 << std::endl;
            std::cout << res.subset2 << std::endl;
        }
    }

    return 0;
}