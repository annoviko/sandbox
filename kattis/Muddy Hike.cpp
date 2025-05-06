#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <limits>


struct state {
    int mud;
    int r;
    int c;
};


struct comparator {
    bool operator()(const state& l, const state& r) {
        return l.mud > r.mud;
    }
};


struct position {
    int r;
    int c;
};


std::vector<position> get_neis(const int r, const int c, const int h, const int w) {
    std::vector<position> neis;

    if (r + 1 < h) {
        neis.push_back({ r + 1, c });
    }
    if (r - 1 >= 0) {
        neis.push_back({ r - 1, c });
    }
    if (c + 1 < w) {
        neis.push_back({ r, c + 1 });
    }
    if (c - 1 >= 0) {
        neis.push_back({ r, c - 1 });
    }

    return neis;
}


int main() {
    int r, c;
    std::cin >> r >> c;

    std::vector<std::vector<int>> mud(r, std::vector<int>(c + 2, 0));
    for (int i = 0; i < r; i++) {
        for (int j = 1; j <= c; j++) {
            std::cin >> mud[i][j];
        }
    }

    std::priority_queue<state, std::vector<state>, comparator> pq;
    std::vector<std::vector<bool>> visited(r, std::vector<bool>(c + 2, false));
    std::vector<std::vector<int>> dist(r, std::vector<int>(c + 2, std::numeric_limits<int>::max()));

    pq.push({ 0, 0, 0 });

    while (!pq.empty()) {
        state cur = pq.top();
        pq.pop();

        visited[cur.r][cur.c] = true;

        auto neis = get_neis(cur.r, cur.c, r, c + 2);
        for (const auto& nei : neis) {
            if (visited[nei.r][nei.c]) {
                continue;
            }

            int cost = std::max(cur.mud, mud[nei.r][nei.c]);
            if (dist[nei.r][nei.c] > cost) {
                dist[nei.r][nei.c] = cost;
                pq.push({ cost, nei.r, nei.c });
            }
        }
    }

    std::cout << dist.back().back() << std::endl;
    return 0;
}