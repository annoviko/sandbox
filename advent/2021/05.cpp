#include <cmath>
#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <tuple>
#include <vector>


using vent_map = std::vector<std::vector<std::size_t>>;


struct position {
    std::int64_t x;
    std::int64_t y;
};


struct cloud_stream {
    position begin;
    position end;

public:
    bool is_orthogonal() const {
        return (begin.x == end.x) || (begin.y == end.y);
    }

    bool is_diagonal() const {
        if (is_orthogonal()) {
            return false;
        }

        return (std::abs(begin.x - end.x) == std::abs(begin.y - end.y));
    }

    std::int64_t get_max_x() const {
        return std::max(begin.x, end.x);
    }

    std::int64_t get_max_y() const {
        return std::max(begin.y, end.y);
    }
};


using cloud_collection = std::vector<cloud_stream>;


enum class analyser_strategy {
    orthogonal = 0,
    orthogonal_diagonal = 1
};


class cloud_analyser {
private:
    cloud_collection m_clouds;
    vent_map m_map;

public:
    cloud_analyser(const cloud_collection& p_clouds) : m_clouds(p_clouds) {
        auto [x_max, y_max] = get_max();
        m_map = vent_map(y_max, std::vector<std::size_t>(x_max, 0));
    }

public:
    std::size_t analyse(const analyser_strategy p_strategy) {
        std::size_t intersections = 0;

        for (const auto& cloud : m_clouds) {
            if (cloud.is_orthogonal()) {
                intersections += travers_orthogonal_cloud(cloud);
            }
            else if ((p_strategy == analyser_strategy::orthogonal_diagonal) && cloud.is_diagonal()) {
                intersections += travers_diagonal_cloud(cloud);
            }
        }

        return intersections;
    }

    void show_cloud_map() {
        for (const auto & row : m_map) {
            for (const auto value : row) {
                if (value == 0) { std::cout << '.'; }
                else { std::cout << value; }
            }
            std::cout << std::endl;
        }
    }

private:
    std::size_t travers_orthogonal_cloud(const cloud_stream& p_cloud) {
        std::size_t intersections = 0;

        if (p_cloud.begin.x == p_cloud.end.x) {
            const std::int64_t x = p_cloud.begin.x;
            const std::int64_t from = std::min(p_cloud.begin.y, p_cloud.end.y);
            const std::int64_t to = std::max(p_cloud.begin.y, p_cloud.end.y);

            for (std::int64_t i = from; i <= to; i++) {
                if (m_map[i][x] == 1) {
                    intersections++;
                }

                m_map[i][x]++;
            }
        }
        else if (p_cloud.begin.y == p_cloud.end.y) {
            const std::int64_t y = p_cloud.begin.y;
            const std::int64_t from = std::min(p_cloud.begin.x, p_cloud.end.x);
            const std::int64_t to = std::max(p_cloud.begin.x, p_cloud.end.x);

            for (std::int64_t i = from; i <= to; i++) {
                if (m_map[y][i] == 1) {
                    intersections++;
                }

                m_map[y][i]++;
            }
        }

        return intersections;
    }

    std::size_t travers_diagonal_cloud(const cloud_stream& p_cloud) {
        std::size_t intersections = 0;

        std::int64_t x = p_cloud.begin.x, y = p_cloud.begin.y;
        for (std::int64_t i = 0; i <= std::abs(p_cloud.end.x - p_cloud.begin.x); i++) {
            if (m_map[y][x] == 1) {
                intersections++;
            }

            m_map[y][x]++;

            if (p_cloud.begin.x < p_cloud.end.x) { x++; }
            else { x--; }

            if (p_cloud.begin.y < p_cloud.end.y) { y++; }
            else { y--; }
        }

        return intersections;
    }

    std::tuple<std::int64_t, std::int64_t> get_max() const {
        std::int64_t x_max = 0, y_max = 0;

        for (const auto& cloud : m_clouds) {
            x_max = std::max(x_max, cloud.get_max_x());
            y_max = std::max(y_max, cloud.get_max_y());
        }

        return { x_max + 1, y_max + 1 };
    }
};


cloud_collection read_input() {
    cloud_collection result;

    std::ifstream stream("input.txt");

    for (std::string line; std::getline(stream, line);) {
        std::regex pattern("(\\d+),(\\d+) -> (\\d+),(\\d+)");
        std::smatch groups;

        if(!std::regex_search(line, groups, pattern)) {
            throw std::exception("invalid input file format");
        }

        position begin{ std::stoul(groups[1]), std::stoul(groups[2]) };
        position end{ std::stoul(groups[3]), std::stoul(groups[4]) };

        result.push_back({ begin, end });
    }

    return result;
}


int main() {
    cloud_collection clouds = read_input();

    std::cout << "Number of intersections (orthogonal): " << cloud_analyser(clouds).analyse(analyser_strategy::orthogonal) << std::endl;
    std::cout << "Number of intersections (all): " << cloud_analyser(clouds).analyse(analyser_strategy::orthogonal_diagonal) << std::endl;

    return 0;
}
