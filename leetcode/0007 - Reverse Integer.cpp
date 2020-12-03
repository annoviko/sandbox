class Solution {
public:
    int reverse(int x) {
        int buffer = std::abs(x);
        int result = 0;
        while(buffer > 0) {
            int value = buffer % 10;
            buffer = buffer / 10;
            
            if ( result > INT_MAX / 10 || (result == INT_MAX / 10 && value > 7) ) {
                return 0;
            }
            
            result = result * 10 + value;
        }
        
        if (x < 0) {
            result = -result;
        }
        
        return result;
    }
};