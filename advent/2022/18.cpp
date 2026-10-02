#include <iostream>
#include <fstream>
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

    int xmax = 0, ymax = 0, zmax = 0;
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
                        if (is_occupied(x + dir.x, y + dir.y, z + dir.z)) {
                            surface--;
                        }
                    }
                }
            }
        }

        return surface;
    }

private:
    bool is_occupied(int x, int y, int z) {
        auto x_iter = sp.find(x);
        if (x_iter == sp.cend()) {
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

    return 0;
}
