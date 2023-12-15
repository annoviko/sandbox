#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>


struct lens {
    std::string  label;
    std::int64_t focal_length;

public:
    friend bool operator==(const lens& l, const std::string& r) {
        return (l.label == r);
    }

    bool operator==(const lens& other) const {
        return (label == other.label);
    }
};


std::vector<std::string> read_input() {
    std::vector<std::string> result;

    std::fstream stream("input.txt");

    std::string line;
    std::getline(stream, line);

    std::string str;
    for (int i = 0; i < line.size(); i++) {
        if ((line[i] == ',') && !str.empty()) {
            result.push_back(std::move(str));
        }
        else {
            str.push_back(line[i]);
        }
    }

    if (!str.empty()) {
        result.push_back(std::move(str));
    }

    return result;
}


std::int64_t hash_counter(const std::string& str) {
    std::int64_t current_value = 0;

    for (char c : str) {
        current_value = ((current_value + c) * 17) % 256;
    }

    return current_value;
}


std::int64_t hash_sum(const std::vector<std::string>& strs) {
    std::int64_t sum = 0;

    for (const auto& str : strs) {
        sum += hash_counter(str);
    }

    return sum;
}


class focus_power_analyser {
private:
    std::vector<std::string> codes;
    std::vector<std::list<lens>> boxes = std::vector<std::list<lens>>(256);

public:
    focus_power_analyser(const std::vector<std::string>& _codes) : codes(_codes) { }

public:
    std::int64_t analyse() {
        for (const auto& code : codes) {
            handle_entry(code);
        }

        std::int64_t total_focusing = 0;

        for (std::int64_t i = 0; i < boxes.size(); i++) {
            std::int64_t slot_counter = 1;
            for (auto& unit : boxes[i]) {
                total_focusing += (i + 1) * slot_counter * unit.focal_length;
                slot_counter++;
            }
        }

        return total_focusing;
    }

private:
    void handle_entry(const std::string& expression) {
        std::size_t p = expression.find('-');
        if (p != std::string::npos) {
            std::string label = expression.substr(0, p);
            handle_lens_dash({ label, -1 });
        }
        else {
            p = expression.find('=');

            std::string label = expression.substr(0, p);
            std::int64_t focal_length = std::stoll(expression.substr(p + 1));

            handle_lens_equal({ label, focal_length });
        }
    }


    void handle_lens_dash(const lens& unit) {
        /* go to the relevant box */
        auto box_index = get_box_index(unit);
        auto& box = boxes[box_index];

        /* remove the lens with the given label if it is present in the box */
        auto iter = std::find(box.begin(), box.end(), unit.label);
        while (iter != box.end()) {
            iter = box.erase(iter);
            iter = std::find(iter, box.end(), unit.label);
        }
    }


    void handle_lens_equal(const lens& unit) {
        /* needs to go into the relevant box */
        auto box_index = get_box_index(unit);
        auto& box = boxes[box_index];

        /* if there is already a lens in the box with the same label */
        auto iter = std::find(box.begin(), box.end(), unit.label);
        if (iter != box.end()) {
            /* replace the old lens with the new lens */
            iter->focal_length = unit.focal_length;
        }
        else {
            /* if there is not already a lens in the box with the same label, add
            the lens to the box immediately behind any lenses already in the box */
            box.push_back(unit);
        }
    }


    std::int64_t get_box_index(const lens& unit) {
        return hash_counter(unit.label);
    }
};


int main() {
    auto input = read_input();

    std::cout << "The sum of the hashes: " << hash_sum(input) << std::endl;
    std::cout << "The focusing power of the resulting lens configuration: " << focus_power_analyser(input).analyse() << std::endl;

    return 0;
}
