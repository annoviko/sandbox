#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>


#include <conio.h>


const int FREE = -1;


struct position_t {
    int row;
    int col;
};


struct field_task_t {
    int width;
    int height;
    std::vector<int> figure_counters;
};


struct placement_t {
    int figure_index;
    int placement_index;
};


using figure_t = std::vector<std::string>;
using figure_place_t = std::vector<position_t>;


class solution {
private:
    std::vector<std::vector<figure_place_t>> all_placements;  /* [figure id] -> [all possible placements for the figure]  */
    std::vector<std::vector<std::vector<placement_t>>> cell_to_placements;  /* [row][col] -> [figure index and placement] */

    std::vector<std::vector<int>> occupation;
    std::vector<std::unordered_set<int>> visited_placements;

    std::vector<field_task_t> tasks;
    std::vector<std::vector<std::string>> figures;

public:
    solution(const std::vector<figure_t>& _figures, const std::vector<field_task_t>& _tasks) :
        all_placements(_figures.size()),
        tasks(_tasks),
        figures(_figures)
    {
        for (int i = 0; i < figures.size(); i++) {
            create_figure_placements(i);
        }
    }

public:
    std::uint64_t count_fit_regions() {
        std::uint64_t counter = 0;
        for (int i = 0; i < tasks.size(); i++) {
            create_possible_figures_on_field(tasks[i]);
            occupation = std::vector<std::vector<int>>(cell_to_placements.size(), std::vector<int>(cell_to_placements[0].size(), FREE));

            int index_figure = 0;
            for (; tasks[i].figure_counters[index_figure] == 0; index_figure++);

            if (is_fit(tasks[i].figure_counters[index_figure], index_figure, i)) {
                counter++;
            }

            std::cout << i << " has been processed" << std::endl;
        }

        return counter;
    }

private:
    void print_occupation(int figure, int remaining) {
        std::cout << std::endl;
        std::cout << "Figure '" << figure << "' placed, remaining: '" << remaining << "'" << std::endl;
        for (int i = 0; i < occupation.size(); i++) {
            for (int j = 0; j < occupation[0].size(); j++) {
                std::cout << (occupation[i][j] == FREE ? "." : std::to_string(occupation[i][j]));
            }
            std::cout << std::endl;
        }
    }

    bool is_fit(int remaining, int cur_index, int task_index) {
        if (cur_index >= figures.size()) {
            return true;    /* all figures placed */
        }

        for (int r = 0; r < cell_to_placements.size(); r++) {
            for (int c = 0; c < cell_to_placements[0].size(); c++) {
                if (occupation[r][c] != FREE) {
                    continue;
                }

                /* check if can place current figure here */
                for (const auto& placement : cell_to_placements[r][c]) {
                    if (placement.figure_index != cur_index) {
                        continue;
                    }

                    int index = placement.placement_index;

                    const auto& figure_placement = all_placements[cur_index][index];
                    bool is_placeable = true;
                    for (const position_t& p : figure_placement) {
                        if (occupation[r + p.row][c + p.col] != FREE) {
                            is_placeable = false;
                            break;
                        }
                    }

                    if (!is_placeable) {
                        continue;
                    }

                    /* yes figure can be placed */

                    /* mark it as occupied */
                    for (const position_t& p : figure_placement) {
                        occupation[r + p.row][c + p.col] = cur_index;
                    }

                    int new_remaining = remaining - 1;

                    //print_occupation(cur_index, new_remaining);
                    //(void) _getch();

                    /* try to place others */
                    bool success = false;
                    if (new_remaining > 0) {
                        success = is_fit(new_remaining, cur_index, task_index); /* place the same figure with the same figure */
                    }
                    else {
                        int next_index_figure = cur_index + 1;
                        for (; (next_index_figure < figures.size()) && (tasks[task_index].figure_counters[next_index_figure] == 0); next_index_figure++);

                        if (next_index_figure >= figures.size()) {
                            return true;    /* the last figure placed - return success */
                        }
                        else {
                            new_remaining = tasks[task_index].figure_counters[cur_index + 1];
                            success = is_fit(new_remaining, next_index_figure, task_index);
                        }
                    }

                    if (success) {
                        return true;
                    }

                    /* continue - we need to consider another position or placement here */

                    /* unmark occupation */
                    for (const position_t& p : figure_placement) {
                        occupation[r + p.row][c + p.col] = FREE;
                    }
                }
            }
        }

        return false;
    }

    bool is_figure_placeable(int r, int c, int cur_index, int cur_placement) {
        for (const auto& placement : cell_to_placements[r][c]) {
            if (placement.figure_index != cur_index) {
                continue;
            }

            int index = placement.placement_index;
            const auto& figure_placement = all_placements[cur_index][index];
            for (const position_t& p : figure_placement) {
                if (occupation[p.row][p.col] != FREE) {
                    continue;
                }
            }

            return true;    /* figure can be placed */
        }
    }

    void create_possible_figures_on_field(const field_task_t& task) {
        cell_to_placements = std::vector<std::vector<std::vector<placement_t>>>(task.height, std::vector<std::vector<placement_t>>(task.width));
        occupation = std::vector<std::vector<int>>(task.height, std::vector<int>(task.width, FREE));

        for (int i = 0; i < figures.size(); i++) {
            place_figure_on_field(i);
        }
    }

    void place_figure_on_field(const int figure_index) {
        auto& figure_placements = all_placements[figure_index];

        /* mark placements on a map */
        for (int i = 0; i < figure_placements.size(); i++) {
            for (int r = 0; r < cell_to_placements.size(); r++) {
                for (int c = 0; c < cell_to_placements[r].size(); c++) {
                    if (!is_inside_borders(cell_to_placements.size(), cell_to_placements[r].size(), r, c, figure_placements[i])) {
                        continue;
                    }

                    cell_to_placements[r][c].push_back({ figure_index, i });
                }
            }
        }
    }

    bool is_inside_borders(int max_rows, int max_cols, int row_offset, int col_offset, const figure_place_t& positions) {
        for (const auto& p : positions) {
            if (p.row + row_offset >= max_rows) {
                return false;
            }

            if (p.col + col_offset >= max_cols) {
                return false;
            }
        }

        return true;
    }

    void create_figure_placements(const int figure_index) {
        std::unordered_set<std::string> placed_rotations;
        std::vector<std::string> cur_fig = figures[figure_index];

        auto& figure_placements = all_placements[figure_index];

        for (int i = 0; i < 4; i++) {
            const auto pair = get_figure_description(cur_fig);
            cur_fig = rotate_figure(cur_fig);

            const auto& hash = pair.first;
            const auto& placement = pair.second;

            if (placed_rotations.count(hash) > 0) {
                continue;   /* figure is symetrical */
            }

            placed_rotations.insert(hash);
            figure_placements.push_back(placement);
        }
    }

    std::pair<std::string, figure_place_t> get_figure_description(const std::vector<std::string>& figure) {
        std::string hash;
        figure_place_t placement;

        for (int r = 0; r < figure.size(); r++) {
            for (int c = 0; c < figure[r].size(); c++) {
                if (figure[r][c] == '#') {
                    placement.push_back({ r, c });
                }
            }

            hash.append(figure[r]);
        }

        return { hash, placement };
    }

    std::vector<std::string> rotate_figure(const std::vector<std::string>& figure) {
        std::vector<std::string> rotated_figure(figure[0].size(), std::string(figure.size(), '.'));
        for (int r = 0; r < figure.size(); r++) {
            for (int c = 0; c < figure[0].size(); c++) {
                rotated_figure[rotated_figure.size() - c - 1][r] = figure[r][c];
            }
        }
        return rotated_figure;
    }
};


std::pair<std::vector<figure_t>, std::vector<field_task_t>> read_input() {
    std::vector<figure_t> figures;
    std::vector<field_task_t> tasks;

    std::fstream stream("input.txt");

    std::string line;
    figure_t figure;
    bool is_figure_coming = false;

    /* figures */
    for (; std::getline(stream, line); ) {
        if (line.empty()) {
            is_figure_coming = false;
            if (!figure.empty()) {
                figures.push_back(figure);
                figure.clear();
            }
            continue;
        }

        if (figure.empty() && !is_figure_coming && line.back() != ':') {
            break;  /* fields are coming */
        }
        else if (line.back() == ':') {
            is_figure_coming = true;
            continue;
        }
        else {
            figure.push_back(line);
        }
    }

    /* tasks */
    do {
        std::stringstream ss(line);

        char ignore;
        int width, height;

        ss >> width >> ignore >> height >> ignore;

        std::vector<int> counters;
        for (int v = 0; ss >> v;) {
            counters.push_back(v);
        }

        tasks.push_back({ width, height, counters });
    }
    while (std::getline(stream, line));

    return { figures, tasks };
}


int main() {
    const auto input = read_input();

    auto count = solution(input.first, input.second).count_fit_regions();

    std::cout << "The regions which can fit: " << count << std::endl;

    return 0;
}