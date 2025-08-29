#include <vector>
#include <iostream>
#include <queue>


int main() {
    int n, m, home, leave;
    std::cin >> n >> m >> home >> leave;

    home--; leave--;

    std::vector<std::vector<int>> g(n);
    std::vector<int> inc_conn(n, 0);

    for (int i = 0; i < m; i++) {
        int a, b;
        std::cin >> a >> b;

        a--; b--;

        g[a].push_back(b);
        g[b].push_back(a);

        inc_conn[a]++;
        inc_conn[b]++;
    }

    std::vector<bool> excluded(n, false);
    std::vector<int> initial = inc_conn;

    std::queue<int> q;
    q.push(leave);

    inc_conn[leave] = 0;
    excluded[leave] = true;

    while (!q.empty()) {
        int cur = q.front();
        q.pop();

        for (int nei : g[cur]) {
            if (excluded[nei]) {
                continue;
            }

            inc_conn[nei]--;
            if (inc_conn[nei] <= initial[nei] / 2) {
                q.push(nei);
                excluded[nei] = true;
            }
        }
    }

    if (excluded[home]) {
        std::cout << "leave" << std::endl;
    }
    else {
        std::cout << "stay" << std::endl;
    }

    return 0;
}