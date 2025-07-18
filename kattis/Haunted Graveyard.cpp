#include <iostream>
#include <vector>
#include <unordered_map>
#include <limits>
#include <algorithm>


const int FIELD = 0;
const int GRAVESTONE = 1;
const int WORMHOLE = 2;

const int INF = std::numeric_limits<int>::max();

struct wormhole {
    int x;
    int y;
    int t;
};

struct edge {
    int x1;
    int y1;
    int x2;
    int y2;
    int w;
};

int main() {
    while (true) {
        int w, h;
        std::cin >> w >> h;

        if (w == 0 && h == 0) {
            break;
        }

        std::vector<std::vector<int>> map(h, std::vector<int>(w, FIELD));
        std::unordered_map<int, std::unordered_map<int, wormhole>> holes;

        int n;
        std::cin >> n;

        int total_nodes = w * h;

        for (int i = 0; i < n; i++) {
            int x, y;
            std::cin >> x >> y;

            map[y][x] = GRAVESTONE;
            total_nodes--;  /* exclude node from graph */
        }

        std::cin >> n;

        for (int i = 0; i < n; i++) {
            int x, y;
            std::cin >> x >> y;

            map[y][x] = WORMHOLE;

            wormhole w;
            std::cin >> w.x >> w.y >> w.t;

            holes[y][x] = w;
        }

        std::vector<std::vector<int>> dist(h, std::vector<int>(w, INF));
        dist[0][0] = 0;

        std::vector<edge> g;
        for (int r = 0; r < h; r++) {
            for (int c = 0; c < w; c++) {
                if (r == h - 1 && c == w - 1) {
                    /* do not add ongoing edges from the goal, we never go back from here. */
                    continue;
                }

                switch (map[r][c]) {
                case FIELD:
                {
                    // north, east, south and west are neighbors
                    std::vector<std::pair<int, int>> neis = {
                        { r + 1, c }, { r - 1, c }, { r, c + 1 }, { r, c - 1 }
                    };

                    for (const auto& nei : neis) {
                        if (nei.first < 0 || nei.first >= h || nei.second < 0 || nei.second >= w) {
                            continue;   /* out of map */
                        }

                        if (map[nei.first][nei.second] == GRAVESTONE) {
                            continue;   /* cannot climb on it - node excluded from the graph */
                        }

                        g.push_back({ c, r, nei.second, nei.first, 1 });
                    }

                    break;
                }
                case WORMHOLE:
                {
                    // single edge and it is wormhole
                    wormhole& h = holes[r][c];
                    g.push_back({ c, r, h.x, h.y, h.t });
                    break;
                }
                default:
                    break;
                }
            }
        }

        /* nodes V - go though V - 1 */
        for (int _i = 0; _i < total_nodes - 1; _i++) {
            /* edges E */
            for (const edge& e : g) {
                if (dist[e.y1][e.x1] == INF) {
                    continue;
                }

                dist[e.y2][e.x2] = std::min(dist[e.y2][e.x2], dist[e.y1][e.x1] + e.w);
            }
        }

        /* check for negative cycles */
        bool has_negative_cycle = false;

        for (const edge& e : g) {
            if (dist[e.y1][e.x1] == INF) {
                continue;
            }

            if (dist[e.y2][e.x2] > dist[e.y1][e.x1] + e.w) {
                has_negative_cycle = true;
                break;
            }
        }

        if (has_negative_cycle) {
            std::cout << "Never" << std::endl;
        }
        else if (dist[h - 1][w - 1] == INF) {
            std::cout << "Impossible" << std::endl;
        }
        else {
            std::cout << dist[h - 1][w - 1] << std::endl;
        }
    }

    return 0;
}