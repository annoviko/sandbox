#include <iostream>
#include <vector>
#include <queue>


using graph = std::vector<std::vector<int>>;


class Solution {
    std::vector<int> m_color;

public:
    void solve(const graph& g) {
        m_color = std::vector<int>(g.size(), -1);

        int it_requests = 0;
        bool infected_network_without_request = false;

        for (int i = 0; i < g.size(); i++) {
            if (m_color[i] == -1) {
                it_requests++;

                if (bfs(g, i)) {
                    infected_network_without_request = true;    /* we just need to connect all other networks to it */
                }
            }
        }

        if (infected_network_without_request) {
            /* one of the network can infected without requests, just connect other networks to it */
            std::cout << it_requests - 1 << std::endl;
        }
        else {
            /* all networks requies IT ticket, so create one ticket for one of the networks and then connect all to this one */
            std::cout << it_requests << std::endl;
        }
    }

private:
    bool bfs(const graph& g, int index) {
        std::queue<int> to_proc;
        to_proc.push(index);

        m_color[index] = 0;
        bool odd_cycle = false;

        while (!to_proc.empty()) {
            int cur = to_proc.front();
            to_proc.pop();

            for (int nei : g[cur]) {
                if (m_color[nei] == -1) {
                    m_color[nei] = (m_color[cur] == 0) ? 1 : 0;
                    to_proc.push(nei);
                }
                else if (m_color[nei] == m_color[cur]) {
                    odd_cycle = true;
                }
            }
        }

        return odd_cycle;   /* we can infect all machines */
    }
};


int main() {
    int n, m;
    std::cin >> n >> m;

    graph g(n);
    for (int i = 0; i < m; i++) {
        int from, to;
        std::cin >> from >> to;

        from--;
        to--;

        g[from].push_back(to);
        g[to].push_back(from);
    }

    Solution().solve(g);
    return 0;
}
