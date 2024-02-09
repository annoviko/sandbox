#include <string>
#include <unordered_map>


class Solution {
    template <typename K, typename V>
    using dict = std::unordered_map<K, V>;

    dict<int, dict<int, int>> cache; /* [length][index] -> [possible variants] */

public:
    int countVowelStrings(int n) {
        return count_vowel_string(0, 0, n);
    }

private:
    int count_vowel_string(int cur_len, int cur_idx, int max_len) {
        if (cur_idx >= 5) {
            return 0;   /* invalid string */
        }

        if (cur_len == max_len) {
            return 1;
        }

        /* get value from cache if it exists */
        auto iter_length = cache.find(cur_len);
        if (iter_length != cache.end()) {
            auto iter_index = iter_length->second.find(cur_idx);
            if (iter_index != iter_length->second.end()) {
                return iter_index->second;
            }
        }

        int result = count_vowel_string(cur_len + 1, cur_idx, max_len);  /* assign the same character */

        /* assign other possible varians */
        for (int i = cur_idx + 1; i < 5; i++) {
            result += count_vowel_string(cur_len + 1, i, max_len);
        }

        cache[cur_len][cur_idx] = result;
        return result;
    }
};