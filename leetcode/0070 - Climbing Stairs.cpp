class Solution {
public:
    int climbStairs(int n) {
        if (n == 0) { return 0; }
        if (n == 1) { return 1; }
        
        std::vector<int> steps(n);
        steps[0] = 1;
        steps[1] = 2;
        
        for (int i = 2; i < n; i++) {
            steps[i] = steps[i - 1] + steps[i - 2];
        }
        
        return steps.back();
    }
};