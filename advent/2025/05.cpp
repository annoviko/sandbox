#include <algorithm>
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>


class solution {
private:
    std::vector<std::pair<std::uint64_t, std::uint64_t>> ranges;
    std::vector<std::uint64_t> ids;

public:
    solution(const std::vector<std::pair<std::uint64_t, std::uint64_t>>& p_ranges, const std::vector<std::uint64_t>& p_ids) :
        ranges(p_ranges),
        ids(p_ids)
    {
        /* Complexity: O(N * log(N) + N) = O(N * log(N)) */

        std::sort(ranges.begin(), ranges.end(), [](const auto& l, const auto& r) {
            if (l.first < r.first) {
                return true;
            }
            else if (l.second == r.second) {
                return l.second < r.second;
            }
            else {
                return false;
            }
        });

        merge();
    }

public:
    std::uint64_t count_fresh_ingredients() const {
        /* Complexity: O(K * log(N)) */

        std::uint64_t counter = 0;
        for (std::uint64_t id : ids) {
            if (is_fresh(id)) {
                counter++;
            }
        }
        return counter;
    }

    std::uint64_t count_possible_fresh_ingredients() const {
        /* Complexity: O(N) */

        std::uint64_t counter = 0;
        for (const auto& r : ranges) {
            counter += (r.second - r.first + 1);
        }
        return counter;
    }

private:
    bool is_fresh(std::uint64_t id) const {
        /* Complexity: O(log(K)) */

        auto iter = std::upper_bound(ranges.begin(), ranges.end(), id, [](std::uint64_t v, const auto& l) {
            return v < l.first;
        });

        if (iter == ranges.begin()) {
            return false;
        }

        iter--;
        return (iter->second >= id);
    }

    void merge() {
        decltype(ranges) new_ranges;
        new_ranges.push_back(ranges[0]);

        for (int i = 1; i < ranges.size(); i++) {
            const auto from = ranges[i].first;
            const auto to = ranges[i].second;

            if (from <= new_ranges.back().second) {
                new_ranges.back().second = std::max(to, new_ranges.back().second);
            }
            else {
                new_ranges.push_back({ from, to });
            }
        }

        ranges = std::move(new_ranges);
    }
};


std::pair<std::vector<std::pair<std::uint64_t, std::uint64_t>>, std::vector<std::uint64_t>> read_input() {
    std::fstream stream("input.txt");
    std::vector<std::pair<std::uint64_t, std::uint64_t>> ranges;  /* [from, to] */

    for (std::string line; std::getline(stream, line);) {
        if (line.empty()) {
            break;
        }

        std::size_t delim = line.find('-');
        std::uint64_t from = std::stoull(line.substr(0, delim));
        std::uint64_t to = std::stoull(line.substr(delim + 1));

        ranges.push_back({ from, to });
    }

    std::vector<std::uint64_t> ids;
    for (std::string line; std::getline(stream, line);) {
        ids.push_back(std::stoull(line));
    }

    return { ranges, ids };
}


int main() {
    auto input = read_input();

    auto solver = solution(input.first, input.second);

    std::cout << "The number of fresh ingredients: " << solver.count_fresh_ingredients() << std::endl;
    std::cout << "The number of possible fresh ingredients: " << solver.count_possible_fresh_ingredients() << std::endl;

    return 0;
}
