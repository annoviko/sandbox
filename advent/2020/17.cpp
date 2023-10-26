#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>


using cube_row = std::unordered_map<int, char>;
using cube_2d_map = std::unordered_map<int, cube_row>;
using cube_3d_map = std::unordered_map<int, cube_2d_map>;
using cube_4d_map = std::unordered_map<int, cube_3d_map>;


cube_3d_map read_3d_map(const std::string & p_file) {
    std::ifstream stream(p_file);

    cube_3d_map result;
    std::size_t x = 0;
    for (std::string line; std::getline(stream, line); ) {
        for (std::size_t y = 0; y < line.size(); y++) {
            result[x][y][0] = line[y];
        }

        x++;
    }

    return result;
}


cube_4d_map read_4d_map(const std::string & p_file) {
    std::ifstream stream(p_file);

    cube_4d_map result;
    std::size_t x = 0;
    for (std::string line; std::getline(stream, line); ) {
        for (std::size_t y = 0; y < line.size(); y++) {
            result[x][y][0][0] = line[y];
        }

        x++;
    }

    return result;
}


class simulator_3d {
private:
    cube_3d_map m_map;
    int xmin = 0, xmax = 0;
    int ymin = 0, ymax = 0;
    int zmin = 0, zmax = 0;

public:
    simulator_3d(const cube_3d_map & p_map) : m_map(p_map) {
        xmax = m_map.size();
        ymax = m_map[0].size();
        zmax = m_map[0][0].size();
    }

public:
    std::size_t run(const std::size_t p_iterations) {
        for (std::size_t i = 0; i < p_iterations; i++) {
            increase_borders();
            iterate();
        }

        return get_active_cubes();
    }

private:
    void print(const std::size_t p_iteration) {
        std::cout << "After " << p_iteration << " cycle:" << std::endl << std::endl;

        for (int z = zmin; z < zmax; z++) {
            std::cout << "z=" << z << std::endl;
            for (int x = xmin; x < xmax; x++) {
                for (int y = ymin; y < ymax; y++) {
                    std::cout << m_map[x][y][z];
                }
                std::cout << std::endl;
            }
            std::cout << std::endl << std::endl;
        }
    }

    std::size_t get_active_cubes() {
        std::size_t result = 0;

        for (auto x = xmin; x < xmax; x++) {
            for (auto y = ymin; y < ymax; y++) {
                for (auto z = zmin; z < zmax; z++) {
                    if (m_map[x][y][z] == '#') {
                        result++;
                    }
                }
            }
        }

        return result;
    }

    void increase_borders() {
        xmin--; xmax++;
        ymin--; ymax++;
        zmin--; zmax++;
    }

    void iterate() {
        cube_3d_map m_next_map;

        for (int x = xmin; x < xmax; x++) {
            for (int y = ymin; y < ymax; y++) {
                for (int z = zmin; z < zmax; z++) {
                    const std::size_t active_neighbors = count_active_neighbors(x, y, z);
                    char state = m_map[x][y][z];
                    char & next_state = m_next_map[x][y][z];
                    if (state == '#') {
                        if (active_neighbors != 2 && active_neighbors != 3) {
                            next_state = '.';
                        }
                        else {
                            next_state = '#';
                        }
                    }
                    else if (state == '.' || state == 0) {
                        if (active_neighbors == 3) {
                            next_state = '#';
                        }
                        else {
                            next_state = '.';    /* override 0 */
                        }
                    }
                    else {
                        throw std::exception("Unknown state.");
                    }
                }
            }
        }

        m_map = std::move(m_next_map);
    }

    std::size_t count_active_neighbors(const int p_x, const int p_y, const int p_z) {
        static const std::vector<int> delta = { -1, 0, 1 };

        std::size_t result = 0;
        for (auto dx = delta.begin(); dx != delta.end(); dx++) {
            const int x = p_x + *dx;

            for (auto dy = delta.begin(); dy != delta.end(); dy++) {
                const int y = p_y + *dy;

                for (auto dz = delta.begin(); dz != delta.end(); dz++) {
                    const int z = p_z + *dz;

                    if ((x == p_x) && (y == p_y) && (z == p_z)) {
                        continue;
                    }

                    if (m_map[x][y][z] == '#') {
                        result++;
                    }
                }
            }
        }

        return result;
    }
};



class simulator_4d {
private:
    cube_4d_map m_map;
    int xmin = 0, xmax = 0;
    int ymin = 0, ymax = 0;
    int zmin = 0, zmax = 0;
    int wmin = 0, wmax = 0;

public:
    simulator_4d(const cube_4d_map & p_map) : m_map(p_map) {
        xmax = m_map.size();
        ymax = m_map[0].size();
        zmax = m_map[0][0].size();
        wmax = m_map[0][0][0].size();
    }

public:
    std::size_t run(const std::size_t p_iterations) {
        for (std::size_t i = 0; i < p_iterations; i++) {
            increase_borders();
            iterate();
        }

        return get_active_cubes();
    }

private:
    std::size_t get_active_cubes() {
        std::size_t result = 0;

        for (auto x = xmin; x < xmax; x++) {
            for (auto y = ymin; y < ymax; y++) {
                for (auto z = zmin; z < zmax; z++) {
                    for (auto w = wmin; w < wmax; w++) {
                        if (m_map[x][y][z][w] == '#') {
                            result++;
                        }
                    }
                }
            }
        }

        return result;
    }

    void increase_borders() {
        xmin--; xmax++;
        ymin--; ymax++;
        zmin--; zmax++;
        wmin--; wmax++;
    }

    void iterate() {
        cube_4d_map m_next_map;

        for (int x = xmin; x < xmax; x++) {
            for (int y = ymin; y < ymax; y++) {
                for (int z = zmin; z < zmax; z++) {
                    for (int w = wmin; w < wmax; w++) {
                        const std::size_t active_neighbors = count_active_neighbors(x, y, z, w);
                        char state = m_map[x][y][z][w];
                        char & next_state = m_next_map[x][y][z][w];
                        if (state == '#') {
                            if (active_neighbors != 2 && active_neighbors != 3) {
                                next_state = '.';
                            }
                            else {
                                next_state = '#';
                            }
                        }
                        else if (state == '.' || state == 0) {
                            if (active_neighbors == 3) {
                                next_state = '#';
                            }
                            else {
                                next_state = '.';    /* override 0 */
                            }
                        }
                        else {
                            throw std::exception("Unknown state.");
                        }
                    }
                }
            }
        }

        m_map = std::move(m_next_map);
    }

    std::size_t count_active_neighbors(const int p_x, const int p_y, const int p_z, const int p_w) {
        static const std::vector<int> delta = { -1, 0, 1 };

        std::size_t result = 0;
        for (auto dx = delta.begin(); dx != delta.end(); dx++) {
            const int x = p_x + *dx;

            for (auto dy = delta.begin(); dy != delta.end(); dy++) {
                const int y = p_y + *dy;

                for (auto dz = delta.begin(); dz != delta.end(); dz++) {
                    const int z = p_z + *dz;

                    for (auto dw = delta.begin(); dw != delta.end(); dw++) {
                        const int w = p_w + *dw;

                        if ((x == p_x) && (y == p_y) && (z == p_z) && (w == p_w)) {
                            continue;
                        }

                        if (m_map[x][y][z][w] == '#') {
                            result++;

                            if (result > 3) {
                                return result;  /* already out of rules */
                            }
                        }
                    }
                }
            }
        }

        return result;
    }
};


int main() {
    auto map_3d = read_3d_map("input.txt");

    std::cout << "The amount of active cubes: " << simulator_3d(map_3d).run(6) << std::endl;

    auto map_4d = read_4d_map("input.txt");

    std::cout << "The amount of active hypercubes: " << simulator_4d(map_4d).run(6) << std::endl;

    return 0;
}
