class Solution {
public:
    std::vector<std::string> buildArray(std::vector<int>& target, int n) {
        std::vector<std::string> result;
        for (std::size_t i = 1, j = 0; i <= (std::size_t) n && j < target.size(); i++) {
            result.push_back("Push");
 
            if (i != target[j]) {
                result.push_back("Pop");
            }
            else {
                j++;
            }
        }
        
        return result;
    }
};