class Solution {
public:
    std::string largestNumber(const std::vector<int>& nums) {
        std::vector<std::string> str_nums;
        for (auto val : nums) {
            str_nums.push_back(std::to_string(val));
        }

        std::sort(str_nums.begin(), str_nums.end(), [](const std::string& l, const std::string& r) {
            std::string lr = l + r;
            std::string rl = r + l;
            return lr > rl;
        });
        
        if (str_nums[0] == "0") {
            return "0";
        }

        std::string result;
        for (auto val : str_nums) {
            result += val;
        }

        return result;
    }
};