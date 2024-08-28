#include <iostream>
#include <queue>
#include <vector>
#include <string>


using graph = std::vector<std::string>;


struct position {
    int r = 0;
    int c = 0;
};


class solution {
private:
    const graph& g;
    std::vector<std::vector<bool>> visited;

public:
    solution(const graph& p_g) :
        g(p_g),
        visited(p_g.size(), std::vector<bool>(p_g[0].size(), false))
    { }

public:
    int count() {
        int result = 0;

        std::vector<int> row_to_consider = { 0, (int)g.size() - 1 };
        for (int r : row_to_consider) {
            for (int c = 0; c < g[r].size(); c++) {
                if (g[r][c] == '0' && visited[r][c] == false) {
                    result += dfs(r, c);
                }
                else if (g[r][c] == '1') {
                    result++;
                }
            }
        }

        std::vector<int> cols_to_consider = { 0, (int)g[0].size() - 1 };
        for (int r = 0; r < g.size(); r++) {
            for (int c : cols_to_consider) {
                if (g[r][c] == '0' && visited[r][c] == false) {
                    result += dfs(r, c);
                }
                else if (g[r][c] == '1') {
                    result++;
                }
            }
        }

        return result;
    }

private:
    int dfs(int r, int c) {
        int length = 0;

        std::queue<position> to_consider;
        to_consider.push({ r, c });

        visited[r][c] = true;

        while (!to_consider.empty()) {
            auto cur = to_consider.front();
            to_consider.pop();

            auto neis = get_neighbors(cur.r, cur.c);
            for (auto& nei : neis) {
                if (visited[nei.r][nei.c]) {
                    continue;
                }

                if (g[nei.r][nei.c] == '1') {
                    length++;
                }
                else {
                    to_consider.push(nei);
                    visited[nei.r][nei.c] = true;
                }
            }
        }

        return length;
    }

    std::vector<position> get_neighbors(int r, int c) {
        std::vector<position> neis;
        if (r > 0) {
            neis.push_back({ r - 1, c });
        }

        if (r < g.size() - 1) {
            neis.push_back({ r + 1, c });
        }

        if (c > 0) {
            neis.push_back({ r, c - 1 });
        }

        if (c < g[r].size() - 1) {
            neis.push_back({ r, c + 1 });
        }

        return neis;
    }
};


int main() {
    int r, c;
    std::cin >> r >> c;

    graph g;
    g.reserve(r);

    for (int i = 0; i < r; i++) {
        std::string row;
        row.reserve(c + 1);

        for (int j = 0; j < c; j++) {
            char s;
            std::cin >> s;
            row.push_back(s);
        }

        g.push_back(std::move(row));
    }

    std::cout << solution(g).count() << std::endl;
    return 0;
}