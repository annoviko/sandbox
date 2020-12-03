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


std::vector<std::size_t> build_max_heap1(const std::vector<std::size_t> & p_array) {
    std::vector<std::size_t> result = p_array;
    const std::size_t first_leaf = (result.size() >> 1) - 1;

    /* for each non-leaf element - O(n) */
    for (long long i = (long long) first_leaf; i >= 0; i--) {
        max_heapify(std::size_t(i), result, result.size());  /* move element to correct place - O(lg(n)) */
    }

    return result;
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


std::vector<std::size_t> build_max_heap2(const std::vector<std::size_t> & p_array) {
    std::vector<std::size_t> heap;

    /* for each element - O(n) */
    for (const auto & item : p_array) {
        heap_insert_key(item, heap);    /* insert new element - O(lg(n)) */
    }

    /* Complexity - O(n * lg(n)) */

    return heap;
}


std::string to_string(std::vector<std::size_t> & p_array) {
    std::string result = "[ ";
    for (const auto & elem : p_array) {
        result += std::to_string(elem) + " ";
    }
    result += "]";

    return result;
}


int main(int argc, char *argv[]) {
    std::vector<std::vector<std::size_t>> arrs = { 
        { 0, 1, 2, 3, 4, 5 },
        { 2, 4, 3, 3, 1, 10, 12, 2 },
        { 7, 9, 101, 2, 3, 5, 6, 2 },
        { 12, 45, 12, 4, 5 },
        { 10, 10, 10, 10, 12, 12, 12, 1, 1, 1 } };

    for (std::size_t i = 0; i < arrs.size(); i++) {
        std::cout << i << ") " << to_string(arrs[i]) << std::endl;
        std::cout << "build heap #1: " << to_string(build_max_heap1(arrs[i])) << std::endl;
        std::cout << "build heap #2: " << to_string(build_max_heap2(arrs[i])) << std::endl;
        std::cout << std::endl;
    }

    getchar();
    return 0;
}

