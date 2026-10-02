#include <iostream>
#include <fstream>
#include <queue>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <climits>
#include <cmath>

#include "raylib.h"
#include "rlgl.h"


struct position_t {
    int x = -1;
    int y = -1;
    int z = -1;
};


using space_t = std::unordered_map<int, std::unordered_map<int, std::unordered_set<int>>>;    /* x, y, z */


std::pair<space_t, int> read_input() {
    std::ifstream stream("input.txt");

    space_t space;
    int blocks = 0;

    for (std::string line; std::getline(stream, line);) {
        std::stringstream ss(line);
        
        int x, y, z;
        char ignore;
        ss >> x >> ignore >> y >> ignore >> z;

        space[x][y].insert(z);
        blocks++;
    }

    return { space, blocks };
}


class solution {
private:
    space_t sp;
    int blocks = 0;

public:
    solution(const space_t& s, int n) : sp(s), blocks(n) {}

public:
    int calculate_surface_area() {
        const std::vector<position_t> dirs = {
            { 0, 0, 1 },
            { 0, 0, -1 },
            { 0, 1, 0 },
            { 0, -1, 0 },
            { 1, 0, 0 },
            { -1, 0, 0 }
        };

        int surface = 6 * blocks;
        for (auto& x_iter : sp) {
            int x = x_iter.first;

            for (auto& y_iter : x_iter.second) {
                int y = y_iter.first;

                for (int z : y_iter.second) {
                    /* inspect every direction */
                    for (const auto& dir : dirs) {
                        if (is_occupied(sp, x + dir.x, y + dir.y, z + dir.z)) {
                            surface--;
                        }
                    }
                }
            }
        }

        return surface;
    }

    int calculate_external_surface_area() {
        const std::vector<position_t> dirs = {
            { 0, 0, 1 },
            { 0, 0, -1 },
            { 0, 1, 0 },
            { 0, -1, 0 },
            { 1, 0, 0 },
            { -1, 0, 0 }
        };

        position_t max = { 0, 0, 0 }, min = { INT_MAX, INT_MAX, INT_MAX };
        for (auto& x_iter : sp) {
            int x = x_iter.first;

            max.x = std::max(x + 1, max.x);
            min.x = std::min(x - 1, min.x);

            for (auto& y_iter : x_iter.second) {
                int y = y_iter.first;

                max.y = std::max(y + 1, max.y);
                min.y = std::min(y - 1, min.y);

                for (int z : y_iter.second) {
                    max.z = std::max(z + 1, max.z);
                    min.z = std::min(z - 1, min.z);
                }
            }
        }

        int surface = 0;

        std::queue<position_t> q;
        q.push(max);

        space_t visited;
        visited[max.x][max.y].insert(max.z);

        while (!q.empty()) {
            auto cur = q.front();
            q.pop();

            for (const auto& d : dirs) {
                position_t nei = { cur.x + d.x, cur.y + d.y, cur.z + d.z };
                if (nei.x < min.x || nei.x > max.x || nei.y < min.y || nei.y > max.y || nei.z < min.z || nei.z > max.z) {
                    continue;
                }

                if (is_occupied(visited, nei.x, nei.y, nei.z)) {
                    continue;
                }

                if (is_occupied(sp, nei.x, nei.y, nei.z)) {
                    surface++;
                    continue;   /* never mask figure as visited */
                }

                visited[nei.x][nei.y].insert(nei.z);
                q.push(nei);
            }
        }

        return surface;
    }

    int surface_area() {
        int surface = calculate_surface_area();
        int external_surface = calculate_external_surface_area();

        InitWindow(1100, 700, "3D surface areas");
        SetTargetFPS(60);

        while (!WindowShouldClose()) {
            BeginDrawing();
            ClearBackground(RAYWHITE);

            DrawRectangle(0, 50, 550, 650, Color{ 245, 248, 255, 255 });
            DrawRectangle(550, 50, 550, 650, Color{ 255, 250, 240, 255 });
            DrawLine(550, 0, 550, 700, DARKGRAY);

            DrawText(TextFormat("TOTAL SURFACE: %d", surface), 20, 18, 24, DARKBLUE);
            DrawText(TextFormat("EXTERNAL SURFACE: %d", external_surface), 570, 18, 24, DARKBROWN);
            draw_figure({ 0, 50, 550, 650 }, false);
            draw_figure({ 550, 50, 550, 650 }, true);

            DrawRectangleLines(0, 50, 550, 650, DARKGRAY);
            DrawRectangleLines(550, 50, 550, 650, DARKGRAY);
            EndDrawing();
        }

        CloseWindow();
        return surface;
    }

    int extrnal_surface_area() {
        return calculate_external_surface_area();
    }

private:
    void draw_figure(Rectangle viewport, bool external_only) {
        position_t min = { INT_MAX, INT_MAX, INT_MAX };
        position_t max = { INT_MIN, INT_MIN, INT_MIN };
        for (auto& x_iter : sp) {
            min.x = std::min(min.x, x_iter.first);
            max.x = std::max(max.x, x_iter.first);
            for (auto& y_iter : x_iter.second) {
                min.y = std::min(min.y, y_iter.first);
                max.y = std::max(max.y, y_iter.first);
                for (int z : y_iter.second) {
                    min.z = std::min(min.z, z);
                    max.z = std::max(max.z, z);
                }
            }
        }

        Camera3D camera = {};
        Vector3 figure_center = { (min.x + max.x) / 2.0f, (min.y + max.y) / 2.0f, (min.z + max.z) / 2.0f };
        camera.target = { 0, 0, 0 };
        camera.position = { 1.0f * (max.x - min.x + 2), 1.0f * (max.y - min.y + 2), 1.0f * (max.z - min.z + 2) };
        camera.up = { 0, 1, 0 };
        camera.fovy = 45;
        camera.projection = CAMERA_PERSPECTIVE;

        BeginScissorMode((int)viewport.x, (int)viewport.y, (int)viewport.width, (int)viewport.height);
        BeginMode3D(camera);
        rlViewport((int)viewport.x, (int)viewport.y, (int)viewport.width, (int)viewport.height);
        const std::vector<position_t> dirs = { { 0, 0, 1 }, { 0, 0, -1 }, { 0, 1, 0 }, { 0, -1, 0 }, { 1, 0, 0 }, { -1, 0, 0 } };

        for (auto& x_iter : sp) {
            for (auto& y_iter : x_iter.second) {
                for (int z : y_iter.second) {
                    Vector3 center = { (float)x_iter.first - figure_center.x,
                        (float)y_iter.first - figure_center.y,
                        (float)z - figure_center.z };
                    DrawCube(center, 0.96f, 0.96f, 0.96f, Color{ BLUE.r, BLUE.g, BLUE.b, 153 });
                    DrawCubeWires(center, 1, 1, 1, Color{ DARKBLUE.r, DARKBLUE.g, DARKBLUE.b, 153 });
                    DrawSphere(center, 0.4f, BLUE);
                    for (const auto& dir : dirs) {
                        if (!is_occupied(sp, x_iter.first + dir.x, y_iter.first + dir.y, z + dir.z) &&
                            (!external_only || is_external_face(x_iter.first, y_iter.first, z, dir))) {
                            Vector3 face = { center.x + dir.x * 0.49f, center.y + dir.y * 0.49f, center.z + dir.z * 0.49f };
                            Vector3 size = { dir.x ? 0.03f : 0.92f, dir.y ? 0.03f : 0.92f, dir.z ? 0.03f : 0.92f };
                            Color color = external_only ? ORANGE : Color{ YELLOW.r, YELLOW.g, YELLOW.b, 153 };
                            DrawCubeV(face, size, color);
                        }
                    }
                }
            }
        }
        EndMode3D();
        rlViewport(0, 0, GetScreenWidth(), GetScreenHeight());
        EndScissorMode();
    }

    bool is_external_face(int x, int y, int z, const position_t& dir) {
        position_t cur = { x + dir.x, y + dir.y, z + dir.z };
        position_t min = { INT_MAX, INT_MAX, INT_MAX };
        position_t max = { INT_MIN, INT_MIN, INT_MIN };
        for (auto& x_iter : sp) {
            min.x = std::min(min.x, x_iter.first);
            max.x = std::max(max.x, x_iter.first);
            for (auto& y_iter : x_iter.second) {
                min.y = std::min(min.y, y_iter.first);
                max.y = std::max(max.y, y_iter.first);
                for (int block_z : y_iter.second) {
                    min.z = std::min(min.z, block_z);
                    max.z = std::max(max.z, block_z);
                }
            }
        }

        for (int i = 0; i < 10000; i++) {
            if (!is_occupied(sp, cur.x, cur.y, cur.z)) {
                bool outside = cur.x < min.x - 1 || cur.x > max.x + 1 ||
                    cur.y < min.y - 1 || cur.y > max.y + 1 ||
                    cur.z < min.z - 1 || cur.z > max.z + 1;
                if (outside) return true;
            }
            cur.x += dir.x;
            cur.y += dir.y;
            cur.z += dir.z;
        }
        return false;
    }

    bool is_occupied(const space_t& collection, int x, int y, int z) {
        auto x_iter = collection.find(x);
        if (x_iter == collection.cend()) {
            return false;
        }

        auto y_iter = x_iter->second.find(y);
        if (y_iter == x_iter->second.cend()) {
            return false;
        }

        auto z_iter = y_iter->second.find(z);
        return (z_iter != y_iter->second.cend());
    }
};


int main() {
    auto pair = read_input();

    solution figure(pair.first, pair.second);
    figure.surface_area();

    return 0;
}
