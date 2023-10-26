#include <iostream>
#include <fstream>
#include <functional>
#include <string>
#include <list>
#include <regex>
#include <unordered_set>
#include <unordered_map>


enum class validation {
    light,
    deep
};


struct passport {
private:
    using validator = std::function<bool(const std::string &)>;

    const static std::unordered_set<std::string> mandatory_fields;
    const static std::unordered_map<std::string, validator> valildator_fields;

private:
    std::unordered_map<std::string, std::string> m_content;

public:
    void add_info(const std::string & p_key, const std::string & p_value) {
        m_content[p_key] = p_value;
    }

public:
    bool is_valid(const validation & p_rule) const {
        switch (p_rule) {
        case validation::light: return is_light_valid();
        case validation::deep: return is_deep_valid();
        default: throw std::invalid_argument("Not supported rule is specified.");
        }
    }

private:
    bool is_light_valid() const {
        for (const auto & field : passport::mandatory_fields) {
            if (m_content.find(field) == m_content.end()) {
                return false;
            }
        }

        return true;
    }

    bool is_deep_valid() const {
        for (const auto & field : passport::mandatory_fields) {
            auto pair = m_content.find(field);
            if (pair == m_content.end()) {
                return false;
            }

            const auto & checker = valildator_fields.at(pair->first);
            if (!checker(pair->second)) {
                return false;
            }
        }

        return true;
    }

private:
    static bool is_valid_byr(const std::string & p_value) {
        std::size_t year = std::stoull(p_value);
        return (year >= 1920 && year <= 2002);
    }

    static bool is_valid_iyr(const std::string & p_value) {
        std::size_t year = std::stoull(p_value);
        return (year >= 2010 && year <= 2020);
    }

    static bool is_valid_eyr(const std::string & p_value) {
        std::size_t year = std::stoull(p_value);
        return (year >= 2020 && year <= 2030);
    }

    static bool is_valid_hgt(const std::string & p_value) {
        std::regex pattern("(\\d+)(\\S+)");
        std::smatch groups;
        if (!std::regex_match(p_value, groups, pattern)) { 
            return false;
        }

        const std::size_t height = std::stoull(groups[1].str());
        const std::string measure = groups[2].str();
        if (measure == "cm") {
            return (height >= 150 && height <= 193);
        }
        else if (measure == "in") {
            return (height >= 59 && height <= 76);
        }

        return false;
    }

    static bool is_valid_hcl(const std::string & p_value) {
        std::regex pattern("#[0-9a-f]{6}");
        std::smatch groups;
        if (!std::regex_match(p_value, groups, pattern)) {
            return false;
        }

        return true;
    }

    static bool is_valid_ecl(const std::string & p_value) {
        static std::unordered_set<std::string> colors = { "amb", "blu", "brn", "gry", "grn", "hzl", "oth" };
        return colors.find(p_value) != colors.end();
    }

    static bool is_valid_pid(const std::string & p_value) {
        return (p_value.size() == 9);
    }

    static bool is_valid_cid(const std::string & p_value) {
        (void) p_value;
        return true;
    }
};


const std::unordered_set<std::string> passport::mandatory_fields = {
    "byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"
};


const std::unordered_map<std::string, passport::validator> passport::valildator_fields = {
    { "byr", passport::is_valid_byr },
    { "iyr", passport::is_valid_iyr },
    { "eyr", passport::is_valid_eyr },
    { "hgt", passport::is_valid_hgt },
    { "hcl", passport::is_valid_hcl },
    { "ecl", passport::is_valid_ecl },
    { "pid", passport::is_valid_pid }
};


std::list<passport> read_passports() {
    std::ifstream stream("input.txt");

    std::list<passport> passports;

    auto add_info = [](const std::string & p_info, passport & p_passport) {
        p_passport.add_info(p_info.substr(0, 3), p_info.substr(4));
    };

    passport current_passport;
    for (std::string line; std::getline(stream, line); ) {
        if (line.empty()) {
            passports.push_back(current_passport);
            current_passport = passport();
            continue;
        }

        std::size_t lborder = 0;
        std::size_t rborder = line.find(" ");
        while (rborder != std::string::npos) {
            add_info(line.substr(lborder, rborder - lborder), current_passport);

            lborder = rborder + 1;
            rborder = line.find(" ", rborder + 1);
        }

        add_info(line.substr(lborder), current_passport);
    }

    passports.push_back(current_passport);
    return passports;
}


std::size_t count_valid_passport(const std::list<passport> & p_passports, const validation p_rule) {
    std::size_t result = 0;
    for (const auto & doc : p_passports) {
        if (doc.is_valid(p_rule)) {
            result++;
        }
    }

    return result;
}


int main() {
    const auto passports = read_passports();
    std::cout << "The amount of valid passports (light checking): " << count_valid_passport(passports, validation::light) << std::endl;
    std::cout << "The amount of valid passports (deep checking): " << count_valid_passport(passports, validation::deep) << std::endl;

    return 0;
}
