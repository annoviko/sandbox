#include <iostream>
#include <fstream>
#include <queue>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>


struct position_t {
    int x = -1;
    int y = -1;
    int z = -1;
};


using space_t = std::unordered_map<int, std::unordered_map<int, std::unordered_set<int>>>;    /* x, y, z */


std::pair<space_t, int> read_input() {
    std::ifstream stream("input.txt");

    space_t space;
    int blocks = 0;

    for (std::string line; std::getline(stream, line);) {
        std::stringstream ss(line);
        
        int x, y, z;
        char ignore;
        ss >> x >> ignore >> y >> ignore >> z;

        space[x][y].insert(z);
        blocks++;
    }

    return { space, blocks };
}


class solution {
private:
    space_t sp;
    int blocks = 0;

public:
    solution(const space_t& s, int n) : sp(s), blocks(n) {}

public:
    int surface_area() {
        const std::vector<position_t> dirs = {
            { 0, 0, 1 },
            { 0, 0, -1 },
            { 0, 1, 0 },
            { 0, -1, 0 },
            { 1, 0, 0 },
            { -1, 0, 0 }
        };

        int surface = 6 * blocks;
        for (auto& x_iter : sp) {
            int x = x_iter.first;

            for (auto& y_iter : x_iter.second) {
                int y = y_iter.first;

                for (int z : y_iter.second) {
                    /* inspect every direction */
                    for (const auto& dir : dirs) {
                        if (is_occupied(sp, x + dir.x, y + dir.y, z + dir.z)) {
                            surface--;
                        }
                    }
                }
            }
        }

        return surface;
    }

    int extrnal_surface_area() {
        const std::vector<position_t> dirs = {
            { 0, 0, 1 },
            { 0, 0, -1 },
            { 0, 1, 0 },
            { 0, -1, 0 },
            { 1, 0, 0 },
            { -1, 0, 0 }
        };

        position_t max = { 0, 0, 0 }, min = { INT_MAX, INT_MAX, INT_MAX };
        for (auto& x_iter : sp) {
            int x = x_iter.first;

            max.x = std::max(x + 1, max.x);
            min.x = std::min(x - 1, min.x);

            for (auto& y_iter : x_iter.second) {
                int y = y_iter.first;

                max.y = std::max(y + 1, max.y);
                min.y = std::min(y - 1, min.y);

                for (int z : y_iter.second) {
                    max.z = std::max(z + 1, max.z);
                    min.z = std::min(z - 1, min.z);
                }
            }
        }

        int surface = 0;

        std::queue<position_t> q;
        q.push(max);

        space_t visited;
        visited[max.x][max.y].insert(max.z);

        while (!q.empty()) {
            auto cur = q.front();
            q.pop();

            for (const auto& d : dirs) {
                position_t nei = { cur.x + d.x, cur.y + d.y, cur.z + d.z };
                if (nei.x < min.x || nei.x > max.x || nei.y < min.y || nei.y > max.y || nei.z < min.z || nei.z > max.z) {
                    continue;
                }

                if (is_occupied(visited, nei.x, nei.y, nei.z)) {
                    continue;
                }

                if (is_occupied(sp, nei.x, nei.y, nei.z)) {
                    surface++;
                    continue;   /* never mask figure as visited */
                }

                visited[nei.x][nei.y].insert(nei.z);
                q.push(nei);
            }
        }

        return surface;
    }

private:
    bool is_occupied(const space_t& collection, int x, int y, int z) {
        auto x_iter = collection.find(x);
        if (x_iter == collection.cend()) {
            return false;
        }

        auto y_iter = x_iter->second.find(y);
        if (y_iter == x_iter->second.cend()) {
            return false;
        }

        auto z_iter = y_iter->second.find(z);
        return (z_iter != y_iter->second.cend());
    }
};


int main() {
    auto pair = read_input();

    std::cout << "The surface area: " << solution(pair.first, pair.second).surface_area() << std::endl;
    std::cout << "The external surface area: " << solution(pair.first, pair.second).extrnal_surface_area() << std::endl;

    return 0;
}
