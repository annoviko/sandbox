#include <iostream>
#include <vector>
#include <algorithm>


const int INF_POS = 1e9;
const int INF_NEG = -1e9;


int main() {
    while (true) {
        int n, m, q;
        std::cin >> n >> m >> q;

        if (n == 0 && m == 0 && q == 0) {
            break;
        }

        std::vector<std::vector<int>> dp(n, std::vector<int>(n, INF_POS));
        for (int i = 0; i < n; i++) {
            dp[i][i] = 0;
        }

        for (int i = 0; i < m; i++) {
            int u, v, w;
            std::cin >> u >> v >> w;

            dp[u][v] = std::min(dp[u][v], w);;
        }

        for (int k = 0; k < n; k++) {
            for (int i = 0; i < n; i++) {
                if (dp[i][k] == INF_POS) {
                    continue;
                }

                for (int j = 0; j < n; j++) {
                    if (dp[k][j] == INF_POS) {
                        continue;
                    }

                    dp[i][j] = std::min(dp[i][j], dp[i][k] + dp[k][j]);
                }
            }
        }

        /* detect negative loops */
        for (int k = 0; k < n; k++) {
            if (dp[k][k] < 0) {
                for (int i = 0; i < n; i++) {
                    if (dp[i][k] == INF_POS) {
                        continue;
                    }
                    for (int j = 0; j < n; j++) {
                        if (dp[k][j] == INF_POS) {
                            continue;
                        }
                        dp[i][j] = INF_NEG;
                    }
                }
            }
        }

        for (int i = 0; i < q; i++) {
            int from, to;
            std::cin >> from >> to;

            switch (dp[from][to]) {
            case INF_POS:
                std::cout << "Impossible" << std::endl;
                break;
            case INF_NEG:
                std::cout << "-Infinity" << std::endl;
                break;
            default:
                std::cout << dp[from][to] << std::endl;
                break;
            }
        }

        std::cout << std::endl;
    }

    return 0;
}