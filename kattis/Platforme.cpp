#include <algorithm>
#include <limits>
#include <vector>


struct point {
    long double x = 0;
    long double y = 0;
};


struct platform {
private:
    point m_p1;
    point m_p2;

    long double m_llen = m_p1.y;
    long double m_rlen = m_p1.y;

public:
    platform(const point& p_p1, const point& p_p2) :
        m_p1(p_p1), m_p2(p_p2)
    { }

public:
    void assign_pillars(const platform& p_other) {
        if (!is_platform_below(p_other)) {
            return;
        }

        m_llen = std::min(m_llen, get_pillar_length(m_p1.x + 0.5, p_other));
        m_rlen = std::min(m_rlen, get_pillar_length(m_p2.x - 0.5, p_other));
    }

    long double get_pillars_length() const { return m_llen + m_rlen; }

private:
    bool is_platform_below(const platform& p_other) const {
        return m_p1.y > p_other.m_p1.y;
    }

    bool is_pillar_touch_platform(const long double p_x, const platform& p_other) const {
        if (p_other.m_p1.x <= p_x && p_x <= p_other.m_p2.x) {
            return true;
        }

        return false;
    }

    long double get_distance_to_platform(const platform& p_other) const {
        return m_p1.y - p_other.m_p1.y;
    }

    long double get_pillar_length(const long double x_pillar, const platform& p_other) {
        if (is_pillar_touch_platform(x_pillar, p_other)) {
            return get_distance_to_platform(p_other);
        }

        return std::numeric_limits<long double>::max();
    }
};


#include <iostream>


int main() {
    std::size_t n;
    std::cin >> n;

    std::vector<platform> platforms;
    for (std::size_t i = 0; i < n; i++) {
        long double y, x1, x2;
        std::cin >> y >> x1 >> x2;

        platforms.emplace_back(point{ x1, y }, point{ x2, y });
    }

    for (std::size_t i = 0; i < n; i++) {
        for (std::size_t j = 0; j < n; j++) {
            if (i == j) {
                continue;
            }

            platforms[i].assign_pillars(platforms[j]);
        }
    }

    long int total_length = 0;
    for (const auto& p : platforms) {
        total_length += (long int)p.get_pillars_length();
    }

    std::cout << total_length << std::endl;
    return 0;
}
