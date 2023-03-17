#include <iostream>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <exception>


struct rational_number {
public:
    std::int64_t p = 0;
    std::int64_t q = 1;

public:
    rational_number() {
        p = 0;
        q = 1;
    }

    rational_number(const std::int64_t p_val) {
        p = p_val;
        q = 1;
    }

    rational_number(const std::int64_t p_p, const std::int64_t p_q) {
        p = p_p;
        q = p_q;
    }

public:
    friend rational_number operator+(const rational_number& l, const rational_number& r) {
        rational_number number;
        number.p = (l.p * r.q) + (r.p * l.q);
        number.q = l.q * r.q;
        return number.reduce();
    }

    friend rational_number operator-(const rational_number& l, const rational_number& r) {
        rational_number number;
        number.p = (l.p * r.q) - (r.p * l.q);
        number.q = l.q * r.q;
        return number.reduce();
    }

    friend rational_number operator*(const rational_number& l, const rational_number& r) {
        rational_number number;
        number.p = l.p * r.p;
        number.q = l.q * r.q;
        return number.reduce();
    }

    friend rational_number operator/(const rational_number& l, const rational_number& r) {
        rational_number number;
        number.p = l.p * r.q;
        number.q = l.q * r.p;
        return number.reduce();
    }

    bool operator>(const int64_t& v) const {
        return *this > rational_number(v);
    }

    bool operator>(const rational_number& p_other) const {
        std::int64_t p1 = p * p_other.q;
        std::int64_t p2 = p_other.p * q;
        return p1 > p2;
    }

    bool operator>=(const rational_number& p_other) const {
        std::int64_t p1 = p * p_other.q;
        std::int64_t p2 = p_other.p * q;
        return p1 >= p2;
    }

    bool operator<(const rational_number& p_other) const {
        std::int64_t p1 = p * p_other.q;
        std::int64_t p2 = p_other.p * q;
        return p1 < p2;
    }

    bool operator<=(const rational_number& p_other) const {
        std::int64_t p1 = p * p_other.q;
        std::int64_t p2 = p_other.p * q;
        return p1 <= p2;
    }

    bool operator==(const double p_other) const {
        std::int64_t p1 = p;
        std::int64_t p2 = p_other * q;
        return p1 == p2;
    }

    bool operator==(const rational_number& p_other) const {
        return p == p_other.p && q == p_other.q;
    }

    bool operator!=(const rational_number& p_other) const {
        return !(*this == p_other);
    }

    rational_number operator-() {
        p = -p;
        return *this;
    }

    rational_number& reduce() {
        std::int64_t greatest_common_devider = std::gcd(p, q);

        p /= greatest_common_devider;
        q /= greatest_common_devider;

        if (p < 0 && q < 0) {
            p = -p;
            q = -q;
        }
        else if (q < 0) {
            p = -p;
            q = -q;
        }

        return *this;
    }
};


std::ostream& operator<<(std::ostream& p_stream, const rational_number& p_value) {
    p_stream << p_value.p << " " << p_value.q;
    return p_stream;
}


struct point {
    rational_number x;
    rational_number y;
};


struct segment {
    point p1;
    point p2;
};


rational_number get_max(const rational_number& l, const rational_number& r) {
    if (l < r) {
        return r;
    }
    return l;
}


rational_number get_min(const rational_number& l, const rational_number& r) {
    if (l > r) {
        return r;
    }
    return l;
}


bool is_inside_segment(const segment& sgm, const point& p) {
    if (get_min(sgm.p1.x, sgm.p2.x) <= p.x && p.x <= get_max(sgm.p1.x, sgm.p2.x)) {
        if (get_min(sgm.p1.y, sgm.p2.y) <= p.y && p.y <= get_max(sgm.p1.y, sgm.p2.y)) {
            return true;
        }
    }

    return false;
}


bool get_intersection(const segment& l1, const segment& l2, point& p_intersec) {
    using rn = rational_number;

    rn a1 = l1.p2.y - l1.p1.y;
    rn b1 = l1.p1.x - l1.p2.x;
    rn c1 = a1 * l1.p1.x + b1 * l1.p1.y;

    rn a2 = l2.p2.y - l2.p1.y;
    rn b2 = l2.p1.x - l2.p2.x;
    rn c2 = a2 * l2.p1.x + b2 * l2.p1.y;

    rn determinant = a1 * b2 - a2 * b1;

    if (determinant == 0) {
        return false;
    }

    p_intersec.x = (b2 * c1 - b1 * c2) / determinant;
    p_intersec.y = (a1 * c2 - a2 * c1) / determinant;

    return true;
}


#include <vector>

class field {
private:
    std::vector<segment> m_square = { segment{ { -1, -1 }, { -1, 1 } }, segment{ { -1, 1 }, { 1, 1 } }, segment{ { 1, -1 }, { 1, 1 } }, segment{ { -1, -1 }, { 1, -1 } } };
    int m_last_segment_index = 0;

    point cur_position = { -1, 0 };

private:
    int get_next_segment_index(int index) {
        return (index + 1) % m_square.size();
    }

public:

    point find_final_position(const int a, const int b, const int n) {
        point delta = { b, a };
        point destination = { cur_position.x + delta.x, cur_position.y + delta.y };

        for (int i = 0; i <= n; i++) {
            segment trajectory = { cur_position, destination };

            point intersection;
            bool intersection_found = false;
            for (int i = 0; i < m_square.size(); i++) {
                if (i == m_last_segment_index) {
                    continue;
                }

                if (get_intersection(trajectory, m_square[i], intersection) && is_inside_segment(m_square[i], intersection)) {
                    cur_position = intersection;
                    intersection_found = true;
                    m_last_segment_index = i;
                    break;
                }
            }
            
            if (!intersection_found) {
                std::cout << "impossible to hit any of the walls" << std::endl;
                throw std::exception();
            }


            if (cur_position.x == 1 || cur_position.x == -1) {
                delta.x = -delta.x;
            }
            else if (cur_position.y == 1 || cur_position.y == -1) {
                delta.y = -delta.y;
            }

            destination = { cur_position.x + delta.x, cur_position.y + delta.y };
        }

        return cur_position;
    }
};


int main() {
    int a, b, n;
    std::cin >> a >> b >> n;

    point result = field().find_final_position(a, b, n);
    std::cout << result.x << " " << result.y << std::endl;

    return 0;
}