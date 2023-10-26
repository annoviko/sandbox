#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <vector>


using octopus_sequence = std::vector<int>;

using octopus_map = std::vector<octopus_sequence>;


struct position {
    std::uint64_t x = 0;
    std::uint64_t y = 0;
};


using coordinate_collection = std::list<position>;


class dumbo_octopuses {
private:
    octopus_map m_map;

public:
    dumbo_octopuses(const octopus_map& p_map) :
        m_map(p_map)
    { }


private:
    coordinate_collection increase_energy() {
        coordinate_collection octopus_to_flesh;
        for (std::uint64_t i = 0; i < m_map.size(); i++) {
            for (std::uint64_t j = 0; j < m_map[i].size(); j++) {
                m_map[i][j]++;

                if (m_map[i][j] > 9) {
                    octopus_to_flesh.push_back({ j, i });
                }
            }
        }

        return octopus_to_flesh;
    }


    coordinate_collection get_neighbors(const std::uint64_t p_row, const std::uint64_t p_col) {
        coordinate_collection neighbors;

        if ((p_row > 0) && (p_col > 0)) { // upper left
            neighbors.push_back( { p_col - 1, p_row - 1 } );
        }

        if (p_row > 0) { // upper
            neighbors.push_back( { p_col, p_row - 1 } );
        }

        if ((p_row > 0) && (p_col + 1 < m_map[0].size())) { // upper right
            neighbors.push_back( { p_col + 1, p_row - 1 } );
        }

        if (p_col + 1 < m_map[0].size()) { // right
            neighbors.push_back( { p_col + 1, p_row } );
        }

        if ((p_col + 1 < m_map[0].size()) && (p_row + 1 < m_map.size())) { // lower right
            neighbors.push_back( { p_col + 1, p_row + 1 } );
        }

        if (p_row + 1 < m_map.size()) { // lower
            neighbors.push_back( { p_col, p_row + 1 } );
        }

        if ((p_row + 1 < m_map.size()) && (p_col > 0)) { // lower left
            neighbors.push_back( { p_col - 1, p_row + 1 } );
        }

        if (p_col > 0) { // left
            neighbors.push_back( { p_col - 1, p_row } );
        }

        return neighbors;
    }


    std::uint64_t flash_octopus(coordinate_collection& p_octopus_to_flash) {
        std::uint64_t flashes = 0;

        octopus_map flashed_octopuses(m_map.size(), octopus_sequence(m_map[0].size(), 0));
        for (const auto& octopus_position : p_octopus_to_flash) {
            flashed_octopuses[octopus_position.y][octopus_position.x]++;
        }

        while (!p_octopus_to_flash.empty()) {
            flashes++;

            const position octopus_position = p_octopus_to_flash.front();
            p_octopus_to_flash.pop_front();

            m_map[octopus_position.y][octopus_position.x] = 0;

            const auto neighbors = get_neighbors(octopus_position.y, octopus_position.x);
            for (const auto& neighbor_position : neighbors) {
                if (flashed_octopuses[neighbor_position.y][neighbor_position.x] == 0) {
                    m_map[neighbor_position.y][neighbor_position.x]++;

                    if (m_map[neighbor_position.y][neighbor_position.x] > 9) {
                        p_octopus_to_flash.push_back(neighbor_position);
                        flashed_octopuses[neighbor_position.y][neighbor_position.x]++;
                    }
                }
            }
        }

        return flashes;
    }


    std::uint64_t simulate_step() {
        coordinate_collection octopus_to_flesh = increase_energy();
        return flash_octopus(octopus_to_flesh);
    }


    void print(const std::uint64_t p_step) {
        std::cout << "STEP " << p_step << std::endl;
        for (const auto& row : m_map) {
            for (const auto value : row) {
                std::cout << value;
            }

            std::cout << std::endl;
        }

        std::cout << std::endl;
    }


public:
    std::uint64_t simulate(const std::uint64_t p_steps) {
        std::uint64_t flashes = 0;

        for (std::size_t i = 0; i < p_steps; i++) {
            flashes += simulate_step();
        }

        return flashes;
    }

    std::uint64_t find_global_sync() {
        const std::uint64_t octopus_amount = m_map.size() * m_map[0].size();
        std::uint64_t active_octopuses = 0;

        std::uint64_t step = 0;
        while (active_octopuses != octopus_amount) {
            active_octopuses = simulate_step();
            step++;
        }

        return step;
    }
};


octopus_map read_input() {
    std::ifstream stream("input.txt");

    octopus_map result;

    for (std::string str_row; std::getline(stream, str_row);) {
        octopus_sequence sequence;
        for (const char digit : str_row) {
            sequence.push_back(digit - '0');
        }

        result.push_back(sequence);
    }

    return result;
}


int main() {
    const octopus_map input = read_input();

    std::cout << "The amount of flashes after 100 steps: " << dumbo_octopuses(input).simulate(100) << std::endl;
    std::cout << "The step when all octopuses are active: " << dumbo_octopuses(input).find_global_sync() << std::endl;

    return 0;
}

