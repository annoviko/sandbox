class Solution {
public:
    std::string removeDuplicateLetters(const std::string & s) {
        if (s.empty()) { return std::string(); }

        constexpr std::size_t length = 'z' - 'a' + 1;

        std::vector<std::size_t> counter(length, 0);
        std::vector<bool> locker(length, false);

        for (const auto symbol : s) {   /* O(n) */
            counter[symbol - 'a']++;
        }

        std::string stack_result(1, s[0]);
        counter[s[0] - 'a']--;
        locker[s[0] - 'a'] = true;

        stack_result.reserve(length);

        for (std::size_t i = 1; i < s.size(); i++) {    /* O(N) */
            const char symbol = s[i];
            counter[symbol - 'a']--;

            if (locker[symbol - 'a']) { /* if it is already locked (added to stack) - skip it */
                continue;
            }

            /* 
            
            go through the stack until the current element is less than the top (but, do not touch element with counter == 0).

            */
            while (!stack_result.empty() && (counter[stack_result.back() - 'a'] != 0) && (stack_result.back() > symbol)) {  /* O(1) - amoung 26 english letters */
                locker[stack_result.back() - 'a'] = false;  /* unlock */
                stack_result.pop_back();
            }

            stack_result.push_back(symbol);
            locker[symbol - 'a'] = true;    /* lock - do not allow to use it further */
        }

        return stack_result;
    }
};