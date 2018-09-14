#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>
#include <string>


std::size_t hoare_partition(std::vector<std::size_t> & p_array, const std::size_t p_begin, const std::size_t p_anchor) {
    std::size_t lborder = p_begin - 1;
    std::size_t rborder = p_anchor + 1;
    std::size_t anchor = p_array[p_anchor];

    while (true) {
        while (p_array[++lborder] < anchor);
        while (p_array[--rborder] > anchor);
  
        if (lborder < rborder) {
            std::swap(p_array[lborder], p_array[rborder]);
        }
        else {
            return lborder;
        }
    }
}


void quicksort(std::vector<std::size_t> & p_array, const std::size_t p_begin, const std::size_t p_anchor) {
    if (p_begin < p_anchor && p_anchor != (std::size_t) -1) {
        std::size_t border = hoare_partition(p_array, p_begin, p_anchor);
        quicksort(p_array, p_begin, border - 1);
        quicksort(p_array, border, p_anchor);
    }
}


void test_sort(const std::vector<std::size_t> & p_array) {
    auto actual   = p_array;
    auto expected = p_array;

    std::sort(expected.begin(), expected.end());
    quicksort(actual, 0, actual.size() - 1);

    if (actual != expected) {
        std::cout << "[FAIL]" << std::endl;
    }
    else {
        std::cout << "[PASS]" << std::endl;
    }
}


std::vector<std::vector<std::size_t>> g_samples = {

  { 3, 2, 6, 8, 1, 4 },
  { 2, 0, 6, 5, 8, 3, 6, 1, 1 },
  { 8, 8, 4 },
  { 1, 2, 3, 4, 5, 6, 7 },
  { 8, 7, 6, 5, 4 },
  { 0, 0, 0, 0, 0, 0 },
  { 1, 1, 1, 1, 1 },
  { 5, 4, 2, 0, 0, 2, 3, 4, 5, 6, 1, 2, 3, 4, 1, 2, 3, 4, 5, 6 },
  { 1, 10, 100, 1000 },
  { 2, 2, 2, 2, 2, 2, 3 },
  { 3, 2, 2, 2, 2, 2, 2 }

};



int main(int argc, char *argv[]) {
    for (auto & ar : g_samples)
      test_sort(ar);

    getchar();
    return 0;
}