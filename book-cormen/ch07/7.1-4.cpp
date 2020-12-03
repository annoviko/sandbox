#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>

std::vector<std::vector<std::size_t>> arrs = {
          { 2, 4, 5, 6, 1, 2, 8, 4 },
          { 2, 5, 6, 10, 34, 2, 0 },
          { 1, 1, 10, 54, 2, 3, 8 },
          { 4, 5, 6, 7, 8, 9 },
          { 9, 8, 7, 6, 5, 4, 3 },
          { 1, 1, 1, 1, 1, 1, 1, 1 },
          { 2, 2, 2, 1, 1, 1, 2, 2 },
          { 1, 2, 3, 2, 0, 1, 2, 0, 2, 1, 4, 2 },
          { 0, 23, 45, 76, 32, 56, 23, 12, 5, 7, 2, 10, 2, 0, 12, 2} };


void assert_eq(const std::vector<std::size_t> & p_expected, const std::vector<std::size_t> & p_actual) {
    if (p_expected != p_actual) {
        std::cout << "[FAIL]" << std::endl;
    }
    else {
        std::cout << "[PASS]" << std::endl;
    }
}


std::size_t partition(std::vector<std::size_t> & p_array, const std::size_t p_begin, const std::size_t p_anchor) {
    auto lborder = p_begin - 1;
    for (auto rborder = p_begin; rborder < p_anchor; rborder++) {
        if (p_array[rborder] > p_array[p_anchor]) {
            std::swap(p_array[++lborder], p_array[rborder]);
        }
    }

    std::swap(p_array[++lborder], p_array[p_anchor]);
    return lborder;
}


void quicksort(std::vector<std::size_t> & p_array, const std::size_t p_begin, const std::size_t p_anchor) {
    if (p_begin < p_anchor && p_anchor != (std::size_t) -1) {
        auto border = partition(p_array, p_begin, p_anchor);
        quicksort(p_array, p_begin, border - 1);
        quicksort(p_array, border + 1, p_anchor);
    }
}


void test_sort(const std::vector<std::size_t> & p_array) {
    auto actual = p_array;
    auto expected = p_array;

    std::sort(expected.begin(), expected.end(), std::greater<std::size_t>());
    quicksort(actual, 0, actual.size() - 1);

    assert_eq(expected, actual);
}


int main(int argc, char *argv[]) {
    for (auto & ar : arrs) {
        test_sort(ar);
    }

    std::cout << "Press any key...";
    getchar();
    return 0;
}