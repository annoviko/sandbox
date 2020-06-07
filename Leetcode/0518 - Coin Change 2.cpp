class Solution {
public:
	int change(int amount, std::vector<int>& coins) {
		std::vector<std::vector<int>> dp(coins.size() + 1, std::vector<int>(amount + 1, 0));
		for (auto & row : dp) {
			row.at(0) = 1;
		}

		for (int i = 1; i < dp.size(); i++) {
			for (int j = 1; j < dp.at(0).size(); j++) {
				int coin = coins[i - 1];
				dp[i][j] = dp[i - 1][j];
				if (j - coin >= 0) {
					dp[i][j] += dp[i][j - coin];
				}
			}
		}

		return dp.back().back();
	}
};