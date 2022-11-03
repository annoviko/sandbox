class Solution {
public:
    bool validPalindrome(const std::string & s) {
        int i = 0, j = s.size() - 1;
        while(i <= j) {
            if (s[i] != s[j]) {
                return is_palindrome(s, i + 1, j) || is_palindrome(s, i, j - 1);
            }
            
            i++; j--;
        }
        
        return true;
    }
    
    bool is_palindrome(const std::string & s, int i, int j) {
        while(i <= j) {
            if (s[i] != s[j]) {
                return false;
            }
            
            i++; j--;
        }
        
        return true;
    }
};