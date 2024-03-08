#include <algorithm>
#include <iostream>
#include <vector>


class solution {
public:
    std::vector<int> max_sequence(const std::vector<int>& in) {
        std::vector<int> dp(in.size(), 1);
        std::vector<int> sequence_map(in.size(), -1);

        for (int i = 0; i < in.size(); i++) {   /* O(N^2) */
            for (int j = 0; j < i; j++) {
                if (in[j] < in[i]) {
                    int new_len = dp[j] + 1;
                    if (dp[i] < new_len) {
                        dp[i] = new_len;
                        sequence_map[i] = j;
                    }
                    else if (new_len == dp[i]) {    /* update pointer to the lowest previous */
                        if (in[j] < in[sequence_map[i]]) {  /* there is a smaller element */
                            sequence_map[i] = j;
                        }
                    }
                }
            }
        }

        int end_index_max = 0;
        for (int i = 0; i < dp.size(); i++) {
            if (dp[i] == dp[end_index_max]) {
                if (in[i] < in[end_index_max]) {    /* if they are not equal - just take the smallest */
                    end_index_max = i;
                }
                else if ((in[i] == in[end_index_max]) && (dp[i] > 1)) {  /* if they are equal - take an element with the smallest previous value if they have (dp[i] > 1 => sequence is bigger than 1) */
                    int prev_idx_for_i = sequence_map[i];
                    int prev_idx_for_cur = sequence_map[end_index_max];

                    if (in[prev_idx_for_i] < in[prev_idx_for_cur]) {
                        end_index_max = i;
                    }
                }
            }
            else if (dp[i] > dp[end_index_max]) {
                end_index_max = i;
            }
        }

        std::vector<int> result;
        while (end_index_max != -1) {
            result.push_back(in[end_index_max]);
            end_index_max = sequence_map[end_index_max];
        }

        std::reverse(result.begin(), result.end());
        return result;
    }
};


std::ostream& operator<<(std::ostream& stream, const std::vector<int>& vec) {
    stream << vec.size();
    for (auto v : vec) {
        std::cout << ' ' << v;
    }
    return stream;
}


int main() {
    while (true) {
        int n = 0;
        std::cin >> n;

        if (n == 0) {
            break;
        }

        std::vector<int> in;
        for (int i = 0; i < n; i++) {
            int v = 0;
            std::cin >> v;
            in.push_back(v);
        }

        std::cout << solution().max_sequence(in) << std::endl;
    }

    return 0;
}