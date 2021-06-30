#include <iostream>
#include <vector>
#include <algorithm>

using std::cin;
using std::cout;
using std::vector;
using std::max;

const static int NO_STATION = -1;

int compute_min_refills(int dist, int tank, vector<int> & stops) {
    int amount = 0;

    int current_position = 0;
    int current_station = NO_STATION;

    while (current_position + tank < dist) {
        int next_station = (current_station == NO_STATION) ? 0 : current_station + 1;
        bool is_reachable_station = false;

        for (; next_station < stops.size(); next_station++) {
            if (stops[next_station] - current_position <= tank) {
                current_station = next_station;
                is_reachable_station = true;
            }
            else {
                break;
            }
        }

        if (is_reachable_station) {
            current_position = stops[current_station];
            amount++;
        }
        else {
            return -1;
        }
    }

    if (current_position + tank >= dist) {
        return amount;
    }

    return -1;
}


int main() {
    int d = 0;
    cin >> d;
    int m = 0;
    cin >> m;
    int n = 0;
    cin >> n;

    vector<int> stops(n);
    for (size_t i = 0; i < n; ++i)
        cin >> stops.at(i);

    cout << compute_min_refills(d, m, stops) << "\n";

    return 0;
}
