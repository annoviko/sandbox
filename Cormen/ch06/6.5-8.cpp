/* 
   Chapter 6. Task 6.5-8
   Merge sorted arrays to one that is sorted as well. Complexity should be O(n * lg(k))
   See function: heap_merge_sorted_lists
*/

#include <algorithm>
#include <vector>
#include <iostream>
#include <string>
#include <numeric>


struct heap_item {
    std::size_t m_index;    // list index
    std::size_t m_value;    // item value

    heap_item(const std::size_t p_index, const std::size_t p_value) :
        m_index(p_index), m_value(p_value)
    { }

    bool operator< (const heap_item & p_other) {
        return m_value < p_other.m_value;
    }
};


void max_heapify(const std::size_t p_index, std::vector<heap_item> & p_array, std::size_t p_len) {
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


void build_max_heap(std::vector<heap_item> & p_array) {
    const std::size_t first_leaf = (p_array.size() >> 1) - 1;

    for (long long i = (long long) first_leaf; i >= 0; i--) {
        max_heapify(std::size_t(i), p_array, p_array.size());
    }
}


heap_item heap_extract_maximum(std::vector<heap_item> & p_array) {
    const heap_item maximum = p_array.front();

    std::swap(p_array.front(), p_array.back());
    p_array.erase(p_array.end() - 1);

    max_heapify(0, p_array, p_array.size());
    return maximum;
}


void heap_insert(const std::size_t p_index, std::vector<heap_item> & p_array) {
    if (p_index == 0) {
        return;
    }

    const std::size_t parent = ((p_index + 1) >> 1) - 1;

    if (p_array[parent] < p_array[p_index]) {
        std::swap(p_array[parent], p_array[p_index]);
        heap_insert(parent, p_array);
    }
}


void heap_insert_key(const std::size_t p_index, const std::size_t p_value, std::vector<heap_item> & p_array) {
    p_array.push_back(heap_item(p_index, p_value));
    heap_insert(p_array.size() - 1, p_array);
}



std::vector<std::size_t> heap_merge_sorted_lists(std::vector<std::vector<std::size_t>> & p_lists) {
    /* reserve memory - O(k) */
    const std::size_t total_length = std::accumulate(p_lists.begin(), p_lists.end(), std::size_t(0), [](std::size_t p_val, auto & container) {
        return p_val + container.size();
    });

    long long position = (long long) total_length - 1;
    std::vector<std::size_t> result(total_length);

    /* build heap using max elements - O(lg(k)) */
    std::vector<heap_item> heap;
    for (std::size_t i = 0; i < p_lists.size(); i++) {
        heap_insert_key(i, p_lists[i].back(), heap);    /* O(lg(k)) */
        p_lists[i].erase(p_lists[i].end() - 1);         /* erase last element - O(1) */
    }

    /* repeat n times - O(n) */
    while(position >= 0) {
        /* get max element - O(lg(k)) */
        auto item = heap_extract_maximum(heap);
        result[position--] = item.m_value;

        /* insert next element to the heap - O(lg(k)) */
        const std::size_t list_index = item.m_index;
        if (p_lists[list_index].size() > 0) {
            heap_insert_key(list_index, p_lists[list_index].back(), heap);  /* O(lg(k)) */
            p_lists[list_index].erase(p_lists[list_index].end() - 1);       /* erase last element - O(1) */
        }
    }

    /* total complexity: O(n * lg(n)) */

    return result;
}


std::string to_string(const std::vector<std::size_t> & p_array) {
    std::string result = "[ ";
    for (const auto & elem : p_array) {
        result += std::to_string(elem) + " ";
    }
    result += "]";

    return result;
}


void test_merge_heap(std::vector<std::vector<std::size_t>> & p_arrs) {
    std::vector<std::size_t> expected;
    for (const auto & arr : p_arrs)
        expected.insert(expected.end(), arr.begin(), arr.end());

    std::sort(expected.begin(), expected.end());

    const auto result = heap_merge_sorted_lists(p_arrs);

    if (result != expected) {
        std::cout << "[FAIL] " << std::endl;
        std::cout << "Expected: " << to_string(expected) << std::endl;
        std::cout << "Actual: " << to_string(result) << std::endl;
        return;
    }

    std::cout << "[PASS] " << to_string(result) << std::endl;
}


int main(int argc, char *argv[]) {
    std::vector<std::vector<std::size_t>> arrs = { 
        { 1, 2, 3, 4, 5, 6, 7, 8 },
        { 2, 5, 7, 8, 10, 12 },
        { 14, 16, 17, 18 },
        { 0, 1, 1, 7, 9, 19 },
        { 2, 3, 4, 5 } };

    test_merge_heap(arrs);

    getchar();
    return 0;
}

