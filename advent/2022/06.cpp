#include <iostream>
#include <fstream>
#include <list>
#include <map>
#include <string>
#include <vector>


std::string read_device_stream() {
    std::ifstream stream("input.txt");

    std::string result;
    std::getline(stream, result);

    return result;
}


class device_debugger {
private:
    std::string m_data;

public:
    device_debugger(const std::string& p_data) :
        m_data(p_data)
    { }

public:
    int find_packet_marker(const std::size_t p_marker_size) {
        std::map<char, int> detector;
        std::list<char> preambula;

        for (int i = 0; i < p_marker_size; i++) {
            detector[m_data[i]]++;
            preambula.push_back(m_data[i]);
        }

        for (int i = p_marker_size; i < m_data.size(); i++) {
            if (detector.size() == p_marker_size) {
                return i;
            }

            const char value = m_data[i];

            const char to_remove = preambula.front();
            preambula.pop_front();

            auto iter = detector.find(to_remove);
            iter->second--;
            if (iter->second == 0) {
                detector.erase(iter);
            }

            preambula.push_back(value);
            detector[value]++;
        }

        return m_data.size();
    }
};


int main() {
    auto signal = read_device_stream();

    std::cout << "Position when the first start-of-packet marker is detected: " << device_debugger(signal).find_packet_marker(4) << std::endl;
    std::cout << "Position when the first start-of-message marker is detected: " << device_debugger(signal).find_packet_marker(14) << std::endl;

    return 0;
}
