#include <iostream>
#include <fstream>
#include <string>
#include <vector>


using landscape_map = std::vector<std::string>;


struct position {
    std::size_t x = 0;
    std::size_t y = 0;
};


landscape_map read_map() {
    std::ifstream stream("input.txt");

    landscape_map result;
    for (std::string line; std::getline(stream, line); ) {
        result.push_back(line);
    }

    return result;
}


std::size_t count_trees(const landscape_map & p_map, const position & p_pos, const position & p_move) {
    std::size_t trees = 0;

    const std::size_t x_len = p_map[0].size();
    for (std::size_t y = p_pos.y, x = p_pos.x; y < p_map.size(); y += p_move.y, x += p_move.x) {
        if (x >= x_len) {
            x %= x_len;
        }

        if (p_map[y][x] == '#') {
            trees++;
        }
    }

    return trees;
}


std::size_t multiply_trees_on_slopes(const landscape_map & p_map) {
    std::size_t result = count_trees(p_map, { 0, 0 }, { 1, 1 });

    const std::vector<position> slopes = { { 3, 1 }, { 5, 1 }, { 7, 1 }, { 1, 2 } };
    for (const auto & slope : slopes) {
        result *= count_trees(p_map, { 0, 0 }, slope);
    }

    return result;
}



int main() {
    std::cout << "The amount of trees to encounter: " << count_trees(read_map(), { 0, 0 }, { 3, 1 }) << std::endl;
    std::cout << "The multiplication of trees on the slopes: " << multiply_trees_on_slopes(read_map()) << std::endl;
    return 0;
}
