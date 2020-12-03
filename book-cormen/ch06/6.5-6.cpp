#include <algorithm>
#include <vector>
#include <iostream>
#include <string>


struct stack_item {
    std::size_t m_key;
    std::size_t m_value;

    stack_item(const std::size_t p_key, const std::size_t p_val) :
        m_key(p_key),
        m_value(p_val)
    { }

    bool operator < (const stack_item & p_other) {
        return m_key < p_other.m_key;
    }
};


void max_heapify(const std::size_t p_index, std::vector<stack_item> & p_array, std::size_t p_len) {
    const std::size_t left = ((p_index + 1) << 1) - 1;
    const std::size_t right = (p_index + 1) << 1;

    std::size_t largest = p_index;

    if (left < p_len && p_array[p_index] < p_array[left]) {
        largest = left;
    }
    
    if (right < p_len && p_array[largest] < p_array[right]) {
        largest = right;
    }

    if (largest != p_index) {
        std::swap(p_array[p_index], p_array[largest]);
        max_heapify(largest, p_array, p_len);
    }
}


void build_max_heap(std::vector<stack_item> & p_array) {
    const std::size_t first_leaf = (p_array.size() >> 1) - 1;

    for (long long i = (long long) first_leaf; i >= 0; i--) {
        max_heapify(std::size_t(i), p_array, p_array.size());
    }
}


stack_item heap_extract_maximum(std::vector<stack_item> & p_array) {
    const stack_item maximum = p_array.front();

    std::swap(p_array.front(), p_array.back());
    p_array.erase(p_array.end() - 1);

    max_heapify(0, p_array, p_array.size());
    return maximum;
}


void heap_insert(const std::size_t p_index, std::vector<stack_item> & p_array) {
    if (p_index == 0) {
        return;
    }

    const std::size_t parent = ((p_index + 1) >> 1) - 1;

    if (p_array[parent] < p_array[p_index]) {
        std::swap(p_array[parent], p_array[p_index]);
        heap_insert(parent, p_array);
    }
}


void heap_insert_key(const stack_item & p_item, std::vector<stack_item> & p_array) {
    p_array.push_back(p_item);
    heap_insert(p_array.size() - 1, p_array);
}


void stack_push(const std::size_t p_value, std::vector<stack_item> & p_array) {
    static std::size_t key_counter = 0;
    heap_insert_key(stack_item(key_counter++, p_value), p_array);
}


std::size_t stack_pop(std::vector<stack_item> & p_array) {
    return heap_extract_maximum(p_array).m_value;
}


std::string to_string(std::vector<std::size_t> & p_array) {
    std::string result = "[ ";
    for (const auto & elem : p_array) {
        result += std::to_string(elem) + " ";
    }
    result += "]";

    return result;
}


void test_stack(const std::vector<std::size_t> & p_array) {
    std::vector<std::size_t> initial = p_array;
    std::vector<std::size_t> expect = p_array;

    std::vector<stack_item> heap;
    for (const auto & item : p_array) {
        stack_push(item, heap);
    }

    for (long long i = (long long) expect.size() - 1; i >= 0; i--) {
        const std::size_t p_actual = stack_pop(heap);
        const std::size_t p_expected = expect[i];

        if (p_actual != p_expected) {
            std::cout << "[FAIL] " << to_string(initial) << std::endl;
            std::cout << "Expected: " << p_expected << ", Actual: " << p_actual << std::endl;
            return;
        }
    }

    std::cout << "[PASS] " << to_string(initial) << std::endl;
}


int main(int argc, char *argv[]) {
    std::vector<std::vector<std::size_t>> arrs = { 
        { 0, 2, 4, 7, 1, 3, 4 },
        { 2, 4, 3, 3, 1, 10, 12, 2 },
        { 7, 9, 101, 2, 3, 5, 6, 2 },
        { 12, 45, 12, 4, 5 },
        { 10, 10, 10, 10, 12, 12, 12, 1, 1, 1 } };

    for (std::size_t i = 0; i < arrs.size(); i++) {
        test_stack(arrs[i]);
    }

    getchar();
    return 0;
}

