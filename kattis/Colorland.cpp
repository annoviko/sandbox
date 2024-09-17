#include <iostream>
#include <vector>
#include <climits>
#include <set>
#include <unordered_map>


const int IMPOSSIBLE = std::numeric_limits<int>::max();


const std::unordered_map<std::string, int> NAME_TO_ID = {
    { "Blue", 0 },
    { "Orange", 1 },
    { "Pink", 2 },
    { "Green", 3 },
    { "Red", 4 },
    { "Yellow", 5 }
};


class solution {
private:
    std::unordered_map<int, int> cache;  /* index -> position in array */
    std::vector<int> ar;

    std::vector<std::set<int>> color_map;

public:
    solution(const std::vector<int>& p_ar) :
        cache(),
        ar(p_ar),
        color_map(NAME_TO_ID.size())
    {
        for (int i = 0; i < ar.size(); i++) {
            int color_id = ar[i];
            color_map[color_id].insert(i);
        }
    }

public:
    int solve(int index = -1) {
        if (index == ar.size() - 1) {
            return 0;
        }

        auto iter = cache.find(index);
        if (iter != cache.end()) {
            return iter->second;
        }

        /* find next colors */
        std::vector<int> next_colors = { -1, -1, -1, -1, -1, -1 };
        for (int color_id = 0; color_id < NAME_TO_ID.size(); color_id++) {
            auto iter = color_map[color_id].upper_bound(index);
            if (iter == color_map[color_id].end()) {
                continue;
            }

            next_colors[color_id] = *iter;
        }

        int steps = IMPOSSIBLE;
        for (int color_id = 0; color_id < NAME_TO_ID.size(); color_id++) {
            if (next_colors[color_id] == -1) {
                continue;
            }

            const int next_index = next_colors[color_id];
            steps = std::min(steps, solve(next_index));
        }

        int result = (steps == IMPOSSIBLE) ? IMPOSSIBLE : steps + 1;

        cache[index] = result;
        return result;
    }
};


int main() {
    int n = 0;
    std::cin >> n;

    std::vector<int> ar;
    for (int i = 0; i < n; i++) {
        std::string color;
        std::cin >> color;

        int color_id = NAME_TO_ID.find(color)->second;
        ar.push_back(color_id);
    }

    std::cout << solution(ar).solve() << std::endl;
    return 0;
}
