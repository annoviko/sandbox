#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>


using graph = std::vector<std::vector<int>>;


class solution {
public:
    int solve(const graph& g, const std::vector<int>& dict) {
        return std::min(solve(g, dict, true), solve(g, dict, false));
    }

    int solve(const graph& g, const std::vector<int>& dict, bool take_first) {
        std::vector<int> incoming_conn(g.size(), 0);
        
        for (int from = 0; from < g.size(); from++) {
            for (int to : g[from]) {
                incoming_conn[to]++;
            }
        }

        std::queue<int> q1, q2;
        for (int id = 0; id < g.size(); id++) {
            if (incoming_conn[id] == 0) {
                if (dict[id] == 1) {
                    q1.push(id);
                }
                else {
                    q2.push(id);
                }
            }
        }

        int transfers = 0;
        int cur_lab = take_first ? 1 : 2;

        while (!q1.empty() || !q2.empty()) {
            int from = -1;
            if (cur_lab == 1) {
                if (q1.empty()) {
                    /* need to transfer the object to another laboratory we cannot work in the first lab */
                    from = q2.front();
                    q2.pop();

                    cur_lab = 2;
                    transfers++;
                }
                else {
                    from = q1.front();
                    q1.pop();
                }
            }
            else {
                if (q2.empty()) {
                    from = q1.front();
                    q1.pop();

                    cur_lab = 1;
                    transfers++;
                }
                else {
                    from = q2.front();
                    q2.pop();
                }
            }

            for (int to : g[from]) {
                incoming_conn[to]--;
                if (incoming_conn[to] == 0) {
                    if (dict[to] == 1) {
                        q1.push(to);
                    }
                    else {
                        q2.push(to);
                    }
                }
            }
        }

        return transfers;
    }
};


int main() {
    int t = 0;
    std::cin >> t;

    for (int i = 0; i < t; i++) {
        int nodes = 0;
        int relations = 0;
        std::cin >> nodes >> relations;

        std::vector<int> dict(nodes, -1);
        for (int id = 0; id < nodes; id++) {
            std::cin >> dict[id];
        }

        graph g(nodes);
        for (int entry_id = 0; entry_id < relations; entry_id++) {
            int from, to;
            std::cin >> from >> to;

            from--;
            to--;

            g[from].push_back(to);
        }

        std::cout << solution().solve(g, dict) << std::endl;
    }

    return 0;
}