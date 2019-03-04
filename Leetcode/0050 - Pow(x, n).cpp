class Solution {
public:
    double myPow(double x, int n) {
        if (n == 0) { return 1.0; }
        
        if (n < 0) {
            return (1.0 / x) * myPow(1.0 / x, -(n + 1));    
        }
        
        else if (n % 2 == 0) {
            double result = myPow(x, n / 2);
            return result * result;
        }
        else {
            return x * myPow(x, n - 1);
        }
    }
};