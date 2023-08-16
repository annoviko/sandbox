class Solution {
public:
    bool containsDuplicate(const std::vector<int>& nums) {
        std::unordered_set<int> unique;
        for (int value : nums) {
            auto iter = unique.find(value);
            if (iter != unique.cend()) {
                return true;
            }

            unique.insert(value);
        }

        return false;
    }
};