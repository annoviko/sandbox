#include <vector>

class Solution {
public:
    std::vector<int> findClosestElements(const std::vector<int>& arr, int k, int x) {
        const int position = find_position(arr, x);     /* O(log(n)) */

        int lborder = position;
        int rborder = position + 1;

        if ((lborder >= 0) && (arr[lborder] == x)) {
            lborder--;
        }

        while (rborder - lborder - 1 < k) { /* O(k) */
            if (lborder >= 0 && rborder < arr.size()) {
                if (x - arr[lborder] <= arr[rborder] - x) {
                    lborder--;
                }
                else {
                    rborder++;
                }
            }
            else if (lborder >= 0) {
                lborder--;
            }
            else {
                rborder++;
            }
        }

        std::vector<int> result;
        result.reserve(k);
        for (int i = lborder + 1; i < rborder; i++) { /* O(k) */
            result.push_back(arr[i]);
        }

        return result;

        /*
        Algorithmic complexity: O(2*k) + O(log(n)) = O(k + log(n))
        Memory usage: O(1)
        */
    }

private:
    int find_position(const std::vector<int>& arr, int x) {
        int lborder = 0;
        int rborder = arr.size() - 1;

        while (lborder != rborder) {    /* Binary search: O(log(n)) */
            const int middle = lborder + (rborder - lborder) / 2;
            const int value = arr[middle];

            if (value > x) {
                rborder = middle;
            }
            else if (value < x) {
                lborder = middle + 1;
            }
            else if (value == x) {
                lborder = middle;
                rborder = middle;
            }
        }

        if (arr[lborder] > x) {
            lborder--;
        }

        return lborder;
    }
};
