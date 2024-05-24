#include <iostream>
#include <queue>
#include <string>
#include <vector>
#include <unordered_map>


using graph = std::vector<std::vector<int>>;


class solution {
public:
    int solve(const graph& g, const std::vector<int>& skepticism, const int initial_id, const int days_limit) {
        std::queue<int> to_proc;
        to_proc.push(initial_id);

        std::vector<bool> spread_rumor(g.size(), false);
        std::vector<bool> heard_rumor(g.size(), false);
        std::vector<int> rumors(g.size(), 0);

        spread_rumor[initial_id] = true;
        heard_rumor[initial_id] = true;

        int cur_day = 0;
        int counter = 0;

        while (!to_proc.empty() && (cur_day < days_limit)) {
            cur_day++;

            std::queue<int> cur_day_to_proc = std::move(to_proc);

            while (!cur_day_to_proc.empty()) {
                int cur_id = cur_day_to_proc.front();
                cur_day_to_proc.pop();

                for (int nei : g[cur_id]) {
                    if (spread_rumor[nei]) {
                        continue;
                    }

                    if (!heard_rumor[nei]) {
                        heard_rumor[nei] = true;
                        counter++;
                    }

                    rumors[nei]++;
                    if (rumors[nei] >= skepticism[nei]) {
                        spread_rumor[nei] = true;
                        to_proc.push(nei);
                    }
                }
            }
        }

        return counter;
    }
};


int main() {
    int n, m, d;
    std::cin >> n >> m >> d;

    std::unordered_map<std::string, int> dict_name_to_id;
    std::vector<int> skepticism(n, 0);
    for (int i = 0; i < n; i++) {
        std::string name;
        int value;

        std::cin >> name >> value;
        dict_name_to_id[name] = i;
        skepticism[i] = value;
    }

    graph g(n);
    for (int i = 0; i < m; i++) {
        std::string from_str, to_str;
        std::cin >> from_str >> to_str;

        int from = dict_name_to_id[from_str];
        int to = dict_name_to_id[to_str];

        g[from].push_back(to);
        g[to].push_back(from);
    }

    std::string start_name;
    std::cin >> start_name;

    int start_id = dict_name_to_id[start_name];
    int counter = solution().solve(g, skepticism, start_id, d);

    std::cout << counter << std::endl;

    return 0;
};