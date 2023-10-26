#include <iostream>
#include <bitset>
#include <fstream>
#include <memory>
#include <numeric>
#include <string>
#include <vector>
#include <unordered_map>


class hex_parser {
public:
    std::string parse(const std::string& p_input) {
        std::string result;
        result.reserve(p_input.size() * 4);

        for (auto hex : p_input) {
            int ivalue = std::stoi(std::string(1, hex), nullptr, 16);
            std::string svalue = std::bitset<4>(ivalue).to_string();

            result += svalue;
        }

        return result;
    }
};



class package_payload {
public:
    using ptr = std::shared_ptr<package_payload>;

public:
    virtual std::size_t parse(const std::string& p_input, const std::size_t p_position) = 0;
    virtual std::size_t sum_versions() const = 0;
    virtual std::uint64_t evaluate() const = 0;
};


class literal_value;
class operator_value;


class package {
private:
    int m_version = -1;
    int m_typeid = -1;
    package_payload::ptr m_payload;

    std::size_t m_pos = -1;

public:
    std::size_t parse(const std::string& p_input, const std::size_t p_position);

    std::size_t sum_versions() const {
        return m_version + m_payload->sum_versions();
    }

    std::uint64_t evaluate() const {
        return m_payload->evaluate();
    }
};


class literal_value : public package_payload {
private:
    std::vector<std::uint64_t> m_vals;
    std::size_t m_begin = -1;
    std::size_t m_end = -1;

public:
    literal_value() = default;
    virtual ~literal_value() { };

public:
    std::size_t parse(const std::string& p_input, const std::size_t p_position) override {
        m_begin = p_position;
        m_end = p_position;
        while (parse_value(p_input)) { }
        return m_end;
    }

    std::size_t sum_versions() const override {
        return 0;
    }

    std::uint64_t evaluate() const override {
        std::uint64_t result = 0;

        for (std::size_t val: m_vals) {
            result <<= 4;
            result += val;
        }

        return result;
    }

private:
    bool parse_value(const std::string& p_input) {
        bool last_value = (p_input[m_end] == '0');
        m_end++;

        std::string value = p_input.substr(m_end, 4);
        m_end += 4;

        m_vals.push_back(std::stoi(value, nullptr, 2));

        return !last_value;
    }
};


class operator_value : public package_payload {
private:
    int m_typeid = -1;
    std::vector<package> m_packages;
    int m_length_type = -1;
    int m_length = -1;

    std::size_t m_pos = -1;

public:
    operator_value(const int p_typeid) : m_typeid(p_typeid) { }

public:
    std::size_t parse(const std::string& p_input, const std::size_t p_pos) override {
        m_pos = p_pos;

        m_length_type = p_input[m_pos] - '0';
        m_pos++;

        parse_length(p_input);
        parse_packages(p_input);

        return m_pos;
    }

    std::size_t sum_versions() const override {
        std::size_t sum = 0;
        for (const auto& sub_pkg : m_packages) {
            sum += sub_pkg.sum_versions();
        }

        return sum;
    }

    std::uint64_t evaluate() const override {
        return evaluate_packages();
    }

private:
    std::uint64_t evaluate_packages() const {
        std::uint64_t result = m_packages[0].evaluate();

        for (std::size_t i = 1; i < m_packages.size(); i++) {
            std::uint64_t current_value = m_packages[i].evaluate();

            switch (m_typeid) {
            case 0:
                result += current_value;
                break;

            case 1:
                result *= current_value;
                break;

            case 2:
                result = std::min(result, current_value);
                break;

            case 3:
                result = std::max(result, current_value);
                break;

            case 5:
                result = (result > current_value) ? 1 : 0;
                break;

            case 6:
                result = (result < current_value) ? 1 : 0;
                break;

            case 7:
                result = (result == current_value) ? 1 : 0;
                break;

            default:
                throw std::exception("unknown operation type");
            }
        }

        return result;
    }

    void parse_length(const std::string& p_input) {
        int str_len = (m_length_type == 0) ? 15 : 11;
        m_length = std::stoi(p_input.substr(m_pos, str_len), nullptr, 2);
        m_pos += str_len;
    }

    void parse_packages(const std::string& p_input) {
        if (m_length_type == 0) {
            parse_packages_in_bytes(p_input);
        }
        else {
            parse_packages_in_amount(p_input);
        }
    }

    void parse_packages_in_bytes(const std::string& p_input) {
        int begin_position = m_pos;
        while (m_pos - begin_position < m_length) {
            package subpackage;
            m_pos = subpackage.parse(p_input, m_pos);
            m_packages.push_back(subpackage);
        }
    }

    void parse_packages_in_amount(const std::string& p_input) {
        for (std::size_t i = 0; i < m_length; i++) {
            package subpackage;
            m_pos = subpackage.parse(p_input, m_pos);
            m_packages.push_back(subpackage);
        }
    }
};


std::size_t package::parse(const std::string& p_input, const std::size_t p_position) {
    m_pos = p_position;

    m_version = std::stoi(p_input.substr(m_pos, 3), nullptr, 2);
    m_pos += 3;

    m_typeid = std::stoi(p_input.substr(m_pos, 3), nullptr, 2);
    m_pos += 3;

    if (m_typeid == 4) {
        m_payload = std::make_shared<literal_value>();
    }
    else {
        m_payload = std::make_shared<operator_value>(m_typeid);
    }

    m_pos = m_payload->parse(p_input, m_pos);

    return m_pos;
}


std::string receive_transmission() {
    std::ifstream stream("input.txt");

    std::string content;
    std::getline(stream, content);

    return content;
}


int main() {
    const std::string transmission = receive_transmission();
    const std::string hex_content = hex_parser().parse(transmission);

    package main_package;
    main_package.parse(hex_content, 0);

    std::cout << "Sum of all versions: " << main_package.sum_versions() << std::endl;
    std::cout << "Package evaluation: " << main_package.evaluate() << std::endl;

    return 0;
}
