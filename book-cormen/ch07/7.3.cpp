#include <algorithm>
#include <iostream>
#include <vector>


void test_partition(const std::vector<std::size_t> & p_array);


std::ostream & operator<<(std::ostream & p_stream, const std::vector<std::size_t> & p_array) {
    p_stream << "[ ";
    for (const auto & elem : p_array)
        p_stream << elem << " ";
    p_stream << "]";
    return p_stream;
}


void stooge_sort(std::vector<std::size_t> & p_array, const std::size_t p_begin, const std::size_t p_end) {
    if (p_array[p_begin] > p_array[p_end])
        std::swap(p_array[p_begin], p_array[p_end]);

    if (p_begin + 1 >= p_end)
        return;

    const std::size_t k = std::floor((p_end - p_begin + 1) / 3);
    stooge_sort(p_array, p_begin, p_end - k);
    stooge_sort(p_array, p_begin + k, p_end);
    stooge_sort(p_array, p_begin, p_end - k);
}


void test_sort(const std::vector<std::size_t> & p_array) {
    auto expected = p_array;
    auto actual = p_array;

    std::sort(expected.begin(), expected.end());
    stooge_sort(actual, 0, actual.size() - 1);

    if (actual != expected) {
        std::cout << "[FAIL] Actual: " << actual << ", Expected: " << expected << std::endl;
    }
    else {
        std::cout << "[PASS] test_sort" << std::endl;
    }
}


std::vector<std::size_t> array1 = { 13, 19, 9, 5, 12, 8, 7, 4, 11, 2, 6, 21 };
std::vector<std::size_t> array2 = { 6, 5, 4, 3, 2 };

std::vector<std::vector<std::size_t>> samples = {
    { 1, 2, 3, 4, 5 },
    { 6, 5, 4, 3, 2 },
    { 1, 2, 3, 5, 3, 2, 5, 8, 9},
    { 2, 6, 8, 3, 3, 3, 1, 0 },
    { 11, 2, 4, 1, 4, 0, 2, 0, 3, 3, 1, 9 },
    { 1, 1, 1, 1, 1 },
    { 1, 1, 1, 1, 1, 1 },
    { 9, 8, 2, 12, 12, 49, 23, 1, 1, 2, 3 },
    { 0, 9, 1 },
    { 1, 1, 1, 1, 2, 2, 2, 3, 3, 4, 4, 4, 3, 3, 2, 2, 2, 2, 1, 1, 1, 1 },
    { 0, 1, 2, 2, 1, 0 }
};

int main()
{
    for (const auto & arr : samples) {
        test_sort(arr);
    }

    getchar();
    return 0;
}

