#include <algorithm>
#include <iostream>
#include <queue>
#include <string>
#include <vector>


using graph = std::vector<std::string>;


struct position {
    int i;
    int j;

    position(int p_row, int p_col) :
        i(p_row),
        j(p_col)
    { }
};


class Solution {
private:
    graph m_graph;
    std::vector<std::vector<bool>> m_visited;

public:
    Solution(const graph& p_graph) :
        m_graph(p_graph),
        m_visited(p_graph.size(), std::vector<bool>(p_graph[0].size(), false))
    { }

public:
    std::vector<int> detect() {
        std::vector<int> result;

        for (int i = 0; i < m_graph.size(); i++) {
            for (int j = 0; j < m_graph[0].size(); j++) {
                if (!m_visited[i][j] && m_graph[i][j] == '*') {
                    const int value = bfs(i, j);
                    result.push_back(value);
                }
            }
        }

        std::sort(result.begin(), result.end());
        return result;
    }

private:
    int bfs(int i, int j) {
        int result = 0;

        std::queue<position> queue;
        queue.push(position(i, j));

        m_visited[i][j] = true;

        std::vector<position> x_to_consider;

        while(!queue.empty()) {
            auto p = queue.front();
            queue.pop();

            auto neighbors = get_neighbors(p);
            for (auto & nei : neighbors) {
                char pixel = m_graph[nei.i][nei.j];
                if (!m_visited[nei.i][nei.j] && (pixel != '.')) {
                    if (pixel == 'X') {
                        x_to_consider.push_back(nei);
                    }

                    m_visited[nei.i][nei.j] = true;
                    queue.push(nei);
                }
            }
        }

        for (auto & x : x_to_consider) {
            m_visited[x.i][x.j] = false;
        }

        for (int i = 0; i < x_to_consider.size(); i++) {
            auto & pos = x_to_consider[i];
            if (!m_visited[pos.i][pos.j]) {
                bfs_x(x_to_consider[i]);
                result++;
            }
        }

        return result;
    }

    void bfs_x(const position & p_start) {
        std::queue<position> queue;
        queue.push(p_start);

        while(!queue.empty()) {
            auto p = queue.front();
            queue.pop();

            auto neighbors = get_neighbors(p);
            for (auto & nei : neighbors) {
                char pixel = m_graph[nei.i][nei.j];
                if (!m_visited[nei.i][nei.j] && (pixel == 'X')) {
                    m_visited[nei.i][nei.j] = true;
                    queue.push(nei);
                }
            }
        }
    }

    std::vector<position> get_neighbors(const position & p) {
        std::vector<position> result;

        // left
        if (p.j > 0) {
            result.push_back(position(p.i, p.j - 1));
        }

        // up
        if (p.i > 0) {
            result.push_back(position(p.i - 1, p.j));
        }

        // right
        if (p.j < m_graph[0].size() - 1) {
            result.push_back(position(p.i, p.j + 1));
        }

        // down
        if (p.i < m_graph.size() - 1) {
            result.push_back(position(p.i + 1, p.j));
        }

        return result;
    }
};


#include <string>
#include <sstream>

int main() {
    int counter = 0;
    for (std::string line; std::getline(std::cin, line);) {
        if (line.empty()) {
            return 0;
        }

        std::stringstream stream(line);
        int w, h;
        stream >> w >> h;

        if (w + h == 0) {
            return 0;
        }

        graph g;
        for (int i = 0; i < h; i++) {
            std::getline(std::cin, line);
            g.push_back(line);
        }

        counter++;
        std::vector<int> result = Solution(g).detect();
        std::cout << "Throw " << counter << std::endl;
        for (int i = 0; i < result.size(); i++) {
            std::cout << result[i];
            if (i != result.size() - 1) {
                std::cout << " ";
            }
        }

        std::cout << std::endl << std::endl;
    }

    return 0;
}
