class Solution {
public:
    int majorityElement(const std::vector<int>& nums) {
#ifdef DO_NOT_TRUST_TO_RESTRICTIONS
        if (nums.empty()) { throw std::invalid_argument("The input collection is empty."); }
#endif
        auto candidate = nums[0];
        std::size_t counter = 1;

        for (std::size_t i = 1; i < nums.size(); i++) {
            auto value = nums[i];

            if (value == candidate) {
                counter++;
            }
            else if (counter == 0) {
                candidate = value;
            }
            else {
                counter--;
            }
        }

#ifdef DO_NOT_TRUST_TO_RESTRICTIONS
        /* check if it real majority - more than n/2 */
        const std::size_t threshold = nums.size() / 2;
        counter = 0;

        for (const auto & value : nums) {
            if (value == candidate) {
                counter++;
            }
        }

        if (counter > threshold) {
            return candidate;
        }

        throw std::invalid_argument("The input collection does not have majority that is greater than n/2.");
#else
        return candidate;
#endif
    }
};