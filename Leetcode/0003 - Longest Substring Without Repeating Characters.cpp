class Solution {
public:
    int lengthOfLongestSubstring(std::string s) {
        std::unordered_map<char, std::size_t> symbols;
        int cursor = 0;
        
        int largest_length = 0;
        int string_begin = 0;
        
        for (; cursor < s.size(); cursor++) {
            auto iter_symbol = symbols.find(s[cursor]);
            if (iter_symbol != symbols.end() && iter_symbol->second >= string_begin) {
                int current_length = cursor - string_begin;
                string_begin = iter_symbol->second + 1;
                
                if (current_length > largest_length) {
                    largest_length = current_length;
                }
            }
            
            symbols[s[cursor]] = cursor;
        }
        
        int current_length = s.size() - string_begin;
        return (current_length > largest_length) ? current_length : largest_length;
    }
};