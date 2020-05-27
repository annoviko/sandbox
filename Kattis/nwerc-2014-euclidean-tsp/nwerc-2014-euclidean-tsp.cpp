#include <iostream>
#include <cmath>
#include <iomanip>


double get_running_time(double n, double p, double c) {
    return n * std::pow(std::log2(n), c * std::sqrt(2)) / (p * std::pow(10.0, 9));
}


double get_tour_duration(double s, double v, double c) {
    return s * (1.0 + 1.0 / c) / v;
}


double get_total_time(double n, double p, double s, double v, double c) {
    return get_running_time(n, p, c) + get_tour_duration(s, v, c);
}


int main() {
    double n, p, s, v;
    std::cin >> n >> p >> s >> v;

    auto func = [n, p, s, v](double c) { return get_total_time(n, p, s, v, c); };

    double c_begin = 1;
    double c_end = 100;

    double t_change = std::numeric_limits<double>::max();
    double c_change = std::numeric_limits<double>::max();

    while (t_change > 0.0000001 || c_change > 0.0000001) {
        double c_13 = c_begin + (c_end - c_begin) / 3;
        double c_23 = c_end - (c_end - c_begin) / 3;

        double t_13 = func(c_13);
        double t_23 = func(c_23);

        if (t_13 > t_23) {
            c_begin = c_13;
        }
        else if (t_13 < t_23) {
            c_end = c_23;
        }
        else {
            c_begin = c_13;
            c_end = c_23;
        }

        t_change = std::abs(t_23 - t_13);
        c_change = std::abs(c_23 - c_13);
    }

    std::cout << std::setprecision(12) << func(c_begin) << " " << c_begin << std::endl;

    return 0;
}