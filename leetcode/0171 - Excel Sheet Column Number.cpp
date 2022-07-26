class Solution {
public:
    int titleToNumber(string columnTitle) {
        const char begin = 'A';
        const char end = 'Z';
        
        int result = 0;
        for (const char sym : columnTitle) {
            const int value = sym - begin + 1;
            result = result * (end - begin + 1) + value;
        }
        
        return result;
    }
};