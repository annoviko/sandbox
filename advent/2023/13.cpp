#include <iostream>
#include <fstream>
#include <string>
#include <vector>


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
            std::int64_t horizontal = find_left_reflection(m);
            std::int64_t vertical = find_left_reflection(transpose(m));

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

    std::int64_t find_left_reflection(const std::vector<std::string>& m) {
        std::vector<std::uint64_t> hashes;

        std::vector<std::size_t> left_reflection_indexes;

        for (const std::string& line : m) {
            std::uint64_t hash_value = 0;
            for (std::size_t i = 0; i < line.size(); i++) {
                hash_value <<= 1;

                if (line[i] == '#') {
                    hash_value++;
                }
            }

            if (hashes.size() > 0) {
                if (hashes.back() == hash_value) {
                    left_reflection_indexes.push_back(hashes.size() - 1);    /* add point to consider reflection later */
                }
            }

            hashes.push_back(hash_value);
        }


        std::size_t best_reflection_index = -1;
        std::size_t best_reflection_size = 0;
        std::size_t debug_suitable_reflections = 0;

        for (std::size_t left_index : left_reflection_indexes) {
            std::size_t lborder = left_index;
            std::size_t rborder = left_index + 1;

            std::size_t actual_reflection_size = 0;
            bool is_reflection = true;

            for (; (lborder != -1) && (rborder < hashes.size()); lborder--, rborder++) {
                if (hashes[lborder] != hashes[rborder]) {
                    is_reflection = false;
                    break;  /* it is not reflection */
                }

                actual_reflection_size++;
            }

            if (!is_reflection) {
                continue;   /* it is not reflection */
            }

            if (actual_reflection_size > best_reflection_size) {
                best_reflection_index = left_index;
                best_reflection_size = actual_reflection_size;
                debug_suitable_reflections++;
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

    return 0;
}
