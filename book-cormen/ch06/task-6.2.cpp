/* Task 6-2. D-ary heap. */


#include <algorithm>
#include <vector>
#include <iostream>
#include <string>
#include <functional>


using heap_builder = std::function<std::vector<std::size_t> (const std::vector<std::size_t> &, const std::size_t)>;


void max_heapify(const std::size_t p_index, std::vector<std::size_t> & p_array, std::size_t p_length, std::size_t p_dimension) {
    std::size_t largest = p_index;

    /* find largest - O(d) */
    for (std::size_t i = 0; i < p_dimension; i++) {
        const std::size_t child = p_index * p_dimension + 1 + i;
        
        if (child < p_length && p_array[child] > p_array[largest]) {
            largest = child;
        }
    }

    if (largest != p_index) {
        std::swap(p_array[p_index], p_array[largest]);
        max_heapify(largest, p_array, p_length, p_dimension); /* continue moving to correct place - O(d * logd(n)) */
    }
}


std::vector<std::size_t> build_max_heap(const std::vector<std::size_t> & p_array, const std::size_t p_dimension) {
    std::vector<std::size_t> result = p_array;
    const std::size_t first_leaf = std::ceil(result.size() / p_dimension) - 1;

    /* for each non-leaf element - O(n) */
    for (long long i = (long long) first_leaf; i >= 0; i--) {
        max_heapify(std::size_t(i), result, result.size(), p_dimension);  /* move element to correct place - O(d * logd(n)) */
    }

    /* total complexity: O(n * d * logd(n)) */

    return result;
}


std::size_t heap_extract_maximum(std::vector<std::size_t> & p_array, const std::size_t p_dimension) {
    const std::size_t maximum = p_array.front();

    std::swap(p_array.front(), p_array.back());
    p_array.erase(p_array.end() - 1);

    max_heapify(0, p_array, p_array.size(), p_dimension);   /* place element to correct place - O(d * logd(n)) */
    return maximum;
}


void heap_insert(const std::size_t p_index, std::vector<std::size_t> & p_array, const std::size_t p_dimension) {
    if (p_index == 0) {
        return;
    }

    const std::size_t parent = std::ceil((p_index - 1) / p_dimension);

    if (p_array[parent] < p_array[p_index]) {
        std::swap(p_array[parent], p_array[p_index]);
        heap_insert(parent, p_array, p_dimension);      /* insert O(d * logd(n)) */
    }
}


void heap_insert_key(const std::size_t p_key, std::vector<std::size_t> & p_array, const std::size_t p_dimension) {
    p_array.push_back(p_key);
    heap_insert(p_array.size() - 1, p_array, p_dimension);  /* O(d * logd(n)) */
}


std::vector<std::size_t> build_max_heap2(const std::vector<std::size_t> & p_array, const std::size_t p_dimension) {
    std::vector<std::size_t> heap;
    heap.reserve(p_array.size());
    for (const auto & elem : p_array) {
        heap_insert_key(elem, heap, p_dimension);
    }

    return heap;
}


void heap_delete(const std::size_t p_index, std::vector<std::size_t> & p_array, const std::size_t p_dimension) {
    p_array[p_index] = 0;
    max_heapify(p_index, p_array, p_array.size(), p_dimension); /* O(d * logd(n)) */
    p_array.erase(p_array.end() - 1);
}


void heap_increase_key(const std::size_t p_index, std::size_t p_key, std::vector<std::size_t> & p_array, const std::size_t p_dimension) {
    heap_delete(p_index, p_array, p_dimension);
    heap_increase_key(p_index, p_key, p_array, p_dimension);
}


std::vector<std::size_t> heap_sort(const std::vector<std::size_t> & p_array, 
                                   const std::size_t p_dimension,
                                   const heap_builder & p_builder = build_max_heap) {
    std::vector<std::size_t> heap = p_builder(p_array, p_dimension);
    
    long long length = (long long) heap.size();

    while(length > 1) {
        std::swap(heap[0], heap[length - 1]);
        max_heapify(0, heap, --length, p_dimension);
    }

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
        std::cout << "initial: " << to_string(arrs[i]) << std::endl;
        std::cout << "sorted #1:  " << to_string(heap_sort(arrs[i], 3)) << std::endl;
        std::cout << "sorted #2:  " << to_string(heap_sort(arrs[i], 3, build_max_heap2)) << std::endl;
        std::cout << std::endl;
    }

    getchar();
    return 0;
}

