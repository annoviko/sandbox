#include <vector>
#include <string>


const std::string cmd_row = "row";
const std::string cmd_col = "col";
const std::string cmd_inc = "inc";
const std::string cmd_dec = "dec";
const std::string cmd_transpose = "transpose";


struct command {
    std::string name;
    std::vector<std::size_t> arguments;
};


using matrix = std::vector<std::vector<int>>;


class solution {
public:
    matrix handle(const matrix& p_matrix, const std::vector<command>& p_instructions) {
        matrix result = p_matrix;

        for (auto& cmd : p_instructions) {
            if (cmd.name == cmd_row) {
                std::swap(result[cmd.arguments[0]], result[cmd.arguments[1]]);
            }
            else if (cmd.name == cmd_col) {
                for (std::size_t i = 0; i < result.size(); i++) {
                    std::swap(result[i][cmd.arguments[0]], result[i][cmd.arguments[1]]);
                }
            }
            else if (cmd.name == cmd_inc) {
                for (std::size_t i = 0; i < result.size(); i++) {
                    for (std::size_t j = 0; j < result.size(); j++) {
                        result[i][j] = (result[i][j] + 1) % 10;
                    }
                }
            }
            else if (cmd.name == cmd_dec) {
                for (std::size_t i = 0; i < result.size(); i++) {
                    for (std::size_t j = 0; j < result.size(); j++) {
                        result[i][j] = (result[i][j] > 0) ? result[i][j] - 1 : 9;
                    }
                }
            }
            else if (cmd.name == cmd_transpose) {
                for (std::size_t i = 0; i < result.size(); i++) {
                    for (std::size_t j = i + 1; j < result.size(); j++) {
                        std::swap(result[i][j], result[j][i]);
                    }
                }
            }
        }

        return result;
    }
};


#include <iostream>

std::ostream& operator<<(std::ostream& p_stream, const matrix& p_matrix) {
    for (std::size_t i = 0; i < p_matrix.size(); i++) {
        for (auto val : p_matrix[i]) {
            p_stream << val;
        }

        p_stream << std::endl;
    }

    return p_stream;
}


int main() {
    int cases = 0;
    std::cin >> cases;

    for (int c = 0; c < cases; c++) {
        int n = 0;
        std::cin >> n;

        matrix mtrx;
        for (int i = 0; i < n; i++) {
            std::string str;
            std::cin >> str;

            std::vector<int> row;
            for (auto sym : str) {
                row.push_back(sym - '0');
            }

            mtrx.push_back(row);
        }

        std::vector<command> commands;
        std::cin >> n;

        for (int i = 0; i < n; i++) {
            command cmd;
            std::cin >> cmd.name;

            if (cmd.name == cmd_row || cmd.name == cmd_col) {
                std::size_t arg;

                std::cin >> arg;
                cmd.arguments.push_back(arg - 1);

                std::cin >> arg;
                cmd.arguments.push_back(arg - 1);
            }

            commands.push_back(cmd);
        }

        matrix result = solution().handle(mtrx, commands);

        std::cout << "Case #" << c + 1 << std::endl;
        std::cout << result;
        std::cout << std::endl;
    }
}