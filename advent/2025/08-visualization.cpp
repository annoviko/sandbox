#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_set>

#include "raylib.h"


static const Color PALETTE[30] = {
    RED, MAROON, ORANGE, GOLD, YELLOW,
    GREEN, LIME, DARKGREEN, SKYBLUE, BLUE,
    DARKBLUE, PURPLE, VIOLET, MAGENTA, PINK,
    BROWN, BEIGE, DARKBROWN, GRAY, DARKGRAY,
    RAYWHITE, BLACK,
    {255,128,0,255}, {128,255,0,255}, {0,255,128,255},
    {0,128,255,255}, {128,0,255,255}, {255,0,128,255},
    {255,64,64,255}, {64,255,64,255}
};


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

    int get_set_size(int id) {
        return sz[get_set_id(id)];
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
    double x;
    double y;
    double z;
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
    void visualize_algorithm() {
        normalize();

        auto edges = eval_sorted_edges();
        disjoint_set set(pos.size());

        InitWindow(1920, 1080, "3D Points Viewer");
        SetTargetFPS(60);

        Camera3D camera = { 0 };
        camera.position = { 1.0f, 1.0f, 1.0f };
        camera.target = { 0.0f, 0.0f, 0.0f };
        camera.up = { 0.0f, 1.0f, 0.0f };
        camera.fovy = 45.0f;
        camera.projection = CAMERA_PERSPECTIVE;

        for (int i = 0; i < pos.size(); i++) {
            Vector3 v = { (float)pos[i].x, (float)pos[i].y, (float)pos[i].z };
            DrawSphere(v, 0.005f, GRAY);
        }

        int iteration = 0;
        bool done = false;

        std::vector<std::vector<int>> neis(pos.size());

        double lastUpdate = GetTime();

        while (!WindowShouldClose()) {
            UpdateCamera(&camera, CAMERA_ORBITAL);

            BeginDrawing();
            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

            double now = GetTime();
            if (now - lastUpdate > 0.0001) {
                if (iteration < edges.size() && !done) {
                    const int from = edges[iteration].from;
                    const int to = edges[iteration].to;

                    neis[to].push_back(from);

                    set.merge(from, to);
                    if (set.size() == 1) {
                        done = true;
                    }

                    iteration++;
                }

                lastUpdate = now;
            }

            for (int i = 0; i < pos.size(); i++) {
                const int set_id = set.get_set_id(i);

                Vector3 v = { (float)pos[i].x, (float)pos[i].y, (float)pos[i].z };

                const int set_size = set.get_set_size(i);
                Color c = (set_size > 1) ? PALETTE[set_id % 30] : GRAY;
                /* DrawCube(v, 0.005f, 0.005f, 0.005f, c); */
                DrawSphereWires(v, 0.005f, 4, 4, c);

                for (int nei : neis[i]) {
                    Vector3 A = { (float)pos[i].x, (float)pos[i].y, (float)pos[i].z };
                    Vector3 B = { (float)pos[nei].x, (float)pos[nei].y, (float)pos[nei].z };
                    DrawLine3D(A, B, c);
                }
            }

            DrawGrid(10, 1.0f);

            EndMode3D();

            EndDrawing();
        }

        CloseWindow();
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

    void normalize() {
        double xmin = std::numeric_limits<double>::max();
        double ymin = std::numeric_limits<double>::max();
        double zmin = std::numeric_limits<double>::max();

        double xmax = 0, ymax = 0, zmax = 0;

        for (int i = 0; i < pos.size(); i++) {
            xmax = std::max(pos[i].x, xmax);
            ymax = std::max(pos[i].y, ymax);
            zmax = std::max(pos[i].z, zmax);

            xmin = std::min(pos[i].x, xmin);
            ymin = std::min(pos[i].y, ymin);
            zmin = std::min(pos[i].z, zmin);
        }

        double xsize = xmax - xmin;
        double ysize = ymax - ymin;
        double zsize = zmax - zmin;

        double max_size = std::max({ xsize, ysize, zsize });

        double xc = (xmin + xmax) / 2.0;
        double yc = (ymin + ymax) / 2.0;
        double zc = (zmin + zmax) / 2.0;

        for (int i = 0; i < pos.size(); i++) {
            pos[i].x = (pos[i].x - xc) / max_size;
            pos[i].y = (pos[i].y - yc) / max_size;
            pos[i].z = (pos[i].z - zc) / max_size;
        }
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

    solution(input).visualize_algorithm();

    return 0;
}