#include <algorithm>
#include <vector>

#include "header.h"

std::vector<std::vector<std::size_t>> arrs = {
          { 2, 4, 5, 6, 1, 2, 8, 4 },
          { 2, 5, 6, 10, 34, 2, 0 },
          { 1, 1, 10, 54, 2, 3, 8 },
          { 4, 5, 6, 7, 8, 9 },
          { 9, 8, 7, 6, 5, 4, 3 },
          { 1, 1, 1, 1, 1, 1, 1, 1 } };


void test_sort(const std::vector<std::size_t> & p_array) {
    auto actual = p_array;
    auto expected = p_array;

    std::sort(expected.begin(), expected.end());
    ch07::quicksort(actual, std::size_t(0), actual.size() - 1);

    ch07::assert_eq(expected, actual);
}


int main(int argc, char *argv[]) {
    for (auto & ar : arrs) {
        test_sort(ar);
    }

    std::cout << "Press any key to finish...";
    getchar();
    return 0;
}