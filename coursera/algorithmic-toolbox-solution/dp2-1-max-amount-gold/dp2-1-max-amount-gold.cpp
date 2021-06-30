#include <algorithm>
#include <iostream>
#include <vector>

using std::vector;

int optimal_weight(int W, const vector<int> &w) {
	std::vector<std::vector<int>> dp(w.size() + 1, std::vector<int>(W + 1, 0));

	for (int index_item = 1; index_item < dp.size(); index_item++) {
		const int item_weight = w[index_item - 1];
		for (int weight = 1; weight < dp[index_item].size(); weight++) {
			int current_weight = dp[index_item - 1][weight];
			if (weight >= w[index_item - 1]) {	/* current element could be added */
				current_weight = std::max(item_weight + dp[index_item - 1][weight - item_weight], current_weight);
			}

			dp[index_item][weight] = current_weight;
		}
	}

	return dp.back().back();
}

int main() {
	int n, W;
	std::cin >> W >> n;
	vector<int> w(n);
	for (int i = 0; i < n; i++) {
		std::cin >> w[i];
	}
	std::cout << optimal_weight(W, w) << '\n';
}
