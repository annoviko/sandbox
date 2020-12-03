#include <algorithm>
#include <vector>
#include <iostream>
#include <string>


void max_heapify(const std::size_t p_index, std::vector<std::size_t> & p_array, std::size_t p_len) {
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


void build_max_heap(std::vector<std::size_t> & p_array) {
    const std::size_t first_leaf = (p_array.size() >> 1) - 1;

    for (long long i = (long long) first_leaf; i >= 0; i--) {
        max_heapify(std::size_t(i), p_array, p_array.size());
    }
}


std::size_t heap_maximum(const std::vector<std::size_t> & p_array) {
    return p_array.at(0);
}


std::size_t heap_extract_maximum(std::vector<std::size_t> & p_array) {
    const std::size_t maximum = p_array.front();

    std::swap(p_array.front(), p_array.back());
    p_array.erase(p_array.end() - 1);

    max_heapify(0, p_array, p_array.size());
    return maximum;
}


void heap_insert(const std::size_t p_index, std::vector<std::size_t> & p_array) {
    if (p_index == 0) {
        return;
    }

    const std::size_t parent = ((p_index + 1) >> 1) - 1;

    if (p_array[parent] < p_array[p_index]) {
        std::swap(p_array[parent], p_array[p_index]);
        heap_insert(parent, p_array);
    }
}


void heap_insert_key(const std::size_t p_key, std::vector<std::size_t> & p_array) {
    p_array.push_back(p_key);
    heap_insert(p_array.size() - 1, p_array);
}



std::string to_string(std::vector<std::size_t> & p_array) {
    std::string result = "[ ";
    for (const auto & elem : p_array) {
        result += std::to_string(elem) + " ";
    }
    result += "]";

    return result;
}


void test_extract_heap(const std::vector<std::size_t> & p_array) {
    std::vector<std::size_t> initial = p_array;
    std::vector<std::size_t> expect = p_array;
    std::sort(expect.begin(), expect.end());

    std::vector<std::size_t> heap = p_array;
    build_max_heap(heap);

    for (long long i = (long long) expect.size() - 1; i >= 0; i--) {
        const std::size_t p_actual = heap_extract_maximum(heap);
        const std::size_t p_expected = expect[i];

        if (p_actual != p_expected) {
            std::cout << "[FAIL] " << to_string(initial) << std::endl;
            std::cout << "Expected: " << p_expected << ", Actual: " << p_actual << std::endl;
            return;
        }
    }

    std::cout << "[PASS] " << to_string(initial) << std::endl;
}


void test_insert_extract_heap(const std::vector<std::size_t> & p_elems) {
    std::vector<std::size_t> initial = p_elems;
    std::vector<std::size_t> expect = p_elems;
    std::sort(expect.begin(), expect.end());

    std::vector<std::size_t> heap = { };

    for (const auto & elem : p_elems) {
        heap_insert_key(elem, heap);
    }

    for (long long i = (long long) expect.size() - 1; i >= 0; i--) {
        const std::size_t p_actual = heap_extract_maximum(heap);
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
        test_extract_heap(arrs[i]);
    }

    for (std::size_t i = 0; i < arrs.size(); i++) {
        test_insert_extract_heap(arrs[i]);
    }

    getchar();
    return 0;
}

