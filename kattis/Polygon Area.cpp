#include <iostream>
#include <iomanip>
#include <vector>
#include <string>


struct point {
    long double x = 0;
    long double y = 0;
};


double long trapezoid(const std::vector<point>& p) {
    long double area = 0;
    for (int i = 1; i < p.size(); i++) {
        area += (p[i - 1].y + p[i].y) * (p[i - 1].x - p[i].x);
    }

    if (p.size() > 0) {
        area += (p.back().y + p[0].y) * (p.back().x - p[0].x);
    }

    return area / 2.0;
}


int main() {
    int n = 0;
    std::cin >> n;

    while (n != 0) {
        std::vector<point> points(n);
        for (int i = 0; i < n; i++) {
            std::cin >> points[i].x >> points[i].y;
        }

        long double area = trapezoid(points);

        std::string result = std::to_string(std::abs(area));
        int decimal_point = result.find('.');
        if (decimal_point == std::string::npos) {
            result = result + ".0";
        }
        else {
            result = result.substr(0, decimal_point + 2);
        }

        std::cout << ((area > 0) ? "CCW " : "CW ") << result << std::endl;
        std::cin >> n;
    }
}