class Solution {
public:
    std::vector<std::vector<int>> threeSum(std::vector<int>& nums) {
        if (nums.size() < 3) { return {}; };
        
        std::vector<std::vector<int>> result;

        std::sort(nums.begin(), nums.end());

        for (std::size_t i = 0; i < nums.size() - 1; i++) {
            int target = -nums[i];
            int index_front = i + 1;
            int index_back = nums.size() - 1;

            while(index_front < index_back) {
                int sum = nums[index_front] + nums[index_back];
                if (sum > target) {
                    index_back--;
                }
                else if (sum < target) {
                    index_front++;
                }
                else {
                    result.push_back({ nums[i], nums[index_front], nums[index_back] });

                    while(index_front < index_back && nums[index_front] == result.back().at(1)) { 
                        index_front++;
                    }

                    while(index_back > index_front && nums[index_back] == result.back().at(2)) {
                        index_back--;
                    }
                }
            }

            const int left_value = nums[i];
            while(i + 1 < nums.size() && left_value == nums[i + 1]) { 
                i++;
            }
        }
        
        return result;
    }
};
