#include <algorithm>
#include <iostream>
#include <vector>
#include <set>
#include <numeric>

using std::vector;


bool take_items(const int target_price, std::vector<int> & items) {
	std::vector<std::vector<int>> dp(items.size() + 1, std::vector<int>(target_price + 1, 0));

	for (std::size_t index_item = 1; index_item < dp.size(); index_item++) {
		const int item_price = items[index_item - 1];
		for (std::size_t price = 1; price < dp.front().size(); price++) {
			int cur_price = dp[index_item - 1][price];
			if (item_price <= price) {
				cur_price = std::max(item_price + dp[index_item - 1][price - item_price], cur_price);
			}

			dp[index_item][price] = cur_price;
		}
	}

	if (dp.back().back() != target_price) {
		return false;
	}

	std::multiset<std::size_t> to_remove = { };
	int price = target_price;
	std::size_t last_added_index = items.size();

	while (price != 0) {
		int item_price = items[last_added_index - 1];

		int price_ignore = dp[last_added_index - 1][price];
		int price_take = (item_price > price) ? std::numeric_limits<int>::min() : dp[last_added_index - 1][price - item_price];

		if (price_take + item_price >= price_ignore) {
			price -= item_price;
			to_remove.insert(items[last_added_index - 1]);
		}

		last_added_index--;
	}

	std::vector<int> remaining_items;
	remaining_items.reserve(items.size());

	for (auto item : items) {
		auto iter = to_remove.find(item);
		if (iter == to_remove.end()) {
			remaining_items.push_back(item);
		}
		else {
			to_remove.erase(iter);
		}
	}

	items = std::move(remaining_items);
	return true;
}


int partition3(vector<int> &A) {
	int total_price = std::accumulate(A.begin(), A.end(), 0);
	if (total_price % 3 != 0) {
		return 0;
	}

	int target_price = total_price / 3;

	bool possible = true;
	std::sort(A.begin(), A.end());
	for (std::size_t i = 0; (i < 3) && (possible); i++) {
		possible &= take_items(target_price, A);
	}

	return possible ? 1 : 0;
}

int main() {
	int n;
	std::cin >> n;
	vector<int> A(n);
	for (size_t i = 0; i < A.size(); ++i) {
		std::cin >> A[i];
	}
	std::cout << partition3(A) << '\n';
}
