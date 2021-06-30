class Solution {
public:
    int strStr(const std::string & haystack, const std::string & needle) {
        if (needle.empty()) { return 0; }

        std::vector<std::size_t> counter(needle.size(), 0);
        for (std::size_t i = 0, j = 1; j < needle.size(); j++) {
            if (needle[i] == needle[j]) {
                counter[j] = i + 1;
                i++;
            }
            else {
                while ((i > 0) && (needle[i] != needle[j])) {
                    i = counter[i - 1];
                }

                if (needle[i] == needle[j]) {
                    counter[j] = i + 1;
                    i++;
                }
                else {
                    counter[j] = 0;
                }
            }
        }

        for (std::size_t i = 0, j = 0; i < haystack.size(); i++) {
            while (j > 0 && haystack[i] != needle[j]) {
                j = counter[j - 1];
            }

            if (haystack[i] == needle[j]) {
                j++;

                if (j == needle.size()) {
                    return (i + 1) - j;
                }
            }
        }

        return -1;
    }
};