#include <fstream>
#include <iostream>
#include <string>
#include <vector>


using paper_content = std::vector<std::string>;


struct position {
    std::uint64_t x = 0;
    std::uint64_t y = 0;
};


enum class fold_t {
    x, y
};


struct fold_instruction {
    fold_t fold = fold_t::x;
    std::uint64_t position = 0;
};


struct instruction {
    std::vector<position> dots;
    std::vector<fold_instruction> folds;
};


class paper {
private:
    paper_content m_content;
    std::vector<fold_instruction> m_folds;

private:
    void extract_width_height(const instruction& p_instruction, std::uint64_t& p_width, std::uint64_t& p_height) const {
        p_width = 0; p_height = 0;

        for (const auto& coordinate: p_instruction.dots) {
            p_width = std::max(p_width, coordinate.x + 1);
            p_height = std::max(p_height, coordinate.y + 1);
        }
    }


    void initialize_content(const instruction& p_instruction) {
        std::uint64_t width = 0, height = 0;
        extract_width_height(p_instruction, width, height);

        m_content = paper_content(height, std::string(width, '.'));
        for (const auto& coordinate : p_instruction.dots) {
            m_content[coordinate.y][coordinate.x] = '#';
        }
    }


    void assert_folding(const std::uint64_t p_length, const std::uint64_t p_position) const {
        if (p_length % 2 == 0) {
            throw std::exception("impossible to fold paper");
        }

        const std::uint64_t border = p_length >> 1;
        if (p_position != border) {
            throw std::exception("impossible to fold paper equally");
        }
    }


    std::uint64_t fold_vertical(const std::uint64_t p_position) {
        assert_folding(m_content.size(), p_position);

        std::uint64_t amount_dots = 0;
        paper_content new_content(p_position, std::string(m_content[0].size(), ' '));

        for (std::size_t i = 0; i < p_position; i++) {
            for (std::size_t j = 0; j < m_content[0].size(); j++) {
                if ((m_content[i][j] == '#') || (m_content[m_content.size() - i - 1][j] == '#')) {
                    new_content[i][j] = '#';
                    amount_dots++;
                }
                else {
                    new_content[i][j] = '.';
                }
            }
        }

        m_content = std::move(new_content);
        return amount_dots;
    }


    std::uint64_t fold_horizontal(const std::uint64_t p_position) {
        assert_folding(m_content[0].size(), p_position);

        std::uint64_t amount_dots = 0;
        paper_content new_content(m_content.size(), std::string(p_position, ' '));

        for (std::size_t i = 0; i < m_content.size(); i++) {
            for (std::size_t j = 0; j < p_position; j++) {
                if ((m_content[i][j] == '#') || (m_content[i][m_content[0].size() - j - 1] == '#')) {
                    new_content[i][j] = '#';
                    amount_dots++;
                }
                else {
                    new_content[i][j] = '.';
                }
            }
        }

        m_content = std::move(new_content);
        return amount_dots;
    }


    std::size_t fold_paper(const fold_instruction& p_instruction) {
        switch(p_instruction.fold) {
            case fold_t::x: return fold_horizontal(p_instruction.position);
            case fold_t::y: return fold_vertical(p_instruction.position);
            default: throw std::exception("unknown fold type");
        }
    }


    void display() {
        for (const auto & row : m_content) {
            std::cout << row << std::endl;
        }
    }


public:
    paper(const instruction& p_instruction) : m_folds(p_instruction.folds) {
        initialize_content(p_instruction);
    }


public:
    std::size_t single_fold_test() {
        return fold_paper(m_folds.front());
    }


    void decode_and_display() {
        for (const auto& fold : m_folds) {
            fold_paper(fold);
        }

        display();
    }
};


instruction read_input() {
    std::ifstream stream("input.txt");

    instruction result;
    for (std::string str_row; std::getline(stream, str_row);) {
        if (str_row.empty()) { break; }

        const std::size_t split_position = str_row.find(',');

        position dot;
        dot.x = std::stoull(str_row.substr(0, split_position));
        dot.y = std::stoull(str_row.substr(split_position + 1));

        result.dots.push_back(dot);
    }

    for (std::string str_row; std::getline(stream, str_row);) {
        const std::size_t split_position = str_row.find('=');

        fold_instruction action;
        action.fold = (str_row[split_position - 1] == 'x') ? fold_t::x : fold_t::y;
        action.position = std::stoull(str_row.substr(split_position + 1));

        result.folds.push_back(action);
    }

    return result;
}


int main() {
    const instruction input = read_input();

    std::cout << "The amount of dots after the first instruction: " << paper(input).single_fold_test() << std::endl;
    std::cout << "The decoded eight capital letters: " << std::endl;
    paper(input).decode_and_display();

    return 0;
}
