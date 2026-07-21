#include <vector>
#include <queue>


class Solution {
public:
    std::vector<std::vector<int>> floodFill(std::vector<std::vector<int>>& image, int sr, int sc, int color) {
        std::queue<std::pair<int, int>> q;
        std::vector<std::vector<bool>> visited(image.size(), std::vector(image[0].size(), false));

        q.push({ sr, sc });
        visited[sr][sc] = true;
        visited[sr][sc] = true;

        std::vector<std::pair<int, int>> dirs = {
            { -1, 0 },
            { 1, 0 },
            { 0, -1 },
            { 0, 1 }
        };

        const int orig_color = image[sr][sc];

        while (!q.empty()) {
            auto cur = q.front();
            q.pop();

            image[cur.first][cur.second] = color;

            for (const auto & d : dirs) {
                int r = cur.first + d.first;
                int c = cur.second + d.second;

                if (r < 0 || r >= image.size() || c < 0 || c >= image[0].size()) {
                    continue;
                }

                if (visited[r][c]) {
                    continue;
                }

                if (orig_color != image[r][c]) {
                    continue;
                }

                visited[r][c] = true;
                q.push({ r, c });
            }
        }

        return image;
    }
};