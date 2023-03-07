#include <iostream>
#include <vector>
#include <unordered_map>


class solution {
public:
    std::vector<int> get_stats(const int p_capacity, std::vector<int> p_teams) {
        std::vector<int> stats(p_teams.size(), 0);
        int registered = 0;
        int remaining = 0;

        for (int i = 0; i < p_teams.size(); i++) {
            remaining += p_teams[i];
        }

        while (true) {
            for (int i = 0; i < p_teams.size(); i++) {
                if (p_teams[i] == 0) {
                    continue;
                }

                stats[i]++;
                registered++;
                if (registered == p_capacity) {
                    return stats;
                }

                p_teams[i]--;
                remaining--;
                if (remaining == 0) {
                    return stats;
                }
            }
        }
    }
};


int main() {
    int n, m;
    std::cin >> n >> m;

    std::vector<int> teams;
    for (int i = 0; i < m; i++) {
        int t;
        std::cin >> t;
        teams.push_back(t);
    }

    auto stats = solution().get_stats(n, teams);

    for (auto stat : stats) {
        std::cout << stat << std::endl;
    }

    return 0;
}
