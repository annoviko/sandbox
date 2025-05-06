#include <algorithm>
#include <iostream>
#include <vector>
#include <limits>
#include <queue>


struct state {
    int cost;
    int row;
    int col;
};


struct comparator {
    bool operator()(const state& l, const state& r) {
        return l.cost > r.cost;
    }
};


std::vector<std::pair<int, int>> get_neis(int r, int c, int h, int w) {
    std::vector<std::pair<int, int>> neis;
    if (r - 1 >= 0) {
        neis.push_back({ r - 1, c });
    }
    if (c - 1 >= 0) {
        neis.push_back({ r, c - 1 });
    }
    if (r + 1 < h) {
        neis.push_back({ r + 1, c });
    }
    if (c + 1 < w) {
        neis.push_back({ r, c + 1 });
    }

    return neis;
}


int main() {
    int h, w;
    std::cin >> h >> w;

    std::vector<std::vector<int>> field(h, std::vector<int>(w, 0));

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            std::cin >> field[i][j];
        }
    }

    std::vector<std::vector<int>> dist(h, std::vector<int>(w, std::numeric_limits<int>::max()));
    std::vector<std::vector<bool>> visited(h, std::vector<bool>(w, false));

    std::priority_queue<state, std::vector<state>, comparator> pq;    /* [distance][node id] */

    dist[0][0] = 0;
    pq.push({ 0, 0, 0 });

    while (!pq.empty()) {
        state cur = pq.top();
        pq.pop();

        visited[cur.row][cur.col] = true;

        auto neis = get_neis(cur.row, cur.col, h, w);
        for (const std::pair<int, int> nei : neis) {
            if (visited[nei.first][nei.second]) {
                continue;
            }

            int step_cost = field[nei.first][nei.second] - field[cur.row][cur.col];
            if (step_cost < 0) {
                step_cost = 0;
            }

            int new_cost = std::max(cur.cost, step_cost);

            if (dist[nei.first][nei.second] > new_cost) {
                dist[nei.first][nei.second] = new_cost;
                pq.push({ new_cost, nei.first, nei.second });
            }
        }
    }

    std::cout << dist[h - 1][w - 1] << std::endl;
    return 0;
}
