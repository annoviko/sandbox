#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>


#if 0
#define RAYLIB_VISUALIZATION
#endif


#if defined(RAYLIB_VISUALIZATION)
#include "raylib.h"
#endif

struct pos_t {
    std::int64_t r;
    std::int64_t c;
};


class solution {
private:
    std::vector<pos_t> coords;

public:
    solution(const std::vector<pos_t>& c) : coords(c) { }

public:
    std::int64_t find_max_rectangle() {
        std::int64_t square = 0;
        for (int i = 0; i < coords.size(); i++) {
            for (int j = i + 1; j < coords.size(); j++) {
                std::int64_t candidate = (std::abs(coords[i].c - coords[j].c) + 1) * (std::abs(coords[i].r - coords[j].r) + 1);
                square = std::max(square, candidate);
            }
        }

        return square;
    }

    std::int64_t find_max_rectangle_limited() {
        const auto split_lines = find_split_lines();

        const std::int64_t upper_max = find_best_corner_for_upper(split_lines[1].first, split_lines[1].second).second;
        const std::int64_t bottom_max = find_best_corner_for_bottom(split_lines[0].first, split_lines[0].second).second;

        return std::max(upper_max, bottom_max);
    }

    void visualize() {
#if defined(RAYLIB_VISUALIZATION)
        const int screen_width = 900;
        const int screen_height = 900;

        InitWindow(screen_width, screen_height, "AoC 2025 - Day 9 Visualization | C++ Raylib");
        SetTargetFPS(60);

        if (coords.size() < 2) {
            CloseWindow();
            return;
        }

        std::int64_t xmin = std::numeric_limits<std::int64_t>::max();
        std::int64_t ymin = std::numeric_limits<std::int64_t>::max();
        std::int64_t xmax = std::numeric_limits<std::int64_t>::min();
        std::int64_t ymax = std::numeric_limits<std::int64_t>::min();

        for (const auto& p : coords) {
            xmin = std::min(xmin, p.c);
            ymin = std::min(ymin, p.r);
            xmax = std::max(xmax, p.c);
            ymax = std::max(ymax, p.r);
        }

        double width = xmax - xmin;
        double height = ymax - ymin;

        if (width < 1) {
            width = 1;
        }
        if (height < 1) {
            height = 1;
        }

        double xscale = (double)screen_width / width;
        double yscale = (double)screen_height / height;
        double scale = std::min(xscale, yscale) * 0.8;

        double scaled_width = width * scale;
        double scaled_height = height * scale;

        double offset_x = (screen_width - scaled_width) / 2.0;
        double offset_y = (screen_height - scaled_height) / 2.0;

        auto split_lines = find_split_lines();
        std::vector<Color> split_colors = { BLUE, GREEN };

        const auto r_below = find_best_corner_for_bottom(split_lines[0].first, split_lines[0].second);
        const auto r_upper = find_best_corner_for_upper(split_lines[1].first, split_lines[1].second);

        const pos_t best_partner_below = r_below.first;
        const pos_t best_partner_upper = r_upper.first;

        const std::int64_t area_below = r_below.second;
        const std::int64_t area_upper = r_upper.second;

        while (!WindowShouldClose())
        {
            BeginDrawing();
            ClearBackground(RAYWHITE);

            for (size_t i = 0; i < coords.size(); i++) {
                const pos_t& p1 = coords[i];
                const pos_t& p2 = coords[(i + 1) % coords.size()];

                Vector2 a = to_screen(p1.c, p1.r, xmin, ymin, scale, screen_height, offset_x, offset_y);
                Vector2 b = to_screen(p2.c, p2.r, xmin, ymin, scale, screen_height, offset_x, offset_y);

                DrawLineEx(a, b, 2.0f, BLACK);
                DrawCircleV(a, 4, BLACK);
            }

            for (int i = 0; i < split_lines.size(); i++) {
                const int p_idx1 = split_lines[i].first;
                const int p_idx2 = split_lines[i].second;

                const pos_t& p1 = coords[p_idx1];
                const pos_t& p2 = coords[p_idx2];

                Vector2 a = to_screen(p1.c, p1.r, xmin, ymin, scale, screen_height, offset_x, offset_y);
                Vector2 b = to_screen(p2.c, p2.r, xmin, ymin, scale, screen_height, offset_x, offset_y);

                DrawLineEx(a, b, 4.0f, split_colors[i]);
            }

            if (best_partner_below.r != -1) {
                auto p1 = best_partner_below;
                auto p2 = coords[split_lines[0].second];

                draw_rect(area_below, p1, p2, DARKBLUE, xmin, ymin, scale, screen_height, offset_x, offset_y);
            }

            if (best_partner_upper.r != -1) {
                auto p1 = best_partner_upper;
                auto p2 = coords[split_lines[1].second];

                draw_rect(area_upper, p1, p2, DARKGREEN, xmin, ymin, scale, screen_height, offset_x, offset_y);
            }

            EndDrawing();
        }

        CloseWindow();
#else
        std::cout << "Raylib visualization is disabled." << std::endl;
#endif
    }

private:
#if defined(RAYLIB_VISUALIZATION)
    Vector2 to_screen(std::int64_t x, std::int64_t y, std::int64_t xmin, std::int64_t ymin, double scale, int screen_hight, double offset_x, double offset_y) {
        float sx = (float)((x - xmin) * scale + offset_x);

        float sy = (float)((y - ymin) * scale);
        sy = screen_hight - sy;
        sy -= offset_y;

        return { sx, sy };
    }

    void draw_rect(std::int64_t area, const pos_t& p1, const pos_t& p2, Color color, std::int64_t xmin, std::int64_t ymin, double scale, int screen_height, double offset_x, double offset_y) {
        float left = std::min(p1.c, p2.c);
        float right = std::max(p1.c, p2.c);
        float top = std::min(p1.r, p2.r);
        float bottom = std::max(p1.r, p2.r);

        Vector2 top_left = to_screen(left, top, xmin, ymin, scale, screen_height, offset_x, offset_y);
        Vector2 bottom_right = to_screen(right, bottom, xmin, ymin, scale, screen_height, offset_x, offset_y);

        float rect_width = bottom_right.x - top_left.x;
        float rect_height = bottom_right.y - top_left.y;

        if (rect_width < 0) {
            top_left.x += rect_width;
            rect_width = -rect_width;
        }
        if (rect_height < 0) {
            top_left.y += rect_height;
            rect_height = -rect_height;
        }

        DrawRectangleLines((int)top_left.x, (int)top_left.y, (int)rect_width, (int)rect_height, color);
        DrawRectangle((int)top_left.x, (int)top_left.y, (int)rect_width, (int)rect_height, Fade(color, 0.3f));

        std::string text = std::to_string(area);
        int font_size = 40;
        int text_w = MeasureText(text.c_str(), font_size);
        int text_h = font_size;

        int text_x = top_left.x + (rect_width - text_w) / 2;
        int text_y = top_left.y + (rect_height - text_h) / 2;

        DrawText(std::to_string(area).c_str(), text_x, text_y, font_size, color);
    }
#endif

    std::pair<pos_t, std::int64_t> find_best_corner_for_bottom(int index_left, int index_right) {
        pos_t corner = coords[index_right];

        std::int64_t best_area = 0;
        pos_t best_partner = { -1, -1 };

        for (int i = index_left + 1; i < coords.size(); i++) {
            bool is_breaking_limit = false;
            const auto cur = coords[i];

            for (int j = i + 1; j < coords.size(); j++) {
                const auto p = coords[j];
                if (p.r <= corner.r && p.c <= corner.c) {
                    /*
                    o------------------------------o
                    |   cur
                    o    |       o <--- p -- `p` is breaking formating due to `row` - it has higher value then `cur`
                    |    v       |
                    o----o       |
                         |       |
                         o-------o
                    
                    |
                    row max value
                    |
                    |
                    |
                    v
                    row min value


                    */
                    if ((p.r > cur.r) && (cur.c < p.c)) {
                        is_breaking_limit = true;
                        break;
                    }
                }
            }

            if (is_breaking_limit) {
                continue;
            }

            std::int64_t candidate = (std::abs(corner.r - cur.r) + 1) * (std::abs(corner.c - cur.c) + 1);
            if (candidate > best_area) {
                best_partner = cur;
                best_area = candidate;
            }
        }

        return { best_partner, best_area };
    }

    std::pair<pos_t, std::int64_t> find_best_corner_for_upper(int index_left, int index_right) {
        pos_t corner = coords[index_right];

        std::int64_t best_area = 0;
        pos_t best_partner = { -1, -1 };

        for (int i = index_left - 1; i > 0; i--) {
            bool is_breaking_limit = false;
            const auto cur = coords[i];

            for (int j = index_left - 1; j > 0; j--) {
                const auto p = coords[j];
                if (p.r >= corner.r && p.c <= corner.c) {
                    if ((cur.r > p.r) && (cur.c < p.c)) {
                        is_breaking_limit = true;
                        break;
                    }
                }
            }

            if (is_breaking_limit) {
                continue;
            }

            std::int64_t candidate = (std::abs(corner.r - cur.r) + 1) * (std::abs(corner.c - cur.c) + 1);
            if (candidate > best_area) {
                best_partner = cur;
                best_area = candidate;
            }
        }

        return { best_partner, best_area };
    }


    std::vector<std::pair<int, int>> find_split_lines() {
        std::pair<int, int> l1, l2;
        std::int64_t d1 = 0, d2 = 0;

        for (int i = 0; i < coords.size(); i++) {
            for (int j = i + 1; j < coords.size(); j++) {
                if (coords[i].r != coords[j].r) {
                    continue;
                }

                std::int64_t d = std::abs(coords[i].c - coords[j].c);
                if (d > d1) {
                    if (d1 > d2) {
                        l2 = l1;
                        d2 = d1;
                    }

                    l1.first = i;
                    l1.second = j;
                    d1 = d;
                }
                else if (d > d2) {
                    l2.first = i;
                    l2.second = j;
                    d2 = d;
                }
            }
        }

        if (coords[l1.first].r > coords[l2.first].r) {
            std::swap(l1, l2);
        }

        if (coords[l1.first].c > coords[l1.second].c) {
            std::swap(l1.first, l1.second);
        }

        if (coords[l2.first].c > coords[l2.second].c) {
            std::swap(l2.first, l2.second);
        }

        /* bottom line is the first */
        /* upper line is the second */
        return { l1, l2 };
    }
};


std::vector<pos_t> read_input() {
    std::fstream stream("input.txt");
    std::vector<pos_t> in;

    for (std::string line; std::getline(stream, line); ) {
        std::size_t p = line.find(',');

        std::int64_t col = std::stoi(line.substr(0, p));
        std::int64_t row = std::stoi(line.substr(p + 1));

        in.push_back({ row, col });
    }

    return in;
}


int main() {
    auto input = read_input();

    auto solver = solution(input);

    std::cout << "The largest area (non limited by the figure): " << solver.find_max_rectangle() << std::endl;
    std::cout << "The largest area (limited by the figure): " << solver.find_max_rectangle_limited() << std::endl;

    solver.visualize();

    return 0;
}
