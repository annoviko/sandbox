class Solution {
public:
    vector<int> intersect(vector<int>& nums1, vector<int>& nums2) {
        std::unordered_map<int, int> freq;
        for (int value : nums1) {
            freq[value]++;
        }

        std::vector<int> result;
        for (int value : nums2) {
            auto iter = freq.find(value);
            if (iter != freq.end() && iter->second > 0) {
                result.push_back(iter->first);
                iter->second--;
            }
        }

        return result;
    }
};
