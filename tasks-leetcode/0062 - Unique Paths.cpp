class Solution {
public:
    int uniquePaths(int m, int n) {
        if (m > n) {
            return uniquePaths(n, m);
        }
        
        std::vector<int> cur_row(n, 1);
        
        for (std::size_t i = 1; i < m; i++) {
            for (std::size_t j = 0; j < n; j++) {
                cur_row[j] = cur_row[j] + ((j > 0) ? cur_row[j - 1] : 0);
            }
        }
        
        return cur_row.back();
    }
};