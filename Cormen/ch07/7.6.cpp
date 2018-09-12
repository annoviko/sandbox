#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>


struct interval {
public:
    std::size_t m_left;
    std::size_t m_right;

public:
    bool operator<=(const interval & p_other) const {
        return (m_left < p_other.m_left || (m_left == p_other.m_right && m_right == p_other.m_right));
    }

    bool operator<(const interval & p_other) const {
        return (m_left < p_other.m_left);
    }

    bool operator==(const interval & p_other) const {
        return (m_left == p_other.m_left);
    }

    friend std::ostream & operator<<(std::ostream & p_stream, const interval & p_other) {
        std::cout << "[" << p_other.m_left << ", " << p_other.m_right << "]";
        return p_stream;
    }
};


std::size_t partition(std::vector<interval> & p_array, const std::size_t p_begin, const std::size_t p_anchor) {
    auto lborder = p_begin - 1;
    for (std::size_t rborder = p_begin; rborder < p_anchor; rborder++) {
        if (p_array[rborder] <= p_array[p_anchor]) {
            std::swap(p_array[++lborder], p_array[rborder]);
        }
    }

    std::swap(p_array[++lborder], p_array[p_anchor]);
    return lborder;
}


void quicksort(std::vector<interval> & p_array, const std::size_t p_begin, const std::size_t p_anchor) {
    if (p_begin < p_anchor && p_anchor != (std::size_t) -1) {
        const std::size_t border = partition(p_array, p_begin, p_anchor);
        quicksort(p_array, p_begin, border - 1);
        quicksort(p_array, border + 1, p_anchor);
    }
}


std::ostream & operator<<(std::ostream & p_stream, std::vector<interval> & p_array) {
    p_stream << "[ ";
    for (const auto & p_elem : p_array) {
        p_stream << p_elem << " ";
    }
    p_stream << "]";
    return p_stream;
}


void test_quicksort(const std::vector<interval> & p_array) {
    auto expected = p_array;
    auto actual = p_array;

    std::sort(expected.begin(), expected.end());
    quicksort(actual, 0, actual.size() - 1);

    if (expected != actual) {
        std::cout << "[FAIL] Actual: " << actual << ", Expected: " << expected << std::endl;
    }
    else {
        std::cout << "[PASS]" << std::endl;
    }
}


std::vector<std::vector<interval>> g_samples = {

  { { 1, 5 }, { 2, 4 }, { 3, 5 }, { 5, 7 } },
  { { 1, 3 }, { 2, 4 }, { 1, 3 } },
  { { 2, 4 }, { 4, 6 }, { 1, 2 }, { 0, 1 }, { 4, 9 } }

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
