#include <vector>
#include <iostream>
#include <queue>
#include <limits>



const int INVALID_VALUE = std::numeric_limits<int>::max();


using graph = std::vector<std::vector<int>>;


class solution {
public:
    graph m_rg;
    int m_s;
    int m_t;

    std::vector<int> p;

public:
    solution(const graph& g, int s, int t) : m_rg(g), m_s(s), m_t(t) { }

public:
    int max_flow() {
        int total_capacity = 0;
        int flow = -1;
        while ((flow = bfs(m_s, m_t)) != INVALID_VALUE) {
            total_capacity += flow;

            int to = m_t;
            while (p[to] != -1) {
                int from = p[to];

                m_rg[from][to] -= flow;
                m_rg[to][from] += flow;

                to = from;
            }
        }

        return total_capacity;
    }

private:
    int bfs(int s, int t) {
        p.assign(m_rg.size(), -1);
        std::vector<int> c(m_rg.size(), INVALID_VALUE);    /* path capacity */

        std::queue<int> to_proc;
        to_proc.push(s);

        while (!to_proc.empty()) {
            int cur = to_proc.front();
            to_proc.pop();

            if (cur == t) {
                break;
            }

            for (int i = 0; i < m_rg[cur].size(); i++) {
                const int w = m_rg[cur][i];
                if (w > 0 && p[i] == -1 && i != m_s) {
                    p[i] = cur;  /* parent */
                    c[i] = std::min(w, c[cur]);
                    to_proc.push(i);
                }
            }
        }

        return c[t];
    }
};


int main() {
    for (int i = 1; ; i++) {
       int n = 0;
       std::cin >> n;

       if (n == 0) {
           break;
       }

       int s, t, c;
       std::cin >> s >> t >> c;
       s--; t--;

       graph rg(n, std::vector<int>(n, 0));

       for (int i = 0; i < c; i++) {
           int from, to, w;
           std::cin >> from >> to >> w;

           from--; to--;

           rg[from][to] += w;
           rg[to][from] += w;
       }

       int max_flow = solution(rg, s, t).max_flow();

       std::cout << "Network " << i << '\n';
       std::cout << "The bandwidth is " << max_flow << ".\n\n";
    }

    return 0;
}