#include <unordered_map>
#include <vector>


struct pool_t {
    int iter = 0;
    std::vector<int> indexes;

    int get() {
        if (iter >= indexes.size()) {
            iter = 0;
        }
        return indexes[iter++];
    }
};

class Solution {
private:
    std::unordered_map<int, pool_t> pool;

public:
    Solution(const std::vector<int>& nums) {
        for (int i = 0; i < nums.size(); i++) {
            pool[nums[i]].indexes.push_back(i);
        }
    }

    int pick(int target) {
        return pool[target].get();
    }
};