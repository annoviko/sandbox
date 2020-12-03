#include <algorithm>
#include <functional>
#include <iostream>
#include <random>
#include <vector>


std::random_device g_device;
std::mt19937 g_generator(g_device());


struct interval {
public:
    std::size_t m_left;
    std::size_t m_right;

public:
    bool contains(const interval & p_interval) {
        return (m_right <= p_interval.m_left && m_left >= p_interval.m_right);
    }

    bool operator<(const interval & p_interval) {
        return m_left < p_interval.m_left;
    }

    bool operator==(const interval & p_interval) {
        return m_left == p_interval.m_left && m_right == p_interval.m_right;
    }

    friend std::ostream & operator<<(std::ostream & p_stream, const interval & p_other) {
        for (std::size_t i = 0; i < p_other.m_left; i++)
            std::cout << " ";

        for (std::size_t i = p_other.m_left; i < p_other.m_right; i++)
            std::cout << "-";

        return p_stream;
    }
};


interval find_pivot_interval(std::vector<interval> & p_array, const std::size_t p_begin, const std::size_t p_end) {
    std::uniform_int_distribution<> distribution(p_begin, p_end);
    std::size_t pivot_index = distribution(g_generator);
    std::swap(p_array[pivot_index], p_array[p_end]);

    interval result = p_array[p_end];
    for (std::size_t i = p_begin; i < p_end; i++) {
        if (result.contains(p_array[i])) {
            if (result.m_left > p_array[i].m_left)
                result.m_left = p_array[i].m_left;

            if (result.m_right < p_array[i].m_right)
                result.m_right = p_array[i].m_right;
        }
    }

    return result;
}


std::size_t partition_by_left_border(std::vector<interval> & p_array, const std::size_t p_begin, const std::size_t p_end, const std::size_t p_left) {
    std::size_t lborder = p_begin - 1;
    for (std::size_t rborder = p_begin; rborder < p_end; rborder++) {
        if (p_array[rborder].m_left <= p_left) {
            std::swap(p_array[rborder], p_array[++lborder]);
        }
    }

    std::swap(p_array[++lborder], p_array[p_end]);
    return lborder;
}


std::size_t partition_by_right_border(std::vector<interval> & p_array, const std::size_t p_begin, const std::size_t p_end, const std::size_t p_right) {
    std::size_t lborder = p_begin - 1;
    for (std::size_t rborder = p_begin; rborder < p_end; rborder++) {
        if (p_array[rborder].m_right < p_right) {
            std::swap(p_array[rborder], p_array[++lborder]);
        }
    }

    std::swap(p_array[++lborder], p_array[p_end]);
    return lborder;
}


void fuzzy_quicksort(std::vector<interval> & p_array, const std::size_t p_begin, const std::size_t p_anchor) {
    if (p_begin < p_anchor && p_anchor != (std::size_t) - 1) {
        auto pivot_interval = find_pivot_interval(p_array, p_begin, p_anchor);

        const std::size_t right_border = partition_by_left_border(p_array, p_begin, p_anchor, pivot_interval.m_left);
        const std::size_t left_border = partition_by_right_border(p_array, p_begin, right_border, pivot_interval.m_right);

        fuzzy_quicksort(p_array, p_begin, left_border - 1);
        fuzzy_quicksort(p_array, right_border + 1, p_anchor);
    }
}


std::ostream & operator<<(std::ostream & p_stream, std::vector<interval> & p_array) {
    for (const auto & p_elem : p_array) {
        p_stream << p_elem << std::endl;
    }
    return p_stream;
}


void test_quicksort(const std::vector<interval> & p_array) {
    auto expected = p_array;
    auto actual = p_array;

    std::sort(expected.begin(), expected.end());
    fuzzy_quicksort(actual, 0, actual.size() - 1);

    std::cout << std::endl << expected << std::endl;
}


std::vector<std::vector<interval>> g_samples = {

    { { 1, 5 },{ 2, 4 },{ 3, 5 },{ 5, 7 } },
    { { 1, 3 },{ 2, 4 },{ 1, 3 } },
    { { 2, 4 },{ 4, 6 },{ 1, 2 },{ 0, 1 },{ 4, 9 } }

};


int main(int argc, char *argv[]) {
    auto samples = g_samples;

    for (const auto & ar : samples) {
        test_quicksort(ar);
    }

    std::cout << "Press any key to continue...";
    getchar();

    return 0;
}
