#include <algorithm>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>


struct entry {
public:
    std::int64_t dst_lo;
    std::int64_t src_lo;
    std::int64_t len;
};


struct range {
public:
    std::int64_t from = -1;
    std::int64_t to = -1;
};

const std::vector<std::string> TABLE_NAMES = {
    "seed-to-soil",
    "soil-to-fertilizer",
    "fertilizer-to-water",
    "water-to-light",
    "light-to-temperature",
    "temperature-to-humidity",
    "humidity-to-location"
};


struct map {
public:
    std::vector<std::int64_t> seeds;

    std::vector<entry> seed_to_soil;
    std::vector<entry> soil_to_fertilizer;
    std::vector<entry> fertilizer_to_water;
    std::vector<entry> water_to_light;
    std::vector<entry> light_to_temperature;
    std::vector<entry> temperature_to_humidity;
    std::vector<entry> humidity_to_location;

public:
    std::int64_t get_number_tables() const {
        return 7;
    }

    const std::vector<entry>& get_table_by_index(const std::int64_t index) const {
        switch (index) {
        case 0: return seed_to_soil;
        case 1: return soil_to_fertilizer;
        case 2: return fertilizer_to_water;
        case 3: return water_to_light;
        case 4: return light_to_temperature;
        case 5: return temperature_to_humidity;
        case 6: return humidity_to_location;
        default: throw std::runtime_error("index is out of range");
        }
    }

    std::vector<entry>& get_table_by_name(const std::string& provided_name) {
        if (is_name("seed-to-soil", provided_name)) {
            return seed_to_soil;
        }
        else if (is_name("soil-to-fertilizer", provided_name)) {
            return soil_to_fertilizer;
        }
        else if (is_name("fertilizer-to-water", provided_name)) {
            return fertilizer_to_water;
        }
        else if (is_name("water-to-light", provided_name)) {
            return water_to_light;
        }
        else if (is_name("light-to-temperature", provided_name)) {
            return light_to_temperature;
        }
        else if (is_name("temperature-to-humidity", provided_name)) {
            return temperature_to_humidity;
        }
        else if (is_name("humidity-to-location", provided_name)) {
            return humidity_to_location;
        }
        else {
            throw std::exception("incorrect table name");
        }
    }

private:
    bool is_name(const std::string& p_substr, const std::string& p_fullname) const {
        return p_fullname.find(p_substr) != std::string::npos;
    }
};


map read_input() {
    std::fstream stream("input.txt");

    std::string line;
    std::getline(stream, line);

    std::stringstream ss(line);
    ss >> line;

    map result;
    while (ss.good()) {
        std::int64_t id;
        ss >> id;

        result.seeds.push_back(id);
    }

    std::string current_table_name;
    while (getline(stream, line)) {
        if (line.find("map") != std::string::npos) {
            current_table_name = line;
            continue;
        }
        else if (line.empty()) {
            continue;
        }

        std::vector<entry>& table = result.get_table_by_name(current_table_name);
        ss = std::stringstream(line);

        std::int64_t dst, src, len;
        ss >> dst >> src >> len;
        table.push_back({ dst, src, len });
    }

    return result;
}


class almanac {
private:
    map m_content;

public:
    almanac(const map& p_content) : m_content(p_content) { }

public:
    std::int64_t find_lowest_location_number() const {
        std::int64_t lowest_location = std::numeric_limits<std::int64_t>::max();

        for (auto id_seed : m_content.seeds) {
            std::int64_t cur_id = id_seed;

            for (int index = 0; index < m_content.get_number_tables(); index++) {
                auto& collection = m_content.get_table_by_index(index);
                cur_id = find_dst_id_by_id(cur_id, collection);
            }

            lowest_location = std::min(cur_id, lowest_location);
        }

        return lowest_location;
    }

    std::int64_t find_lowest_location_number_among_ranges() const {
        std::int64_t lowest_location = std::numeric_limits<std::int64_t>::max();

        for (int i = 0; i < m_content.seeds.size(); i += 2) {
            range seed_range;
            seed_range.from = m_content.seeds[i];
            seed_range.to = seed_range.from + m_content.seeds[i + 1];

            std::vector<range> cur_ranges = { seed_range };

            for (int index = 0; index < m_content.get_number_tables(); index++) {
                auto& collection = m_content.get_table_by_index(index);
                cur_ranges = find_ranges_from_entries(cur_ranges, collection);
            }

            for (const auto& r : cur_ranges) {
                lowest_location = std::min(r.from, lowest_location);
            }
        }

        return lowest_location;
    }

private:
    std::int64_t find_dst_id_by_id(const std::int64_t id, const std::vector<entry>& collection) const {
        std::int64_t dst_id = -1;

        for (const auto& c : collection) {
            std::int64_t from = c.src_lo, to = c.src_lo + c.len;

            if (id >= from && id < to) {
                const std::int64_t offset = c.dst_lo - c.src_lo;
                const std::int64_t candidate = id + offset;
                if (dst_id != -1) {
                    throw std::runtime_error("two ranges cannot have the same id");
                }

                dst_id = candidate;
            }
        }

        if (dst_id == -1) {
            /* any source numbers that aren't mapped correspond to the same destination number. */
            return id;
        }

        return dst_id;
    }


    std::vector<range> find_ranges_from_entries(const std::vector<range>& ranges, const std::vector<entry>& collection) const {
        std::vector<range> result;

        for (auto& r : ranges) {
            std::vector<range> ranges_for_range = find_dst_ranges_from_entries_for_single_range(r, collection);
            if (!ranges_for_range.empty()) {
                result.insert(result.end(), ranges_for_range.begin(), ranges_for_range.end());
            }
        }

        return result;
    }


    std::vector<range> find_dst_ranges_from_entries_for_single_range(const range& r, const std::vector<entry>& collection) const {
        std::vector<range> result;
        std::vector<range> mapped_sources;

        for (auto& c : collection) {
            std::int64_t from = c.src_lo, to = c.src_lo + c.len;

            /* check if there is an intersection between ranges */
            if (((r.from < from) && (r.to <= from)) || ((r.from >= to) && (r.to > to))) {
                continue; /* no intersection */
            }

            std::int64_t new_from = std::max(r.from, from);
            std::int64_t new_to = std::min(r.to, to);

            /* find destination range */
            std::int64_t from_offset = new_from - from;
            std::int64_t length = new_to - new_from;

            std::int64_t new_dst_from = c.dst_lo + from_offset;
            std::int64_t new_dst_to = new_dst_from + length;

            result.push_back({ new_dst_from, new_dst_to });
            mapped_sources.push_back({ new_from, new_to });
        }

        if (mapped_sources.empty()) {
            return { r };   /* nothing intersected at all - return as it is */
        }

        std::sort(mapped_sources.begin(), mapped_sources.end(), [](range& l, range& r) { return l.from < r.from; });

        std::vector<range> direct_ranges;
        std::int64_t cur_from = r.from;
        for (auto& cur_range : mapped_sources) {
            if (cur_from < cur_range.from) {
                direct_ranges.push_back({ cur_from, cur_range.from });
            }

            cur_from = cur_range.to;
        }

        if (cur_from < r.to) {
            direct_ranges.push_back({ cur_from, r.to });
        }

        result.insert(result.end(), direct_ranges.begin(), direct_ranges.end());

        return result;
    }
};


int main() {
    auto input_dict = read_input();

    std::cout << "The lowest location number that corresponds to any of the initial seed numbers: " << almanac(input_dict).find_lowest_location_number() << std::endl;
    std::cout << "The lowest location number that corresponds to any of the initial seed numbers (ranges): " << almanac(input_dict).find_lowest_location_number_among_ranges() << std::endl;

    return 0;
}