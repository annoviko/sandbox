#define _USE_MATH_DEFINES

#include <iostream>
#include <iomanip>
#include <cmath>
#include <limits>
#include <queue>
#include <vector>


struct position {
    int x = -1;
    int y = -1;
};


struct edge {
    position node;
    double distance = std::numeric_limits<double>::max();
};


/* The solution based on Dijkstra is only for the presentation here as one of the ways to
   solve the task. But is not efficient here - the solution will be out of time limit.
   The complixty is O(V^2). */
class solution_dijkstra {
private:
    std::vector<std::vector<double>> dist; // row = y; col = x
    double R;
    int N, M;

public:
    double shortest_distance(int _M, int _N, double _R, const position& p1, const position& p2) {
        N = _N; M = _M;  R = _R;
        dist = std::vector<std::vector<double>>(N + 1, std::vector<double>(M + 1, std::numeric_limits<double>::max()));

        auto comparator = [](const edge& left, const edge& right) { return left.distance < right.distance; };
        std::priority_queue<edge, std::vector<edge>, decltype(comparator)> queue(comparator);

        queue.push({ p1, 0.0 });
        dist[p1.y][p1.x] = 0.0;

        while (!queue.empty()) {
            edge cur = queue.top();
            queue.pop();

            if (cur.distance > dist[cur.node.y][cur.node.x]) {
                continue;
            }

            std::vector<position> neighbors = get_neighbors(cur.node);
            for (auto& nei : neighbors) {
                double dist_to_nei = cur.distance + distance(cur.node, nei);
                if (dist_to_nei < dist[nei.y][nei.x]) {
                    dist[nei.y][nei.x] = dist_to_nei;
                    queue.push({ nei, dist_to_nei });
                }
            }
        }

        return dist[p2.y][p2.x];
    }

private:
    std::vector<position> get_neighbors(const position& p) {
        std::vector<position> result;
        if (p.x - 1 >= 0) {
            result.push_back({ p.x - 1, p.y });
        }

        if (p.x + 1 < dist[p.y].size()) {
            result.push_back({ p.x + 1, p.y });
        }

        if (p.y - 1 >= 0) {
            result.push_back({ p.x, p.y - 1 });
        }

        if (p.y + 1 < dist.size()) {
            result.push_back({ p.x, p.y + 1 });
        }

        return result;
    }

    double distance(const position& p1, const position& p2) {
        const double r = R / N * std::min(p1.y, p2.y);
        const double dx = M_PI * r * std::abs(p1.x - p2.x) / M;
        const double dy = std::abs(p1.y - p2.y) * R / N;
        return dx + dy;
    }
};


/* This is more efficient solution that examines distance by moving along axis Y. Complexity O(N). */
class solution {
private:
    double R;
    int N, M;

public:
    double shortest_distance(int _M, int _N, double _R, position p1, position p2) {
        N = _N; M = _M;  R = _R;

        double result = distance(p1, p2);

        double dy_fraction = R / N;
        double dy_appendix = 0.0;

        position cur = p1.y > p2.y ? p1 : p2;
        position other = p1.y > p2.y ? p2 : p1;

        while (cur.y > 0) {
            cur.y--;
            dy_appendix += dy_fraction;

            double candidate = distance(cur, other) + dy_appendix;

            result = std::min(candidate, result);
        }

        return result;
    }

private:
    double distance(const position& p1, const position& p2) {
        const double r = R / N * std::min(p1.y, p2.y);
        const double dx = M_PI * r * std::abs(p1.x - p2.x) / M;
        const double dy = std::abs(p1.y - p2.y) * R / N;
        return dx + dy;
    }
};


int main() {
    int x1, x2, y1, y2, M, N;
    double R;

    std::cin >> M >> N >> R >> x1 >> y1 >> x2 >> y2;

    // const double distance = solution_dijkstra().shortest_distance(M, N, R, { x1, y1 }, { x2, y2 });  // O((M * N)^2)
    const double distance = solution().shortest_distance(M, N, R, { x1, y1 }, { x2, y2 });  // O(N)

    std::cout << std::setprecision(15) << distance << std::endl;

    return 0;
}