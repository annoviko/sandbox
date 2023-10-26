#include <iostream>
#include <fstream>
#include <regex>
#include <set>


struct area {
public:
    int x_begin = 0;
    int x_end = 0;
    int y_begin = 0;
    int y_end = 0;

public:
    bool is_in(const int x, const int y) const {
        return is_in_x(x) && is_in_y(y);
    }

    bool is_in_x(const int x) const {
        return (x >= x_begin) && (x <= x_end);
    }

    bool is_in_y(const int y) const {
        return (y <= y_begin) && (y >= y_end);
    }
};


struct velocity {
    int x = 0;
    int y = 0;
};

using position = velocity;


class optimizer {
private:
    area m_area;

public:
    optimizer(const area& p_area) : m_area(p_area) { }

public:
    int evaluate_optimal_height() {
        /*

        We can ignore X velocity, since when it is 0, it keeps 0, so, we can throw it as we want, and we can focus on
        Y velocity only.

        Using arithmetical progression.
        1. Lets consider the first case for our understanding: we throw the probe from initial point and want to know the height when Y velocity is 0.
           So in this case we have progression Sn = vy + (vy - 1) + (vy - 2) + ... + 1. The last value vy = 1 when we reach the highest point.
           Lets rewrite it using the arithmetical progression: Sn = n * (vy + 1) / 2, where n is equal to vy (vy is changing by 1, it is just a counter), so:
           Sn = vy * (vy + 1) / 2. Now the question what is optimal vy?

        2. We considered the throw from the initial point. But lets consider it from the area that we want to reach. We want to be exactly in
           the target area: (y_max, y_min). It means, that we can use the minimum y as a velocity (otherwise we will miss the area) and as
           before we want to know when the velocity is going to be 0: Sn = vy * (vy + 1) / 2, but vy = abs(ymin) - 1. Why vy = abs(ymin) - 1? 
           We always cross 0 twice (parabola). It means that the highest speed that we can afford is equal to speed that bring us from 0 to minimum Y,
           that's why speed vy = abs(ymin) - 1.
        
        */

        int vy = std::abs(m_area.y_end) - 1;
        return vy * (vy + 1) / 2;
    }


    std::size_t find_possible_ways() const {
        std::set<std::pair<int, int>> unique_ways = { };

        for (int initial_vy = m_area.y_end; initial_vy <= std::abs(m_area.y_end); initial_vy++) {
            for (int initial_vx = m_area.x_end; initial_vx >= 0; initial_vx--) {
                int y = 0;
                int x = 0;

                int vx = initial_vx;
                int vy = initial_vy;

                while (y >= m_area.y_end && x <= m_area.x_end) {
                    y += vy;
                    x += vx;

                    vx = vx > 0 ? vx - 1 : 0;
                    vy--;

                    if (m_area.is_in(x, y)) {
                        //std::cout << initial_vx << "," << initial_vy << std::endl;
                        unique_ways.insert({initial_vx, initial_vy});
                    }
                }
            }
        }

        return unique_ways.size();
    }
};


area read_read() {
    std::ifstream stream("input.txt");

    std::string computer_input;
    std::getline(stream, computer_input);

    std::regex pattern("target area: x=(\\d+)\\.\\.(\\d+), y=-(\\d+)\\.\\.-(\\d+)");
    std::smatch result;

    if (!std::regex_search(computer_input, result, pattern)) {
        throw std::invalid_argument("incorrect file format");
    }

    return { std::stoi(result[1].str()), std::stoi(result[2].str()), -std::stoi(result[4].str()), -std::stoi(result[3].str()) };
}


int main() {
    area target = read_read();

    std::cout << "The highest y: " << optimizer(target).evaluate_optimal_height() << std::endl;
    std::cout << "The number of initial velocities: " << optimizer(target).find_possible_ways() << std::endl;

    return 0;
}
