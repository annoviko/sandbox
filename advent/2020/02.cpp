#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <regex>


class password_policy {
public:
    enum policy {
        initial,
        official
    };

private:
    char m_letter;
    std::size_t m_min;
    std::size_t m_max;

public:
    password_policy(const char p_letter, const std::size_t p_min, const std::size_t p_max) :
        m_letter(p_letter), m_min(p_min), m_max(p_max)
    { }

public:
    bool is_valid(const std::string & p_password, const password_policy::policy & p_policy) const {
        switch (p_policy) {
        case policy::initial: return is_valid_old_policy(p_password);
        case policy::official: return is_valid_new_policy(p_password);
        default: throw std::invalid_argument("Invalid policy is specified.");
        }
    }

private:
    bool is_valid_old_policy(const std::string & p_password) const {
        std::size_t appearance = 0;
        for (auto letter : p_password) {
            if (letter == m_letter) {
                appearance++;
            }
        }

        return appearance >= m_min && appearance <= m_max;
    }

    bool is_valid_new_policy(const std::string & p_password) const {
        if (m_min > p_password.size() || m_max > p_password.size()) {
            return false;
        }

        if (p_password[m_min - 1] == p_password[m_max - 1]) {
            return false;
        }

        if ((p_password[m_min - 1] == m_letter) || (p_password[m_max - 1] == m_letter)) {
            return true;
        }

        return false;
    }
};


struct database_entry {
public:
    password_policy m_policy;
    std::string     m_password;

public:
    bool is_valid(const password_policy::policy p_policy) const {
        return m_policy.is_valid(m_password, p_policy);
    }
};


std::list<database_entry> read_database() {
    std::ifstream stream("input.txt");

    std::list<database_entry> result;

    for (std::string line; std::getline(stream, line); ) {
        std::regex pattern("(\\d+)-(\\d+) (\\S): (\\S+)");

        std::smatch groups;
        if (!std::regex_match(line, groups, pattern)) {
            std::invalid_argument("Impossible to parse database entry: '" + line + "'.");
        }

        result.push_back({ password_policy(groups[3].str()[0], std::stoull(groups[1].str()), std::stoull(groups[2].str())), groups[4].str() });
    }

    return result;
}


class password_database {
private:
    std::list<database_entry> m_db;

public:
    password_database() {
        load();
    }

public:
    std::size_t count_valid_passwords(const password_policy::policy p_policy) {
        std::size_t result = 0;
        for (auto & entry : m_db) {
            if (entry.is_valid(p_policy)) {
                result++;
            }
        }

        return result;
    }

private:
    void load() {
        std::ifstream stream("input.txt");

        for (std::string line; std::getline(stream, line); ) {
            std::regex pattern("(\\d+)-(\\d+) (\\S): (\\S+)");

            std::smatch groups;
            if (!std::regex_match(line, groups, pattern)) {
                std::invalid_argument("Impossible to parse database entry: '" + line + "'.");
            }

            m_db.push_back({ password_policy(groups[3].str()[0], std::stoull(groups[1].str()), std::stoull(groups[2].str())), groups[4].str() });
        }
    }
};



int main() {
    password_database database;
    std::cout << "The amount of valid passwords: " << database.count_valid_passwords(password_policy::initial) << std::endl;
    std::cout << "The amount of valid passwords (official): " << database.count_valid_passwords(password_policy::official) << std::endl;
    return 0;
}