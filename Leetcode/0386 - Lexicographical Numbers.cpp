class Solution {
public:
    std::vector<int> lexicalOrder(int n) {
        if (n == 0) { return { }; }
        
        int cur = 1;
        int limit = 2;
        
        std::vector<int> result(n, 0);
        result[0] = 1;
        
        for (int i = 1; i < n; i++) {
            if (cur * 10 <= n) {
                cur *= 10;
            }
            else if (cur % 10 != 9 && cur < n) {
                cur++;
            }
            else {
                do {
                    cur /= 10;
                }
                while(cur % 10 == 9);
                
                cur += 1;
            }
            
            result[i] = cur;
        }
        
        return result;
    }
};