class Solution {
public:
    void reverseWords(std::string & words) {
        trim(words);

        for (std::size_t i = 0; i < words.size(); i++) {
            std::size_t end_word = words.find(" ", i);
            if (end_word == std::string::npos) {
                end_word = words.size();
            }

            std::reverse(words.begin() + i, words.begin() + end_word);
            i = end_word;
        }

        std::reverse(words.begin(), words.end());
    }

private:
    void trim(std::string & words) {
        for (std::size_t pos = 0; pos < words.size(); pos++) {
            pos = words.find(" ", pos);
            if (pos != std::string::npos) {
                while ( ((pos + 1) < words.size() && words[pos] == ' ' && words[pos + 1] == ' ') || (pos < words.size() && words[pos] == ' ' && (pos == 0 || pos == words.size() - 1)) ) {
                    words.erase(pos, 1);
                }
            }
            else { break; }
        }
    }
};