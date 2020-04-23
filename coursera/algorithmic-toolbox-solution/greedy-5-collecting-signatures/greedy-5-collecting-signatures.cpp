#include <algorithm>
#include <iostream>
#include <climits>
#include <vector>

using std::vector;

struct Segment {
    int start, end;
};

/*

1 ****
2  ****
3  *
4    ****
   ^
   |
0 123456789


*/


vector<int> optimal_points(vector<Segment> &segments) {
    std::sort(segments.begin(), segments.end(), [](const Segment & p1, const Segment & p2) {
        return p1.start < p2.start;
    });

    vector<int> points;

    for (std::size_t i = 0; i < segments.size(); i++) {
        int left = segments[i].start;
        int right = segments[i].end;

        for (std::size_t j = i + 1; j < segments.size(); j++) {
            int candidate_left = segments[j].start, candidate_right = segments[j].end;
            if (candidate_left > right) { // no intersections anymore
                break;
            }
            else {
                // assign new right border
                right = std::min(right, candidate_right);
                i++;
            }
        }

        // right border as time to visit
        points.push_back(right);
    }

    return points;
}

int main() {
    int n;
    std::cin >> n;
    vector<Segment> segments(n);
    for (size_t i = 0; i < segments.size(); ++i) {
        std::cin >> segments[i].start >> segments[i].end;
    }
    vector<int> points = optimal_points(segments);
    std::cout << points.size() << "\n";
    for (size_t i = 0; i < points.size(); ++i) {
        std::cout << points[i] << " ";
    }
}
