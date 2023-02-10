class Solution {
public:
    std::vector<int> topKFrequent(const std::vector<int>& nums, int k) {
        std::unordered_map<int, int> counter;  /* value: frequency */
        for (int val : nums) {
            counter[val]++;
        }

        std::vector<std::vector<int>> frequency(nums.size() + 1);
        for (auto& pair : counter) {
            frequency[pair.second].push_back(pair.first);
        }

        std::vector<int> result;
        for (int i = int(frequency.size() - 1); (i >= 0) && (result.size() < k); i--) {
            if (frequency[i].empty()) {
                continue;
            }

            for (int j = 0; j < frequency[i].size() && result.size() < k; j++) {
                result.push_back(frequency[i][j]);
            }
        }

        return result;
    }
};