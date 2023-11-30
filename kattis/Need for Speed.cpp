#include <iostream>
#include <iomanip>
#include <vector>


struct segment {
public:
    double s = 0;
    double v = 0;
};


const double INF_TIME = 1e09;


double simulate(const std::vector<segment>& p_segments, const double vc) {
    double t = 0;
    for (const auto& sgm : p_segments) {
        const double v = sgm.v + vc;
        if (v < 1e-08) {
            return INF_TIME;       /* speed is negative or zero - we are not moving anywhere on this segment, we need to be faster */
        }

        t += sgm.s / v;
    }

    return t;
}


int main() {
    int n;
    double t;

    std::cin >> n >> t;

    double cl = 0.0, cr = 0.0;
    bool cl_found = false, cr_found = false;

    std::vector<segment> segments(n);
    for (int i = 0; i < n; i++) {
        std::cin >> segments[i].s >> segments[i].v;
    }

    double delta = 1.0;
    double vc = 0;

    while (delta > 1e-06) {
        if (cl_found && cr_found) {
            vc = (cl + cr) / 2;
        }
        else if (cr_found) {    /* decrease speed to find left border */
            if (cr >= 1.0) {
                vc = cr / 2.0;
            }
            else if (cr <= -1.0) {
                vc = cr * 2.0;
            }
            else {
                vc = -2.0;
            }
        }
        else if (cl_found) {    /* increase speed to found right border */
            if (cl >= 1.0) {
                vc = cl * 2.0;
            }
            else if (cl <= -1.0) {
                vc = cl / 2.0;
            }
            else {
                vc = 2.0;
            }
        }

        double t_actual = simulate(segments, vc);
        delta = std::abs(t_actual - t);     /* be careful, it might not be easy reachable */
        if (cr_found && cl_found) {
            delta = std::abs(cr - cl);      /* rely on delta between left and right borders */
        }

        if (t_actual < t) {
            cr = vc;    /* too fast, reduce velocity */
            cr_found = true;
        }
        else if (t_actual > t) {
            cl = vc;    /* too slow, increase velocity */
            cl_found = true;
        }
        else {
            break;
        }
    }

    std::cout << std::setprecision(15) << vc << std::endl;
    return 0;
}
