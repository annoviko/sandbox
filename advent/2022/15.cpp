#include <iostream>
#include <fstream>
#include <regex>
#include <set>
#include <string>
#include <vector>


struct position_t {
    std::int64_t x = 0;
    std::int64_t y = 0;
};


struct link_t {
    position_t sensor;
    position_t beacon;
};


std::vector<link_t> read_input() {
    std::ifstream stream("input.txt");

    std::vector<link_t> in;
    for (std::string line; std::getline(stream, line);) {
        position_t s, b;

        /* Sensor at x=2619948, y=2439745: closest beacon is at x=2537630, y=2793941 */
        std::regex pattern("Sensor at x=(-?\\d+), y=(-?\\d+): closest beacon is at x=(-?\\d+), y=(-?\\d+)");
        std::smatch match;

        std::regex_match(line, match, pattern);
        
        s.x = std::stoll(match[1].str());
        s.y = std::stoll(match[2].str());

        b.x = std::stoll(match[3].str());
        b.y = std::stoll(match[4].str());

        in.push_back({ s, b });
    }

    return in;
}


class solution {
private:
    std::vector<link_t> links;

public:
    solution(const std::vector<link_t>& l) : links(l) {}

public:
    std::int64_t count_positions(const std::int64_t y) {
        std::set<std::pair<std::int64_t, std::int64_t>> existed_beacons;

        for (const auto& l : links) {
            if (l.beacon.y == y) {
                existed_beacons.insert({ l.beacon.x, l.beacon.y });
            }
        }

        auto merged_intervals = eval_intervals(y);

        std::int64_t counter = 0;
        for (const auto& interval : merged_intervals) {
            const std::int64_t dist = interval.second - interval.first + 1;
            counter += dist;
        }

        return counter - existed_beacons.size();
    }

    std::int64_t find_beacon(const std::int64_t min, const std::int64_t max) {
        for (std::int64_t y = min; y <= max; y++) {
            const auto intervals = eval_intervals(y);

            for (int j = 0; j < intervals.size(); j++) {
                if (intervals[j].first > min) {  /* the very first cell is our beacon */
                    std::int64_t x = intervals[j].first - 1;
                    return x * 4000000 + y;
                }
                else if (intervals[j].second < max) {
                    std::int64_t x = intervals[j].second + 1;
                    return x * 4000000 + y;
                }
            }
        }
    }

private:
    std::vector<std::pair<std::int64_t, std::int64_t>> eval_intervals(const std::int64_t y) {
        std::vector<std::pair<std::int64_t, std::int64_t>> intervals;

        for (const auto& l : links) {
            const std::int64_t radius = std::abs(l.sensor.x - l.beacon.x) + std::abs(l.sensor.y - l.beacon.y);
            const std::int64_t target_radius = radius - std::abs(l.sensor.y - y);

            if (target_radius >= 0) {
                const std::int64_t from = l.sensor.x - target_radius;   /* including */
                const std::int64_t to = l.sensor.x + target_radius;     /* including */

                intervals.push_back({ from, to });
            }
        }

        if (intervals.empty()) {
            return {};
        }

        std::sort(intervals.begin(), intervals.end());

        /* merge */
        std::vector<std::pair<std::int64_t, std::int64_t>> merged_intervals;
        merged_intervals.push_back(intervals[0]);

        for (int i = 1; i < intervals.size(); i++) {
            if (intervals[i].first <= merged_intervals.back().second) {
                if (merged_intervals.back().second < intervals[i].second) {
                    merged_intervals.back().second = intervals[i].second;   /* extend */
                }
            }
            else {
                merged_intervals.push_back(intervals[i]);
            }
        }

        return merged_intervals;
    }
};


int main() {
    auto in = read_input();

    std::cout << "The number of position where beacon cannot exist: " << solution(in).count_positions(2000000) << std::endl;
    std::cout << "The beacon's tuning frequency: " << solution(in).find_beacon(0, 4000000) << std::endl;

    return 0;
}