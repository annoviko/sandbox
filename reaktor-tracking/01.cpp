#include <iostream>
#include <fstream>
#include <string>
#include <vector>


std::string read(const std::string & p_file) {
    std::fstream stream(p_file);

    std::string result;
    std::getline(stream, result);

    return result;
}


class password_finder {
private:
    constexpr static std::size_t MESSAGE_LENGTH = 16;

private:
    std::string m_code;

    char m_usage[256];
    std::size_t m_length = 0;

public:
    password_finder(const std::string & p_code) :
        m_code(p_code)
    { }

public:
    std::string extract() {
        std::memset(&m_usage, 0, sizeof(m_usage));

        /* place the first word */
        for (std::size_t i = 0; i < MESSAGE_LENGTH; i++) {
            const char symbol = m_code[i];
            if (m_usage[symbol] == 0) {
                m_length++;
            }

            m_usage[symbol]++;
        }

        if (m_length == MESSAGE_LENGTH) {
            return m_code.substr(0, MESSAGE_LENGTH);
        }

        for (std::size_t i = MESSAGE_LENGTH; i < m_code.size(); i++) {
            const char to_remove = m_code[i - MESSAGE_LENGTH];
            const char to_append = m_code[i];

            m_usage[to_remove]--;

            if (m_usage[to_remove] == 0) {
                m_length--;
            }

            if (m_usage[to_append] == 0) {
                m_length++;
            }

            m_usage[to_append]++;

            if (m_length == MESSAGE_LENGTH) {
                return m_code.substr(i + 1 - MESSAGE_LENGTH, MESSAGE_LENGTH);
            }
        }

        throw std::exception("Password is not found.");
    }
};


class base64 {
private:
    const static std::string BASE;

private:
    std::string m_content;

public:
    base64(const std::string & p_content) : m_content(p_content) { }

public:
    std::string decode() {
        std::string result;

        std::vector<int> T(256, -1);
        for (int i = 0; i < 64; i++) T["ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[i]] = i;

        int val = 0, valb = -8;
        for (unsigned char c : m_content) {
            if (T[c] == -1) {
                break;
            }

            val = (val << 6) + T[c];
            valb += 6;

            if (valb >= 0) {
                result.push_back(char((val >> valb) & 0xFF));
                valb -= 8;
            }
        }

        return result;
    }
};


const std::string base64::BASE = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";


int main() {
    const std::string code = read("input.txt");

    const std::string password_base64 = password_finder(code).extract();

    std::cout << "Password (base64): " << password_base64 << std::endl;

    const std::string password = base64(password_base64).decode();

    std::cout << "Password: " << password << std::endl;

    return 0;
}
