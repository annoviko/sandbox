class Solution {
public:
    bool isValid(string s) {
        std::stack<char> brackets;
        std::unordered_map<char, char> map = {
            { '(', ')' },
            { '{', '}' },
            { '[', ']' }
        };
        
        for (auto sym : s) {
            switch(sym) {
                case '(':
                case '{':
                case '[':
                    brackets.push(sym);
                    break;
                case ')':
                case '}':
                case ']': {
                    if (brackets.empty()) {
                        return false;
                    }
                    
                    auto prev = brackets.top();
                    brackets.pop();
                    
                    auto expected = map.find(prev);
                    if (sym != expected->second) {
                        return false;
                    }
                    
                    break;
                }
            }
        }
        
        return brackets.empty();
    }
};