class Solution {
public:
    int trap(std::vector<int>& height) {
        if (height.size() < 3) { return 0; }
        std::vector<int> lborders(height.size());
        std::vector<int> rborders(height.size());

        lborders[0] = height[0];
        for (int i = 1; i < height.size(); i++) {
            lborders[i] = std::max(height[i], lborders[i - 1]);
        }

        rborders[height.size() - 1] = height.back();
        for (int i = height.size() - 2; i >= 0; i--) {
            rborders[i] = std::max(height[i], rborders[i + 1]);
        }

        int result = 0;
        for (int i = 1; i < height.size() - 1; i++) {
            result += std::min(lborders[i], rborders[i]) - height[i];
        }

        return result;
    }
};