#include <vector>
#include <string>

#include <iostream>


const std::string DIRECTIONS = "RDLU";


class solution {
public:
    bool is_correct(std::vector<std::string>& p_map, const std::string& p_instructions) {
        int x = 0;
        int y = p_map.size() - 1;
        char direction = 0;

        p_map.back()[0] = '.';

        for (char instruction : p_instructions) {
            switch (instruction) {
            case 'F':
                if (!forward(p_map, direction, x, y)) {
                    return false;
                }

                if ((p_map[y][x] != '.') && (p_map[y][x] != 'D')) {
                    return false;
                }

                break;

            case 'L':
                direction = turn_left(direction);
                break;

            case 'R':
                direction = turn_right(direction);
                break;

            case 'X':
                if (!laser(direction, x, y, p_map)) {
                    return false;
                }

                break;
            }
        }

        return p_map[y][x] == 'D';
    }

private:
    bool laser(const char direction, const int x, const int y, std::vector<std::string>& p_map) {
        int lx = x;
        int ly = y;

        switch (DIRECTIONS[direction]) {
        case 'R':
            lx++;
            break;

        case 'L':
            lx--;
            break;

        case 'U':
            ly--;
            break;

        case 'D':
            ly++;
            break;
        }

        if (lx >= 0 && ly >= 0 && lx < p_map[0].size() && ly < p_map.size()) {
            if (p_map[ly][lx] == 'I') {
                p_map[ly][lx] = '.';
                return true;
            }
            
            return false;
        }

        return false;
    }

    char turn_left(const char direction) {
        return (direction - 1) % DIRECTIONS.size();
    }

    char turn_right(const char direction) {
        return (direction + 1) % DIRECTIONS.size();
    }

    bool forward(const std::vector<std::string>& p_map, const char direction, int& x, int& y) {
        switch (DIRECTIONS[direction]) {
        case 'R':
            x++;
            return (x < p_map[0].size());

        case 'U':
            y--;
            return (y >= 0);

        case 'L':
            x--;
            return (x >= 0);

        case 'D':
            y++;
            return (y < p_map.size());
        }

        return false;
    }
};


int main() {
    std::vector<std::string> map;
    for (int i = 0; i < 8; i++) {
        std::string line;
        std::cin >> line;

        map.push_back(line);
    }

    std::string instructions;
    std::cin >> instructions;

    bool result = solution().is_correct(map, instructions);
    std::cout << (result ? "Diamond!" : "Bug!") << std::endl;

    return 0;
}
