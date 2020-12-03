#include <algorithm>
#include <functional>
#include <iostream>
#include <random>
#include <set>
#include <vector>


std::random_device g_device;
std::mt19937 g_generator(g_device());


using sorter = std::function<void(std::vector<std::size_t> &, const std::size_t, const std::size_t)>;


std::size_t partition(std::vector<std::size_t> & p_array, const std::size_t p_begin, const std::size_t p_anchor) {
    auto lborder = p_begin - 1;
    for (std::size_t rborder = p_begin; rborder < p_anchor; rborder++) {
        if (p_array[rborder] <= p_array[p_anchor]) {
            std::swap(p_array[++lborder], p_array[rborder]);
        }
    }

    std::swap(p_array[++lborder], p_array[p_anchor]);
    return lborder;
}


std::size_t randomize_partition(std::vector<std::size_t> & p_array, const std::size_t p_begin, const std::size_t p_anchor) {
    std::uniform_int_distribution<> distribution(p_begin, p_anchor);
    
    std::set<std::size_t> random_values = { };
    for (std::size_t i = 0; i < 3; i++) {
        random_values.insert(distribution(g_generator));
    }
    
    if (random_values.size() > 1)
        random_values.erase(random_values.begin());
    
    const std::size_t median_random_anchor = *(random_values.begin());    

    std::swap(p_array[median_random_anchor], p_array[p_anchor]);
    return partition(p_array, p_begin, p_anchor);
}


void quicksort(std::vector<std::size_t> & p_array, const std::size_t p_begin, const std::size_t p_anchor) {
    if (p_begin < p_anchor && p_anchor != (std::size_t) -1) {
        const std::size_t border = partition(p_array, p_begin, p_anchor);
        quicksort(p_array, p_begin, border - 1);
        quicksort(p_array, border + 1, p_anchor);
    }
}


std::ostream & operator<<(std::ostream & p_stream, std::vector<std::size_t> & p_array) {
    p_stream << "[ ";
    for (const auto & p_elem : p_array) {
        p_stream << p_elem << " ";
    }
    p_stream << "]";
    return p_stream;
}


void test_quicksort(const std::vector<std::size_t> & p_array, sorter & p_sorter) {
    auto expected = p_array;
    auto actual = p_array;

    std::sort(expected.begin(), expected.end());
    p_sorter(actual, 0, actual.size() - 1);

    if (expected != actual) {
        std::cout << "[FAIL] Actual: " << actual << ", Expected: " << expected << std::endl;
    }
    else {
        std::cout << "[PASS]" << std::endl;
    }
}


std::vector<std::vector<std::size_t>> g_samples = {

    { 2, 6, 3, 1, 7, 8, 9, 3, 4, 8, 2, 5, 1 },
    { 0, 0, 0, 1, 2, 8, 4, 3, 7, 8, 3 },
    { 1, 2, 3, 2, 1, 7, 8, 4 },
    { 1, 1, 1, 1, 1, 1 },
    { 3, 2 }

};


int main(int argc, char *argv[]) {
    auto samples = g_samples;

    sorter quick_sorter = quicksort;
    for (const auto & ar : samples) {
        test_quicksort(ar, quick_sorter);
    }

    return 0;
}
