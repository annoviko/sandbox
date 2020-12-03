std::unordered_map<int, char> g_table = 
  { 
    { 1, 'I' },
    { 5, 'V' },
    { 10, 'X' },
    { 50, 'L' },
    { 100, 'C' },
    { 500, 'D' },
    { 1000, 'M' }
  };


class Solution {
public:
    std::string intToRoman(int num) {
        std::string result;

        int divider = 10;
        while (num > 0) {
            int digit = num % divider;

            if (digit != 0) {
                int center_key = divider / 2;

                int key_limit = center_key;
                int key_base  = divider / 10;
                int key_unit  = key_base;
                if (center_key <= digit) {
                    key_limit = center_key * 2;
                    key_base  = center_key;
                }

                char sym_unit   = g_table[key_unit];
                char sym_base   = g_table[key_base];
                char sym_limit  = g_table[key_limit];

                if (key_limit - key_unit == digit) {
                    std::string value = std::string(1, sym_unit) + std::string(1, sym_limit);
                    result.insert(0, value);
                }
                else {
                    std::string value = std::string(1, sym_base);
                    int score = digit - key_base;
                    while (score > 0) {
                        score -= key_unit;
                        value += std::string(1, sym_unit);
                    }

                    result.insert(0, value);
                }
            }

            num -= digit;
            divider *= 10;
        }

        return result;
    }
};