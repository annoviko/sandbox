#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


using forest = std::vector<std::string>;


forest read() {
    std::ifstream stream("input.txt");

    forest result;
    for (std::string line; std::getline(stream, line);) {
        result.push_back(line);
    }

    return result;
}


class high_tree_finder {
private:
    forest m_forest;
    int m_visible_trees = 0;

public:
    high_tree_finder(const forest& p_forest) :
        m_forest(p_forest),
        m_visible_trees(0)
    { }

public:
    int count() {
        std::vector<std::vector<bool>> visible(m_forest.size(), std::vector<bool>(m_forest[0].size(), false));

        std::vector<int> max_from_top(m_forest[0].size(), -1);
        std::vector<int> max_from_bottom(m_forest[0].size(), -1);

        const std::size_t row_size = m_forest.size();
        const std::size_t col_size = m_forest[0].size();

        for (std::size_t i = 0; i < row_size; i++) {
            int max_from_left = -1;
            int max_from_right = -1;

            for (std::size_t j = 0; j < col_size; j++) {
                if (max_from_left < m_forest[i][j]) {
                    visible[i][j] = true;
                    max_from_left = m_forest[i][j];
                }

                if (max_from_right < m_forest[i][col_size - 1 - j]) {
                    visible[i][m_forest.size() - 1 - j] = true;
                    max_from_right = m_forest[i][col_size - 1 - j];
                }

                if (max_from_top[j] < m_forest[i][j]) {
                    visible[i][j] = true;
                    max_from_top[j] = m_forest[i][j];
                }

                if (max_from_bottom[j] < m_forest[row_size - 1 - i][j]) {
                    visible[row_size - 1 - i][j] = true;
                    max_from_bottom[j] = m_forest[row_size - 1 - i][j];
                }
            }
        }

        for (const auto& row : visible) {
            for (const bool is_tree_visible : row) {
                if (is_tree_visible) {
                    m_visible_trees++;
                }
            }
        }

        return m_visible_trees;
    }
};


class tree_visibility_analyser {
private:
    forest m_forest;
    int m_best_score = 0;

public:
    tree_visibility_analyser(const forest& p_forest) :
        m_forest(p_forest),
        m_best_score(0)
    { }

public:
    int max_visibility_score() {
        for (int i = 0; i < m_forest.size(); i++) {
            for (int j = 0; j < m_forest[0].size(); j++) {
                m_best_score = std::max(m_best_score, get_score(i, j));
            }
        }

        return m_best_score;
    }

private:
    int get_score(int row, int col) {
        if ((row == 0) || (row == m_forest.size() - 1) || (col == 0) || (col == m_forest[0].size() - 1)) {
            return 0;
        }

        int score = get_down(row, col) * get_up(row, col) * get_right(row, col) * get_left(row, col);
        return score;
    }


    int get_down(int row, int col) {
        int visible_trees = 0;

        for (int i = row + 1; i < m_forest.size(); i++) {
            visible_trees++;
            if (m_forest[i][col] >= m_forest[row][col]) {
                break;
            }
        }

        return visible_trees;
    }


    int get_up(int row, int col) {
        int visible_trees = 0;

        for (int i = row - 1; i >= 0; i--) {
            visible_trees++;
            if (m_forest[i][col] >= m_forest[row][col]) {
                break;
            }
        }

        return visible_trees;
    }


    int get_right(int row, int col) {
        int visible_trees = 0;

        for (int i = col + 1; i < m_forest[0].size(); i++) {
            visible_trees++;
            if (m_forest[row][i] >= m_forest[row][col]) {
                break;
            }
        }

        return visible_trees;
    }


    int get_left(int row, int col) {
        int visible_trees = 0;

        for (int i = col - 1; i >= 0; i--) {
            visible_trees++;
            if (m_forest[row][i] >= m_forest[row][col]) {
                break;
            }
        }

        return visible_trees;
    }
};


int main() {
    auto input_forest = read();
    std::cout << "The number of visible trees: " << high_tree_finder(input_forest).count() << std::endl;
    std::cout << "The highest scenic score: " << tree_visibility_analyser(input_forest).max_visibility_score() << std::endl;

    return 0;
}