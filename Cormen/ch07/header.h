#include <iostream>
#include <limits>


namespace ch07
{


template <typename Container>
void print(const Container & p_container) {
    std::cout << "[ ";
    for (const auto & elem : p_container)
      std::cout << elem << " ";

    std::cout << "]" << std::endl;
}


template <typename ArgumentType>
void assert_eq(const ArgumentType & p_expected, const ArgumentType & p_actual) {
    if (p_expected != p_actual) {
        std::cout << "[FAIL]" << std::endl;
    }
    else {
        std::cout << "[PASS]" << std::endl;
    }
}


template <typename Container>
std::size_t partition(Container & p_array, const std::size_t p_begin, const std::size_t p_anchor) {
    std::size_t lborder = p_begin - 1;

    for (std::size_t rborder = p_begin; rborder < p_anchor; rborder++) {
        if (p_array[rborder] < p_array[p_anchor]) {
            std::swap(p_array[rborder], p_array[++lborder]);
        }
    }

    std::swap(p_array[lborder + 1], p_array[p_anchor]);
    return lborder + 1;
}


template <typename Container>
void quicksort(Container & p_array, const std::size_t p_begin, const std::size_t p_anchor) {
    if (p_begin < p_anchor && p_anchor != (std::size_t) -1) {
        const std::size_t border = partition(p_array, p_begin, p_anchor);
        quicksort(p_array, p_begin, border - 1);
        quicksort(p_array, border + 1, p_anchor);
    }
}


}