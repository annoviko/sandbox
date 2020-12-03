class Solution {
public:
    int maxArea(std::vector<int>& height) {
        if (height.size() < 2) { return 0; }

        int result = 0;
        std::size_t lpos = 0, rpos = height.size() - 1;
        while (lpos < rpos) {
            int candidate = std::min(height[lpos], height[rpos]) * (rpos - lpos);
            result = std::max(result, candidate);

            if (height[lpos] > height[rpos]) {
                rpos--;
            }
            else {
                lpos++;
            }
        }

        return result;
    }
};