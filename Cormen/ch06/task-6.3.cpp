#include <iostream>
#include <iomanip>
#include <numeric>
#include <vector>
#include <stack>
#include <limits>
#include <cmath>


using matrix = std::vector<std::vector<std::size_t>>;

const static std::size_t INF = std::numeric_limits<std::size_t>::max();


matrix test_matrix1 = {
    { 1, 2, 3 },
    { 4, 5, 6 },
    { 7, 8, 9 }
};


matrix test_matrix2 = {
    { 2, 4, 6 },
    { 2, 7, 8 },
    { 6, 7, 9 }
};


matrix test_matrix3 = {
    {   2,   4,   6 },
    {   9,  12,  15 },
    {  23, INF, INF }
};


matrix test_matrix4 = {
    { 1, 4, 9 },
    { 2, 5, 10 },
    { 8, 9, 11 }
};


struct place {
    std::size_t x;
    std::size_t y;

    bool operator==(const place & p_other) {
        return (p_other.x == x && p_other.y == y);
    }

    bool operator!=(const place & p_other) {
        return !(*this == p_other);
    }

    bool in_range(const matrix & p_mtrx) {
        return (x < p_mtrx[0].size() && y < p_mtrx.size());
    }
};


void move_max_down(matrix & p_mtrx, const place & p_pos) {
    place min_pos = p_pos;

    place r_pos = { p_pos.x + 1, p_pos.y };
    place l_pos = { p_pos.x, p_pos.y + 1 };

    std::size_t min_val = p_mtrx[p_pos.x][p_pos.y];

    if (l_pos.in_range(p_mtrx) && p_mtrx[l_pos.x][l_pos.y] < min_val) {
        min_pos = l_pos;
        min_val = p_mtrx[l_pos.x][l_pos.y];
    }

    if (r_pos.in_range(p_mtrx) && p_mtrx[r_pos.x][r_pos.y] < min_val) {
        min_pos = r_pos;
    }

    if (min_pos != p_pos) {
        std::swap(p_mtrx[min_pos.x][min_pos.y], p_mtrx[p_pos.x][p_pos.y]);
        move_max_down(p_mtrx, min_pos);
    }
}


void move_max_upper(matrix & p_mtrx, const place & p_pos) {
    place max_pos = p_pos;

    place r_pos = { p_pos.x - 1, p_pos.y };
    place l_pos = { p_pos.x, p_pos.y - 1 };

    std::size_t max_val = p_mtrx[p_pos.x][p_pos.y];

    if (l_pos.in_range(p_mtrx) && p_mtrx[l_pos.x][l_pos.y] > max_val) {
        max_pos = l_pos;
        max_val = p_mtrx[l_pos.x][l_pos.y];
    }

    if (r_pos.in_range(p_mtrx) && p_mtrx[r_pos.x][r_pos.y] > max_val) {
        max_pos = r_pos;
    }

    if (max_pos != p_pos) {
        std::swap(p_mtrx[max_pos.x][max_pos.y], p_mtrx[p_pos.x][p_pos.y]);
        move_max_upper(p_mtrx, max_pos);
    }
}


std::size_t extract_min(matrix & p_mtrx, const std::size_t p_size) {
    const auto min = p_mtrx[0][0];

    if (min != INF) {
        const std::size_t max_row = std::ceil((double) p_size / (double) p_mtrx.size()) - 1;
        const std::size_t max_col = p_size - max_row * p_mtrx.size() - 1;

        p_mtrx[0][0] = p_mtrx[max_row][max_col];
        p_mtrx[max_row][max_col] = INF;

        move_max_down(p_mtrx, { 0, 0 });
    }

    return min;
}


void insert_element(matrix & p_mtrx, const std::size_t p_element, const std::size_t p_size) {
    if (p_size > p_mtrx.size() * p_mtrx[0].size()) {
        return;
    }

    std::size_t free_index = p_size;

    const std::size_t max_row = std::ceil((double) free_index / (double) p_mtrx.size()) - 1;
    const std::size_t max_col = free_index - max_row * p_mtrx.size() - 1;

    p_mtrx[max_row][max_col] = p_element;
    move_max_upper(p_mtrx, { max_row, max_col });
}


bool contain(matrix & p_mtrx, const std::size_t p_elem) {
    place pos = { 0, 0 };

    std::stack<place> to_check;

    do {
        if (p_mtrx[pos.x][pos.y] == p_elem) {
            return true;
        }
        else {
            place l_pos = { pos.x + 1, pos.y };
            place r_pos = { pos.x, pos.y + 1 };

            bool l_check = false;

            place next_pos = pos;
            std::size_t max_elem = p_mtrx[pos.x][pos.y];

            if (l_pos.in_range(p_mtrx) && p_mtrx[l_pos.x][l_pos.y] <= p_elem) {
                next_pos = l_pos;
                max_elem = p_mtrx[l_pos.x][l_pos.y];
                l_check = true;
            }

            if (r_pos.in_range(p_mtrx) && p_mtrx[r_pos.x][r_pos.y] <= p_elem) {
                if (p_mtrx[r_pos.x][r_pos.y] > max_elem) {
                    next_pos = r_pos;

                    if (l_check) {
                        to_check.push(l_pos);
                    }
                }
                else {
                    to_check.push(r_pos);
                }
            }

            if (pos == next_pos) {
                if (to_check.empty()) {
                    break;
                }
                else {
                    next_pos = to_check.top();
                    to_check.pop();
                }
            }

            pos = next_pos;
        }
    }
    while(true);

    return false;
}


std::vector<std::size_t> sort(matrix & p_mtrx) {
    std::vector<std::size_t> result;
    std::size_t length = p_mtrx.size() * p_mtrx[0].size();
    for (std::size_t i = 0; i < length; i++) {
        result.push_back(extract_min(p_mtrx, length - i));
    }

    return result;
}


void print_matrix(const matrix & p_mtrx) {
    for (const auto & row : p_mtrx) {
        for (const auto & elem : row) {
            if (elem == INF)
                std::cout << std::setw(3) << "F";
            else
                std::cout << std::setw(3) << elem;
        }

        std::cout << std::endl;
    }
}


void process_extract(matrix & p_mtrx, const std::size_t p_size) {
    std::cout << "process extract matrix" << std::endl;
    print_matrix(p_mtrx);
    std::cout << std::endl;

    for(std::size_t i = p_size; i > 0; i--) {
        const auto min = extract_min(p_mtrx, i);

        std::cout << "extracted: " << min << std::endl << std::endl;
        print_matrix(p_mtrx);
        std::cout << std::endl;

        //getchar();
    }
}


void process_insert(matrix & p_mtrx, const std::size_t p_val, const std::size_t p_size) {
    std::cout << "process insert matrix" << std::endl;
    print_matrix(p_mtrx);
    std::cout << std::endl;

    insert_element(p_mtrx, p_val, p_size);

    std::cout << "inserted: " << p_val << std::endl << std::endl;
    print_matrix(p_mtrx);
    std::cout << std::endl;

    //getchar();
}


int main(int argc, char *argv[]) {
    process_extract(test_matrix1, test_matrix1.size() * test_matrix1[0].size());
    process_extract(test_matrix2, test_matrix2.size() * test_matrix2[0].size());

    process_insert(test_matrix3, 3, test_matrix1.size() * test_matrix1[0].size() - 1);
    process_insert(test_matrix3, 1, test_matrix1.size() * test_matrix1[0].size());

    std::cout << std::endl << "Find Element" << std::endl;
    std::cout << "T " << contain(test_matrix3, 23) << std::endl;
    std::cout << "F " << contain(test_matrix3, 5) << std::endl;
    std::cout << "T " << contain(test_matrix3, 6) << std::endl;
    std::cout << "T " << contain(test_matrix3, 15) << std::endl;
    std::cout << "T " << contain(test_matrix3, 12) << std::endl;
    std::cout << "T " << contain(test_matrix4, 8) << std::endl;
    std::cout << std::endl;

    std::cout << "Sort Array" << std::endl;
    auto sorted_array = sort(test_matrix4);
    std::cout << "[ ";
    for (const auto & elem : sorted_array) {
        std::cout << elem << " ";
    }
    std::cout << "]";

    getchar();

    return 0;
}