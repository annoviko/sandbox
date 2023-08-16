class Solution {
public:
    bool containsNearbyDuplicate(std::vector<int>& nums, int k) {
        std::unordered_map<int, std::vector<int>> dict;
        for (int i = 0; i < nums.size(); i++) {
            auto& indexes = dict[nums[i]];

            if ((indexes.size() > 0) && (i - indexes.back() <= k)) {
                return true;
            }

            indexes.push_back(i);
        }

        return false;
    }
};