#define SOLUTION_WITHOUT_MEMORY

class Solution {
public:
    int singleNumber(std::vector<int>& nums) {
#if defined(SOLUTION_WITHOUT_MEMORY)
        int value = 0;
        for (auto num : nums) {
            value ^= num;
        }

        return value;
#else
        std::unordered_map<int, int> freq = { };
        for (auto num : nums) {
            freq[num]++;
        }
        
        for (const auto & pair : freq) {
            if (pair.second == 1) {
                return pair.first;
            }
        }

        return 0;
#endif
    }
};