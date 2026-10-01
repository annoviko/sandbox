#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <conio.h>


struct position_t {
    int r = 0;
    int c = 0;
};

/*
 012345
0 #
1###
2 #
3
4
5

*/

const std::vector<std::vector<position_t>> FIGURES = {
    { { 0, 0 }, { 0, 1 }, { 0, 2 }, { 0, 3 } },
    { { 0, 1 }, { 1, 0 }, { 1, 1 }, { 1, 2 }, { 2, 1 } },
    { { 0, 2 }, { 1, 2 }, { 2, 2 }, { 2, 1 }, { 2, 0 } },
    { { 0, 0 }, { 1, 0 }, { 2, 0 }, { 3, 0 } },
    { { 0, 0 }, { 0, 1 }, { 1, 0 }, { 1, 1 } }
};


const std::vector<int> HEIGHTS = { 1, 3, 3, 4, 2 };


std::string read_input() {
    std::ifstream stream("input.txt");
    
    std::string jets;
    std::getline(stream, jets);

    return jets;
}


class solution {
private:
    std::string jets;
    std::vector<std::vector<bool>> field;

    int height = 0;

public:
    solution(const std::string& j) : jets(j) { }

    int tower_height(int n) {
        int iteration = 0;

        for (int i = 0; i < n; i++) {
            const int index_figure = i % FIGURES.size();
            iteration = simulate(index_figure, iteration);
        }

        return height;
    }

private:
    int simulate(int index, int iteration) {
        int height_to_add = height + (3 + HEIGHTS[index]) - field.size();
        for (int i = 0; i < height_to_add; i++) {
            field.push_back(std::vector<bool>(7, false));
        }

        const int height_to_start = height + 3 + HEIGHTS[index];
        const int dr = field.size() - height_to_start;
        const int dc = 2;

        /* place figure to initial position */
        std::vector<position_t> coord = FIGURES[index];
        for (int i = 0; i < coord.size(); i++) {
            coord[i].c += dc;
            coord[i].r = (field.size() - 1) - coord[i].r - dr;
        }

        //visualize(coord);

        int previous_row = -1;
        while (previous_row != coord[0].r) {
            previous_row = coord[0].r;

            int dc = get_dc(iteration);

            move_figure_by_jet(dc, coord);
            //visualize(coord);

            move_figure_by_gravity(coord);
            //visualize(coord);

            iteration++;
        }

        int new_height = coord[0].r + 1;    /* 0 - the top of the figure */
        height = std::max(height, new_height);

        for (const auto& p : coord) {
            field[p.r][p.c] = true;
        }

        //visualize({});

        return iteration;
    }

    int get_dc(int iteration) {
        int jet_index = iteration % jets.size();
        char jet = jets[jet_index];

        if (jet == '<') {
            return -1;
        }
        
        return 1;
    }

    void move_figure_by_jet(int dc, std::vector<position_t>& p) {
        move_figure({ 0, dc }, p);
    }

    void move_figure_by_gravity(std::vector<position_t>& p) {
        move_figure({ -1, 0 }, p);
    }

    void move_figure(const position_t& d, std::vector<position_t>& p) {
        bool success = true;
        std::vector<position_t> next_coord = p;
        for (int i = 0; i < next_coord.size(); i++) {
            int row = next_coord[i].r + d.r;
            int col = next_coord[i].c + d.c;

            if ((col < 0) || (col >= field[0].size()) || (row < 0) || (field[row][col] != false)) {
                success = false;
                break;
            }

            next_coord[i].r = row;
            next_coord[i].c = col;
        }

        if (success) {
            p = next_coord;
        }
    }

    void visualize(const std::vector<position_t>& figure) {
        system("cls");
        std::cout << "Height: " << height << "\n\n";

        for (int i = field.size() - 1; i >= 0; i--) {
            std::cout << '|';
            for (int j = 0; j < field[i].size(); j++) {
                bool active_figure = false;
                for (const auto& p : figure) {
                    if (p.r == i && p.c == j) {
                        active_figure = true;
                        std::cout << '@';
                        break;
                    }
                }

                if (!active_figure) {
                    std::cout << (field[i][j] ? '#' : '.');
                }
            }
            std::cout << '|' << std::endl;
        }

        std::cout << "+-------+" << std::endl;
        _getch();
    }
};


int main() {
    auto jets = read_input();

    std::cout << "The height of the tower of rocks: " << solution(jets).tower_height(2022) << std::endl;

    return 0;
}