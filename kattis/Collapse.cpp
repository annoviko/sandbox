#include <iostream>
#include <map>
#include <vector>
#include <set>


struct nei_node {
    int id;
    int transfer_goods;
};


using graph = std::vector<std::map<int, int>>;  /* cur_node -> { nei_id : amount_goods } - connection from cur_node to set of pairs { id : goods } */


class solution {
public:
    int solve(const graph& g, const std::vector<int>& goods_thresholds) {
        /* the node without incoming connection - is a colapsing node */
        std::vector<int> inc_goods(g.size(), 0);

        for (int from = 0; from < g.size(); from++) {
            for (auto& to : g[from]) {
                inc_goods[to.first] += to.second;
            }
        }

        std::set<int> to_delete = { 0 };
        std::vector<bool> deleted_nodes(g.size(), false);

        int collapsed_nodes = 0;

        while (!to_delete.empty()) {
            int cur_id = *(to_delete.begin());
            to_delete.erase(to_delete.begin());

            collapsed_nodes++;
            deleted_nodes[cur_id] = true;

            for (auto& nei : g[cur_id]) {
                if (deleted_nodes[nei.first]) {
                    continue;   /* escape loop */
                }

                inc_goods[nei.first] -= nei.second;

                if (inc_goods[nei.first] < goods_thresholds[nei.first]) {
                    to_delete.insert(nei.first);
                }
            }
        }

        return g.size() - collapsed_nodes;
    }
};


int main() {
    int n = 0;
    std::cin >> n;

    graph g(n);
    std::vector<int> thr(n, 0);

    for (int i = 0; i < n; i++) {
        int goods_thr, neis;
        std::cin >> goods_thr >> neis;

        for (int k = 0; k < neis; k++) {
            int from, goods_amount;
            std::cin >> from >> goods_amount;

            g[from - 1].insert({ i, goods_amount });
        }

        thr[i] = goods_thr;
    }

    std::cout << solution().solve(g, thr) << std::endl;
    return 0;
}