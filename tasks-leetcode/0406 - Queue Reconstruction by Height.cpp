class Solution {
public:
	std::vector<std::vector<int>> reconstructQueue(std::vector<std::vector<int>>& people) {
		std::sort(people.begin(), people.end(), [](const auto & h1, const auto & h2) {
			return ((h1[0] > h2[0]) || (h1[0] == h2[0] && h1[1] < h2[1]));
		});

		std::list<std::vector<int>> result;
		for (const auto & human : people) {
			int before = human[1];

			auto position = result.begin();
			while (position != result.end() && before > 0) {
				before--;
				position++;
			}

			result.insert(position, human);
		}

		return std::vector<std::vector<int>>(result.begin(), result.end());
	}
};