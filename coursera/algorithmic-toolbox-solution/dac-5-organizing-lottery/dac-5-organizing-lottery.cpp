#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>

using std::vector;

struct point_desc {
public:
    char m_type;
    int m_value;
    int m_index;
};

vector<int> fast_count_segments(vector<int> starts, vector<int> ends, vector<int> points) {
    vector<int> cnt(points.size());

    vector<point_desc> elements;
    elements.reserve(starts.size() * 2 + points.size());

    for (auto val : starts) { elements.push_back({ 'l', val, -1 }); }
    for (auto val : ends) { elements.push_back({ 'r', val, -1 }); }
    for (int i = 0; i < points.size(); i++) { elements.push_back({ 'p', points[i], i }); }

    std::sort(elements.begin(), elements.end(), [](point_desc & p1, point_desc & p2) {
        if (p1.m_value == p2.m_value) {
            // left border must be always at left side
            if (p1.m_type == 'l' && p2.m_type != 'l') {
                return true;
            }
            else if (p1.m_type != 'l' && p2.m_type == 'l') {
                return false;
            }

            // right border must be always at right side
            else if (p1.m_type == 'r' && p2.m_type != 'r') {
                return false;
            }
            else if (p1.m_type != 'r' && p2.m_type == 'r') {
                return true;
            }
        }

        return p1.m_value < p2.m_value;
    });

    int overlap = 0;
    for (const auto & item : elements) {
        if (item.m_type == 'l') {
            overlap++;
        }
        else if (item.m_type == 'r') {
            overlap--;
        }
        else {
            cnt[item.m_index] = overlap;
        }
    }

    return cnt;
}

vector<int> naive_count_segments(vector<int> starts, vector<int> ends, vector<int> points) {
    vector<int> cnt(points.size());
    for (size_t i = 0; i < points.size(); i++) {
        for (size_t j = 0; j < starts.size(); j++) {
            cnt[i] += starts[j] <= points[i] && points[i] <= ends[j];
        }
    }
    return cnt;
}

int main() {
    int n, m;
    std::cin >> n >> m;
    vector<int> starts(n), ends(n);
    for (size_t i = 0; i < starts.size(); i++) {
        std::cin >> starts[i] >> ends[i];
    }
    vector<int> points(m);
    for (size_t i = 0; i < points.size(); i++) {
        std::cin >> points[i];
    }
    //use fast_count_segments
    //vector<int> cnt = naive_count_segments(starts, ends, points);
    //for (size_t i = 0; i < cnt.size(); i++) {
    //    std::cout << cnt[i] << ' ';
    //}

    std::cout << std::endl;
    vector<int> cnt = fast_count_segments(starts, ends, points);
    for (size_t i = 0; i < cnt.size(); i++) {
        std::cout << cnt[i] << ' ';
    }
}
