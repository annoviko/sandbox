#include <algorithm>
#include <cmath>
#include <numeric>
#include <queue>
#include <vector>


struct coord {
    int x = -1;
    int y = -1;

public:
    coord(const int p_x, const int p_y) :
        x(p_x), y(p_y)
    { }

    coord get_up() {
        return { x, y + 1 };
    }

    coord get_down() {
        return { x, y - 1 };
    }

    coord get_left() {
        return { x - 1, y };
    }

    coord get_right() {
        return { x + 1, y };
    }
};


class shortest_path {
private:
    const int not_visited = -2;
    std::vector<std::vector<int>> m_field = std::vector<std::vector<int>>(2001, std::vector<int>(2001, not_visited));

public:
    int calculate(const std::vector<coord> & p_mall1, const std::vector<coord> & p_mall2) {
        const int goal = -1;

        std::queue<coord> to_process;

        for (auto & c : p_mall1) {
            m_field[c.x][c.y] = 0;
            to_process.push(c);
        }

        for (auto& c : p_mall2) {
            m_field[c.x][c.y] = goal;
        }

        while (!to_process.empty()) { // O(V+E)
            coord cur = to_process.front();
            to_process.pop();

            std::vector<coord> neis = { cur.get_up(), cur.get_right(), cur.get_down(), cur.get_left() };
            for (auto& nei : neis) {
                if (is_inside(nei)) {
                    if (m_field[nei.x][nei.y] == goal) {
                        return m_field[cur.x][cur.y] + 1;
                    }
                    if (m_field[nei.x][nei.y] == not_visited) {
                        m_field[nei.x][nei.y] = m_field[cur.x][cur.y] + 1;
                        to_process.push(nei);
                    }
                }
            }
        }

        return -1;
    }

    bool is_inside(const coord& p_c) {
        return p_c.x >= 0 && p_c.y >= 0 && p_c.x < 2001 && p_c.y < 2001;
    }
};


#include <iostream>
#include <iomanip>


int main() {
    for (;;) {
        int n;
        std::cin >> n;

        if (n == 0) {
            return 0;
        }

        std::vector<coord> mall1;
        for (int i = 0; i < n; i++) {
            int x, y;
            std::cin >> x >> y;

            mall1.push_back(coord(x, y));
        }

        std::cin >> n;
        std::vector<coord> mall2;
        for (int i = 0; i < n; i++) {
            int x, y;
            std::cin >> x >> y;

            mall2.push_back(coord(x, y));
        }

        std::cout << shortest_path().calculate(mall1, mall2) << std::endl;
    }

    return 0;
}