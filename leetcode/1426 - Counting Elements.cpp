/*

There is no ID for the task yet.

Given an integer array arr, count element x such that x + 1 is also in arr.
If there're duplicates in arr, count them seperately.

*/

#include <vector>
#include <unordered_set>


class Solution {
public:
    int countElements(const std::vector<int>& arr) {
        if (arr.size() < 2) { return 0; }

        std::unordered_set<int> unique_elements;

        for (const int value : arr) {
            unique_elements.insert(value);
        }

        int result = 0;
        for (const int value : arr) {
            if (unique_elements.find(value + 1) != unique_elements.end()) {
                result++;
            }
        }

        return result;
    }
};