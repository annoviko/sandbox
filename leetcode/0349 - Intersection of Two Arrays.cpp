class Solution {
public:
    std::vector<int> intersection(std::vector<int>& nums1, std::vector<int>& nums2) {
        std::unordered_set<int> set;

        for (const auto val : nums1) {  // O(n)
            set.insert(val);  // O(1)
        }

        std::vector<int> result;
        result.reserve(set.size());

        for (const auto val2 : nums2) { // O(m)
            const auto iter = set.find(val2); // O(1)
            if (iter != set.end()) {
                result.push_back(*iter);
                set.erase(iter);
            }
        }

        return result;
    }
};