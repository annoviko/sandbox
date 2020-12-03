// See explanation: https://www.youtube.com/watch?v=YDf982Lb84o

class Solution {
public:
    int numTrees(int n) {
        if (n == 0) { return 0; }

        std::vector<int> ntree(n, 0);
        
        ntree[0] = 1;
        for (int i = 1; i < n; i++) {
            for (int j = 0; j <= i; j++) {
                int left_subtrees = (j > 0) ? ntree[j - 1] : 1;
                int right_subtrees = (j < i) ? ntree[i - j - 1] : 1;
                
                ntree[i] += left_subtrees * right_subtrees;
            }
        }
        
        return ntree[n - 1];
    }
};