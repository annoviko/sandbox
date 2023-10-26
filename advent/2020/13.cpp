#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>
#include <numeric>
#include <string>
#include <vector>



using bus_ids = std::vector<std::size_t>;
using bus_idxs = std::vector<std::size_t>;


struct task {
    std::size_t m_time;
    bus_ids     m_buses;
    bus_idxs    m_indexes;
};


task read_input(const std::string & p_file) {
    std::ifstream stream(p_file);

    task result;

    std::string line;
    std::getline(stream, line);
    result.m_time = std::stoull(line);

    std::getline(stream, line);
    std::size_t rborder = 0;
    std::size_t lborder = 0;
    std::size_t index = 0;

    while (lborder != std::string::npos) {
        lborder = line.find(',', rborder);
        std::string value = line.substr(rborder, lborder - rborder);
        if (value != "x") {
            result.m_buses.push_back(std::stoull(value));
            result.m_indexes.push_back(index);
        }

        index++;
        rborder = lborder + 1;
    }

    return result;
}


std::size_t find_earliest_bus(const task & p_task) {
    std::size_t waiting_time = std::numeric_limits<std::size_t>::max();
    std::size_t bus_id = 0;

    for (auto id : p_task.m_buses) {
        std::size_t departure = (std::size_t) std::ceil((double) p_task.m_time / (double) id) * id;
        std::size_t current_waiting_time = departure - p_task.m_time;
        if (waiting_time > current_waiting_time) {
            waiting_time = current_waiting_time;
            bus_id = id;
        }
    }

    return bus_id * waiting_time;
}


/*

Modulo inverse algorithm: https://www.geeksforgeeks.org/multiplicative-inverse-under-modulo-m/

*/
std::size_t mod_inverse(const std::size_t a, const std::size_t m)
{
    std::size_t m0 = m;
    std::size_t a0 = a;
    std::size_t y = 0;
    long long x = 1;

    if (m == 1) {
        return 0;
    }

    while (a0 > 1) {
        std::size_t q = a0 / m0;
        std::size_t t = m0;

        m0 = a0 % m0;
        a0 = t;
        t = y;

        y = x - q * y;
        x = t;
    }

    if (x < 0) {
        x += m;
    }

    return x;
}


/* 

The solution is represented by CRT: https://crypto.stanford.edu/pbc/notes/numbertheory/crt.html 

*/
std::size_t find_earliest_timestamp(const task & p_task) {
    std::size_t M = 1;
    for (auto id : p_task.m_buses) {
        M *= id;
    }

    std::size_t result = 0;
    for (std::size_t i = 0; i < p_task.m_buses.size(); i++) {
        const std::size_t m = p_task.m_buses[i];
        const std::size_t a = (m - p_task.m_indexes[i] % m) % m;
        const std::size_t bi = M / m;
        const std::size_t bi_inv = mod_inverse(bi, m);
        result += a * bi * bi_inv;
    }

    return result % M;
}


int main() {
    auto current_task = read_input("input.txt");

    std::cout << "The ID of the earliest bus multiplied by the number of minutes you'll need to wait: " << find_earliest_bus(current_task) << std::endl;
    std::cout << "The earliest timestamp: " << find_earliest_timestamp(current_task) << std::endl;

    return 0;
}
