#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using std::string;

int edit_distance(const string &str1, const string &str2) {
    std::vector<std::vector<int>> dp(str1.size() + 1, std::vector<int>(str2.size() + 1, 0));

    for (int i = 0; i < str1.size() + 1; i++) {
        for (int j = 0; j < str2.size() + 1; j++) {
            if (i == 0 && j == 0) {
                dp[i][j] = 0;
                continue;
            }

            int value = std::numeric_limits<int>::max();

            if (i - 1 >= 0) {
                value = std::min(dp[i - 1][j] + 1, value);
            }

            if (j - 1 >= 0) {
                value = std::min(dp[i][j - 1] + 1, value);
            }

            if (i - 1 >= 0 && j - 1 >= 0) {
                int cost = (i - 1 < str1.size() && j - 1 < str2.size() && str1[i - 1] == str2[j - 1]) ? 0 : 1;
                value = std::min(dp[i - 1][j - 1] + cost, value);
            }

            dp[i][j] = value;
        }
    }

    //for (auto & row : dp) {
    //	for (auto elem : row) {
    //		std::cout << elem << " ";
    //	}
    //	std::cout << std::endl;
    //}

    return dp.back().back();
}

int main() {
    string str1;
    string str2;
    std::cin >> str1 >> str2;
    std::cout << edit_distance(str1, str2) << std::endl;
    return 0;
}
