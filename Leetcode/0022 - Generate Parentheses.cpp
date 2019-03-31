class Solution {
private:
    std::vector<std::string> m_result;
    
public:
    std::vector<std::string> generateParenthesis(int n) {
        m_result = { };
        
        generate(n, 0, 0, std::string());
        return m_result;
    }
    
private:
    void generate(int n, int open, int close, std::string p_parent) {
        if (p_parent.size() == n << 1) {
            m_result.push_back(p_parent);
        }
        
        if (open < n) {
            generate(n, open + 1, close, p_parent + "(");
        }
        
        if (close < open) {
            generate(n, open, close + 1, p_parent + ")");
        }
    }
};