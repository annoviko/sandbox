#include <cmath>
#include <vector>
#include <queue>
#include <limits>


struct position {
    int x = 0;
    int y = 0;

    position() : 
        x(0), y(0)
    { }

    position(int p_x, int p_y) :
        x(p_x), y(p_y)
    { }

    double distance_to(const position& p_other) const {
        return std::sqrt(std::pow(x - p_other.x, 2) + std::pow(y - p_other.y, 2));
    }
};


struct edge {
    int id = -1;
    double weight = -1;

    edge(int p_id, double p_w) :
        id(p_id), weight(p_w)
    { }

    bool operator<(const edge& p_other) const {
        return weight < p_other.weight;
    }

    bool operator>(const edge& p_other) const {
        return weight > p_other.weight;
    }
};


using subway_line = std::vector<position>;
using subway = std::vector<subway_line>;


class solution {
private:
    std::vector<position> m_nodes;
    std::vector<std::vector<double>> m_mtrx;

    position m_source;
    position m_destination;

public:
    solution(const subway& p_s, const position& source, const position& destination) :
        m_source(source), m_destination(destination)
    {
        int node_amount = 0;
        for (auto line : p_s) {
            node_amount += line.size();
        }

        node_amount += 2; // source and destination

        m_nodes = std::vector<position>();
        m_mtrx = std::vector<std::vector<double>>(node_amount, std::vector<double>(node_amount, -1.0));

        m_nodes.push_back(source);

        // fill stations
        int node_id = 1;
        for (int i = 0; i < p_s.size(); i++) {
            int j = 1;

            for (; j < p_s[i].size(); j++) {
                int node_id_from = node_id;
                int node_id_to = node_id + 1;

                double distance = p_s[i][j - 1].distance_to(p_s[i][j]) / 1000.0;
                double time = distance / 40.0;

                m_mtrx[node_id_from][node_id_to] = time;
                m_mtrx[node_id_to][node_id_from] = time;

                node_id++;
                m_nodes.push_back(p_s[i][j - 1]);
            }

            m_nodes.push_back(p_s[i][j - 1]);
            node_id++;
        }

        m_nodes.push_back(destination);

        for (int i = 0; i < m_mtrx.size(); i++) {
            for (int j = i; j < m_mtrx.size(); j++) {
                if (i == j) {
                    m_mtrx[i][j] = 0.0;
                    continue;
                }

                if (m_mtrx[i][j] >= 0.0) {
                    continue;
                }

                double distance = m_nodes[i].distance_to(m_nodes[j]) / 1000.0;
                double time = distance / 10.0;

                m_mtrx[i][j] = time;
                m_mtrx[j][i] = time;
            }
        }
    }

public:
    int find_fastest_path() {
        std::priority_queue<edge, std::vector<edge>, std::greater<edge>> queue;
        queue.push(edge(0, 0));

        std::vector<double> cost(m_nodes.size(), std::numeric_limits<double>::max());
        cost[0] = 0;

        while (!queue.empty()) {
            edge cur = queue.top();
            queue.pop();

            if (cur.weight > cost[cur.id]) {
                continue;
            }

            for (int to = 0; to < m_nodes.size(); to++) {
                if (to == cur.id) {
                    continue;
                }

                double weight = cost[cur.id] + m_mtrx[cur.id][to];
                if (weight < cost[to]) {
                    cost[to] = weight;
                    queue.push(edge(to, weight));
                }
            }
        }

        double hours = cost.back();
        double minutes = hours * 60;
        return std::round(minutes);
    }
};


#include <iostream>
#include <string>
#include <sstream>


int main() {
    std::string line;
    std::getline(std::cin, line);

    int n = std::stoi(line);

    std::getline(std::cin, line);

    for (int i = 0; i < n; i++) {
        std::getline(std::cin, line);

        position source, destination;

        {
            std::stringstream stream(line);
            stream >> source.x >> source.y >> destination.x >> destination.y;
        }

        subway metro;

        for (std::getline(std::cin, line); !line.empty(); std::getline(std::cin, line))
        {
            std::stringstream stream(line);

            subway_line metro_line;
            position station;
            do {
                stream >> station.x >> station.y;
                if (station.x != -1 && station.y != -1) {
                    metro_line.push_back(station);
                }
            } while (station.x != -1 && station.y != -1);

            metro.push_back(metro_line);
        }

        std::cout << solution(metro, source, destination).find_fastest_path() << std::endl;
        if (i != n - 1) {
            std::cout << std::endl;
        }
    }

    return 0;
}