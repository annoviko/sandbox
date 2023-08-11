class Solution {
public:
    bool wordPattern(std::string pattern, std::string s) {
        std::unordered_map<char, std::string> code_to_word;
        std::unordered_map<std::string, char> word_to_code;

        std::stringstream stream(s);

        int i = 0;
        for (std::string word; stream >> word; i++) {
            char cur_code = pattern[i];

            if (code_to_word.find(cur_code) != code_to_word.cend()) {
                std::string cur_word = code_to_word[cur_code];
                if (cur_word != word) {
                    return false;
                }
                else {
                    code_to_word[cur_code] = word;
                    word_to_code[word] = cur_code;
                }
            }
            else {
                if (word_to_code.find(word) == word_to_code.cend()) {
                    word_to_code[word] = cur_code;
                    code_to_word[cur_code] = word;
                }
                else if (cur_code != word_to_code[word]) {
                    return false;
                }
            }
        }

        return (i == pattern.size());
    }
};
