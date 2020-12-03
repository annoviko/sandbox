class Solution {
public:
#ifdef SOLUTION_WITH_ADDITIONAL_MEMORY
    std::vector<int> majorityElement(const std::vector<int> & nums) {
        if (nums.empty()) { return { }; }

        std::size_t threshold = nums.size() / 3;
        std::unordered_map<int, int> counter;

        for (const int value : nums) {
            counter[value]++;
        }

        std::vector<int> result;
        for (const auto & pair : counter) {
            if (counter[pair.first] > threshold) {
                result.push_back(pair.first);
            }
        }

        return result;
    }
#else
    std::vector<int> majorityElement(const std::vector<int> & nums) {
        if (nums.empty()) { return { }; }

        int candidate1 = nums[0];
        std::optional<int> candidate2;
        std::size_t counter1 = 1, counter2 = 0;

        for (std::size_t i = 1; i < nums.size(); i++) {
            const int value = nums[i];

            if (candidate1 == value) {
                counter1++;
            }
            else if (candidate2.has_value() && (candidate2.value() == value)) {
                counter2++;
            }
            else if (counter1 == 0) {
                candidate1 = value;
                counter1++;
            }
            else if (counter2 == 0) {
                candidate2 = value;
                counter2++;
            }
            else {
                counter1--;
                counter2--;
            }
        }

        /* check they are real majority */
        const std::size_t threshold = nums.size() / 3;
        counter1 = 0, counter2 = 0;
        for (std::size_t i = 0; i < nums.size(); i++) {
            const int value = nums[i];
            if (value == candidate1) {
                counter1++;
            }
            else if (candidate2.has_value() && value == candidate2) {
                counter2++;
            }
        }

        std::vector<int> result;
        result.reserve(2);

        if (counter1 > threshold) {
            result.push_back(candidate1);
        }

        if (candidate2.has_value() && counter2 > threshold) {
            result.push_back(candidate2.value());
        }

        return result;
    }
#endif
};