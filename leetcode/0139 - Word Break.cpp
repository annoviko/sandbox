class Solution {
public:
    bool wordBreak(const std::string& s, const std::vector<std::string>& wordDict) {
        std::unordered_set<std::string> dictionary;
        for (const auto & word : wordDict) {
            dictionary.insert(word);
        }

        std::vector<bool> allowed_starts(s.size() + 1, false); // dp
        allowed_starts[0] = true;
        std::size_t max_allowed_start = 0;

        for (std::size_t length = 1; length <= s.size(); length++) {        // O(n)
            for (std::size_t start = 0; (start < length) && (start <= max_allowed_start); start++) {   // O(n) -> O(n^2)
                if (!allowed_starts[start]) {
                    continue;
                }

                std::string word_candidate = s.substr(start, length - start);   // O(n) -> O(n^3)
                if (dictionary.find(word_candidate) != dictionary.end()) {
                    max_allowed_start = std::max(max_allowed_start, length);
                    allowed_starts[length] = true;
                    continue; // does not matter if any other words finish at index length
                }
            }
        }

        return allowed_starts.back();
    }
};