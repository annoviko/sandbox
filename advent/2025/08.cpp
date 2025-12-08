#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_set>


class disjoint_set {
private:
    std::vector<int> ids;
    std::vector<int> sz;
    int n;

public:
    disjoint_set(int len) :
        ids(len, 0),
        sz(len, 1),
        n(len)
    {
        for (int i = 0; i < len; i++) {
            ids[i] = i;
        }
    }

public:
    int get_set_id(int id) {
        if (ids[id] != id) {
            ids[id] = get_set_id(ids[id]);
        }

        return ids[id];
    }

    bool is_same_set(int id1, int id2) {
        return get_set_id(id1) == get_set_id(id2);
    }

    void merge(int id1, int id2) {
        int set_id1 = get_set_id(id1);
        int set_id2 = get_set_id(id2);

        if (set_id1 == set_id2) {
            return;
        }

        if (sz[set_id2] > sz[set_id1]) {
            std::swap(set_id1, set_id2);
        }

        ids[set_id2] = set_id1;
        sz[set_id1] += sz[set_id2];
        n--;
    }

    int size() {
        return n;
    }

    std::uint64_t three_largest_circuits() {
        std::unordered_set<int> visited;
        std::vector<int> sizes;

        for (int i = 0; i < ids.size(); i++) {
            int set_id = get_set_id(i);
            auto iter = visited.find(set_id);
            if (iter != visited.end()) {
                continue;
            }

            visited.insert(set_id);
            sizes.push_back(sz[set_id]);
        }

        std::sort(sizes.begin(), sizes.end(), std::greater<int>());
        std::uint64_t result = 1;
        for (int i = 0; i < 3; i++) {
            result *= sizes[i];
        }

        return result;
    }
};


struct position_t {
    int x;
    int y;
    int z;
};


std::ostream& operator<<(std::ostream& stream, const position_t& p) {
    stream << "(" << p.x << ", " << p.y << ", " << p.z << ")";
    return stream;
}


struct edge_t {
    long double dist;
    int from;
    int to;
};


class solution {
private:
    std::vector<position_t> pos;

public:
    solution(const std::vector<position_t>& p_pos) : pos(p_pos) { }

public:
    std::uint64_t connect(int pairs) {
        auto edges = eval_sorted_edges();
        disjoint_set set(pos.size());

        int counter = 0;
        for (int i = 0; i < edges.size() && counter < pairs; i++) {
            const int from = edges[i].from;
            const int to = edges[i].to;

            set.merge(from, to);
            counter++;
        }

        return set.three_largest_circuits();
    }

    std::uint64_t connect_all() {
        auto edges = eval_sorted_edges();
        disjoint_set set(pos.size());

        for (int i = 0; i < edges.size(); i++) {
            const int from = edges[i].from;
            const int to = edges[i].to;

            set.merge(from, to);
            if (set.size() == 1) {
                const std::uint64_t x1 = pos[from].x;
                const std::uint64_t x2 = pos[to].x;

                return x1 * x2;
            }
        }

        throw std::exception("input data or the algorithm is incorrect");
    }

private:
    std::vector<edge_t> eval_sorted_edges() {
        std::vector<edge_t> result;
        for (int i = 0; i < pos.size(); i++) {
            for (int j = i + 1; j < pos.size(); j++) {
                const long double dist = euclidean_distance_squared(i, j);
                result.push_back({ dist, i, j });
            }
        }

        std::sort(result.begin(), result.end(), [](const edge_t& l, const edge_t& r) {
            return l.dist < r.dist;
        });

        return result;
    }

    long double euclidean_distance_squared(int i, int j) {
        long double dx = pos[j].x - pos[i].x;
        long double dy = pos[j].y - pos[i].y;
        long double dz = pos[j].z - pos[i].z;

        return dx * dx + dy * dy + dz * dz;
    }
};


std::vector<position_t> read_input() {
    std::fstream stream("input.txt");
    std::vector<position_t> input;

    for (std::string line; std::getline(stream, line); ) {
        std::stringstream ss(line);

        position_t p;
        char i;
        ss >> p.x >> i >> p.y >> i >> p.z;

        input.push_back(p);
    }

    return input;
}


int main() {
    const auto input = read_input();

    std::cout << "Value of the three largest circuits: " << solution(input).connect(1000) << std::endl;
    std::cout << "Value of the last two connected boxes: " << solution(input).connect_all() << std::endl;

    return 0;
}