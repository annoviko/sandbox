std::unordered_map<char, std::string> keyboard = 
{
    { '2', "abc" },
    { '3', "def" },
    { '4', "ghi" },
    { '5', "jkl" },
    { '6', "mno" },
    { '7', "pqrs"},
    { '8', "tuv" },
    { '9', "wxyz"}
};


class Counter {
private:
    std::vector<int> m_cnt;
    std::vector<int> m_limit;

public:
    Counter(int size, std::vector<int> & limits) : 
        m_cnt(size, 0),
        m_limit(limits)
    { }

public:
    bool inc(void) {
        for (int i = m_cnt.size() - 1; i >= 0; i--) {
            m_cnt[i]++;
            if (m_cnt[i] < m_limit[i]) { 
                return false; 
            }
            else {
                m_cnt[i] = 0;
            }
        }
        
        return true;
    }
    
    int get(int position) { return m_cnt[position]; }
    
};

class Solution {
public:
    vector<string> letterCombinations(string digits) {
        if (digits.empty()) { return { }; }
        
        std::vector<int> limits;
        limits.reserve(digits.size());
        for (auto digit : digits) {
            limits.push_back(keyboard[digit].size());
        }
        
        Counter cnt(digits.size(), limits);
        
        vector<string> result;
        do {
            std::string letters;
            for (std::size_t i = 0; i < digits.size(); i++) {
                int position = cnt.get(i);
                std::string& word = keyboard[digits[i]];
                letters.push_back(word[position]);
            }
            result.push_back(std::move(letters));
        }
        while(!cnt.inc());

        return result;
    }
};
