#include <algorithm>
#include <iomanip>
#include <iostream>
#include <vector>


struct position {
    long double x = 0;
    long double y = 0;
};


struct shield {
    long double y1 = 0;
    long double y2 = 0;
    long double factor = 0.0;
};


position simulate(const position& from, const long double y_position, const long double x_speed) {
    const long double t = (y_position - from.y);             /* y speed is always constant and equal to 1 */
    const long double x_position = from.x + t * x_speed;

    return { x_position, y_position };
}


position simulate(const std::vector<shield>& shields, const position& firefly, const long double x_speed) {
    position cur = { 0, 0 };
    for (const auto& s : shields) {
        if (cur.y < s.y1) {
            cur = simulate(cur, s.y1, x_speed);
        }

        cur = simulate(cur, s.y2, x_speed * s.factor);
    }
    
    if (cur.y < firefly.y) {
        cur = simulate(cur, firefly.y, x_speed);
    }

    return cur;
}


int main() {
    position goal;
    std::cin >> goal.x >> goal.y;

    position cur = { 0, 0 };
    bool invert_speed = false;
    if (goal.x < 0) {
        goal.x = std::abs(goal.x);
        invert_speed = true;
    }

    int n = 0;
    std::cin >> n;

    std::vector<shield> shields(n);
    for (int i = 0; i < n; i++) {
        std::cin >> shields[i].y1 >> shields[i].y2 >> shields[i].factor;
    }

    std::sort(shields.begin(), shields.end(), [](const shield& l, const shield& r) {
        return l.y1 < r.y1;
    });

    long double x_speed_l = -1;
    long double x_speed_r = -1;
    long double x_speed_m = 0;

    long double delta = goal.x;
    while (delta > 1e-6) {
        if (x_speed_l > 0 && x_speed_r > 0) {
            x_speed_m = (x_speed_l + x_speed_r) / 2.0;
        }
        else if (x_speed_l > 0) {
            x_speed_m = x_speed_l * 2;  /* too slow, and do not know the upper speed limit */
        }
        else if (x_speed_r > 0) {
            x_speed_m = x_speed_r / 2;  /* too fast, and do not know the lower speed limit */
        }
        else {
            x_speed_m = 1.0;
        }

        position final_position = simulate(shields, goal, x_speed_m);

        if (final_position.x > goal.x) {
            x_speed_r = x_speed_m;  /* we are too fast - update upper limit */
        }
        else if (final_position.x < goal.x) {
            x_speed_l = x_speed_m;  /* we are too slow - update lower limit */
        }
        else {
            break;
        }

        delta = std::abs(final_position.x - goal.x);
    }

    if (invert_speed) {
        x_speed_m = -x_speed_m;
    }

    std::cout << std::fixed << std::setprecision(15) << x_speed_m << std::endl;

    return 0;
}