class Solution {
public:
    int arrayPairSum(std::vector<int>& nums) {
        int counters[20001] = { 0 };
        for (int elem : nums) {
            int index = elem + 10000;
            counters[index]++;
        }
        
        bool odd = true;
        int sum = 0;
        
        for (int i = 0; i < 20001; i++) {
            while (counters[i] > 0) { 
                counters[i]--;
                
                if (odd) {
                    int value = i - 10000;
                    sum += value;
                }
                
                odd = !odd;
            }
        }
        
        return sum;
    }
};