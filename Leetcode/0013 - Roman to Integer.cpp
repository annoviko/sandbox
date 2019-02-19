class Solution {
public:
    std::unordered_map<char, int> table = { 
      { 'I', 1 }, 
      { 'V', 5 },
      { 'X', 10 },
      { 'L', 50 },
      { 'C', 100 },
      { 'D', 500 },
      { 'M', 1000 } };

    int romanToInt(std::string s) {
        int result = 0;
        int previous = INT_MAX;

        for (const auto symbol : s) {
            int current = table[symbol];
            if (previous < current) {
                result += current - (previous << 1);
                previous = INT_MAX;
            }
            else {
                result += current;
                previous = current;
            }
        }

        return result;
    }
};