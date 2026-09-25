#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <variant>
#include <vector>


enum type_t {
    INT = 0,
    LIST,
    UNKNOWN
};


enum compare_t {
    LESS = 0,
    EQUAL,
    GREATER
};


class packet_t {
private:
    type_t type = type_t::UNKNOWN;
    std::vector<void*> content;

public:
    packet_t() = default;
    packet_t(const std::string& p_str, std::size_t& p_pos) {
        if (p_str[p_pos] == '[') {
            p_pos++;
        }

        type = type_t::LIST;

        while (p_str[p_pos] != ']') {
            if (p_str[p_pos] == '[') {
                packet_t* list = new packet_t(p_str, p_pos);  /* nested package */
                content.push_back(list);
            }
            else if (std::isdigit(p_str[p_pos])) {
                packet_t* val = new packet_t();

                val->type = type_t::INT;

                const int int_len = get_integer_length(p_str, p_pos);
                const std::string int_str = p_str.substr(p_pos, int_len);
                const int value = std::stoi(int_str);

                val->content.push_back((void*)value);

                p_pos += int_len;
                content.push_back(val);
            }
            else {
                p_pos++;    /* comma */
            }
        }

        p_pos++;    /* list is over - move further */
    }

public:
    static packet_t* create_divider_packet(const std::string& p_str) {
        std::size_t position = 0;
        return new packet_t{ p_str, position };
    }

private:
    int get_integer_length(const std::string& p_str, std::size_t p_pos) {
        int len = 0;
        while (std::isdigit(p_str[p_pos])) {
            len++;
            p_pos++;
        }

        return len;
    }

    compare_t compare(const packet_t& p_other) const {
        const int len = std::min(content.size(), p_other.content.size());

        for (int i = 0; i < len; i++) {
            packet_t* lp_ptr = (packet_t*)content[i];
            packet_t* rp_ptr = (packet_t*)p_other.content[i];

            if ((lp_ptr->type == type_t::INT) && (rp_ptr->type == type_t::INT)) {
                const int lp_val = (int)lp_ptr->content[0];
                const int rp_val = (int)rp_ptr->content[0];

                if (lp_val < rp_val) {
                    return compare_t::LESS;
                }
                else if (lp_val > rp_val) {
                    return compare_t::GREATER;
                }

                continue;
            }

            packet_t int_wrapper;
            int_wrapper.type = type_t::LIST;

            if (lp_ptr->type == type_t::INT) {
                int_wrapper.content.push_back(lp_ptr);  /* wrap integer to list */
                lp_ptr = &int_wrapper;
            }
            else if (rp_ptr->type == type_t::INT) {
                int_wrapper.content.push_back(rp_ptr);  /* wrap integer to list */
                rp_ptr = &int_wrapper;
            }

            if (*lp_ptr < *rp_ptr) {
                return compare_t::LESS;
            }
            else if (*lp_ptr > *rp_ptr) {
                return compare_t::GREATER;
            }
        }

        /* all elements are equal and smaller - lets check if left length is smaller than right one */
        if (content.size() < p_other.content.size()) {
            return compare_t::LESS;
        }
        else if (content.size() > p_other.content.size()) {
            return compare_t::GREATER;
        }

        return compare_t::EQUAL;
    }

public:
    bool operator!=(const packet_t& p_other) {
        return compare(p_other) != compare_t::EQUAL;
    }

    bool operator==(const packet_t& p_other) {
        return compare(p_other) == compare_t::EQUAL;
    }

    bool operator>(const packet_t& p_other) {
        return compare(p_other) == compare_t::GREATER;
    }

    bool operator<(const packet_t& p_other) {
        return compare(p_other) == compare_t::LESS;
    }

    bool operator<=(const packet_t& p_other) {
        return compare(p_other) != compare_t::GREATER;
    }
};


std::vector<packet_t> read_input() {
    std::ifstream stream("input.txt");

    std::vector<packet_t> packages;
    for (std::string line; std::getline(stream, line);) {
        if (line.empty()) {
            continue;
        }

        std::size_t position = 0;
        packet_t package{ line, position };

        packages.push_back(package);
    }

    return packages;
}


int main() {
    auto packages = read_input();

    int correct_order_score = 0;
    for (int i = 1; i < packages.size(); i += 2) {
        const int index_pair = (i >> 1) + 1;

        if (packages[i - 1] <= packages[i]) {
            correct_order_score += index_pair;
        }
    }

    std::cout << "Sum of the indices of ordered pairs: " << correct_order_score << std::endl;

    packet_t* divider1 = packet_t::create_divider_packet("[[2]]");
    packet_t* divider2 = packet_t::create_divider_packet("[[6]]");

    packages.push_back(*divider1);
    packages.push_back(*divider2);

    std::sort(packages.begin(), packages.end());

    int divider_position1 = -1;
    int divider_position2 = -1;

    for (int i = 0; i < packages.size(); i++) {
        if (*divider1 == packages[i]) {
            divider_position1 = i + 1;
        }
        else if (*divider2 == packages[i]) {
            divider_position2 = i + 1;
            break;
        }
    }

    const int decoder_key = divider_position1 * divider_position2;
    std::cout << "The decoder key for the distress signal: " << decoder_key << std::endl;

    return 0;
}