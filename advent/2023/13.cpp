#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>


using mirror_t = std::vector<std::string>;


class solution {
private:
    std::vector<mirror_t> mirrors;

public:
    solution(const std::vector<mirror_t>& _mirrors) : mirrors(_mirrors) { }

public:
    std::int64_t analyse() {
        std::int64_t result = 0;

        for (const auto& m : mirrors) {
            std::int64_t horizontal = find_left_reflection(m, 0);
            std::int64_t vertical = find_left_reflection(transpose(m), 0);

            std::int64_t mirror_score = vertical + horizontal * 100;
            result += mirror_score;
        }

        return result;
    }


    std::int64_t fix_and_analyse() {
        std::int64_t result = 0;

        for (const auto& m : mirrors) {
            std::int64_t horizontal = find_left_reflection(m, 1);
            std::int64_t vertical = find_left_reflection(transpose(m), 1);

            std::int64_t mirror_score = vertical + horizontal * 100;
            result += mirror_score;
        }

        return result;
    }


private:
    mirror_t transpose(const mirror_t& m) {
        mirror_t tm;

        for (std::size_t col = 0; col < m[0].size(); col++) {
            std::string new_row;

            for (std::size_t row = 0; row < m.size(); row++) {
                new_row.push_back(m[row][col]);
            }

            tm.push_back(new_row);
        }

        return tm;
    }


    std::vector<std::uint64_t> build_hash(const std::vector<std::string>& m) {
        std::vector<std::uint64_t> hashes;

        for (const std::string& line : m) {
            std::uint64_t hash_value = 0;
            for (std::size_t i = 0; i < line.size(); i++) {
                hash_value <<= 1;

                if (line[i] == '.') {
                    hash_value++;
                }
            }

            hashes.push_back(hash_value);
        }

        return hashes;
    }


    std::uint64_t count_bits(std::uint64_t value) {
        std::uint64_t counter = 0;
        while (value) {
            value = value & (value - 1);
            counter++;
        }
        return counter;
    }


    bool is_suitable_for_bit_change(const std::uint64_t v1, const std::uint64_t v2) {
        std::uint64_t diff = v1 ^ v2;
        std::uint64_t diff_size = count_bits(diff);

        if (diff_size != 1) {
            return false;
        }

        return true;
    }


    std::uint64_t get_reflection_size(const std::vector<std::uint64_t>& hashes, std::size_t lborder, std::size_t rborder, std::size_t required_difference) {
        std::size_t actual_reflection_size = 0;
        std::size_t patch_applied = false;

        std::size_t difference_counter = 0;

        for (; (lborder != -1) && (rborder < hashes.size()); lborder--, rborder++) {
            const auto v1 = hashes[lborder];
            const auto v2 = hashes[rborder];

            if (v1 != v2) {
                const std::uint64_t diff = v1 ^ v2;
                const std::uint64_t diff_size = count_bits(diff);

                difference_counter += diff_size;
            }

            actual_reflection_size++;
        }

        if (required_difference != difference_counter) {
            return 0;
        }

        return actual_reflection_size;
    }


    std::int64_t find_left_reflection(const std::vector<std::string>& m, const std::size_t required_difference) {
        std::vector<std::uint64_t> hashes = build_hash(m);

        std::size_t best_reflection_index = -1;
        std::size_t best_reflection_size = 0;

        for (std::size_t left_index = 0; left_index + 1 < hashes.size(); left_index++) {
            std::uint64_t actual_reflection_size = get_reflection_size(hashes, left_index, left_index + 1, required_difference);

            if (actual_reflection_size > best_reflection_size) {
                best_reflection_index = left_index;
                best_reflection_size = actual_reflection_size;
            }
        }

        return (best_reflection_index == -1) ? 0 : best_reflection_index + 1;   /* indexing from 1, not from 0 */
    }
};


std::vector<mirror_t> read_input() {
    std::fstream stream("input.txt");

    std::vector<mirror_t> result;

    mirror_t current_mirror;
    for (std::string line; std::getline(stream, line);) {
        if (line.empty() && !current_mirror.empty()) {
            result.push_back(std::move(current_mirror));
            continue;
        }

        current_mirror.push_back(line);
    }

    if (!current_mirror.empty()) {
        result.push_back(std::move(current_mirror));
    }

    return result;
}


int main() {
    auto input = read_input();

    std::cout << "The number after summarizing all of your notes: " << solution(input).analyse() << std::endl;
    std::cout << "The number after summarizing all of your notes (with new reflection line): " << solution(input).fix_and_analyse() << std::endl;

    return 0;
}
