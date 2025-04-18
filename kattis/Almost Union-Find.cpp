#include <vector>
#include <unordered_set>
#include <iostream>


class disjoint_set {
private:
    std::vector<int> place;
    std::vector<std::int64_t> sum;
    std::vector<std::unordered_set<int>> sets;

public:
    disjoint_set(int len) :
        place(len, 0),
        sets(len),
        sum(len, 0)
    {
        for (int i = 0; i < len; i++) {
            place[i] = i;
            sum[i] = i;
            sets[i].insert(i);
        }
    }

public:
    void merge(int i, int j) {
        int id1 = place[i];
        int id2 = place[j];

        if (id1 == id2) {
            return;
        }

        if (sets[id1].size() < sets[id2].size()) {
            std::swap(id1, id2);
        }

        for (int node : sets[id2]) {
            sets[id1].insert(node);
            place[node] = id1;
        }

        sets[id2].clear();
        sum[id1] += sum[id2];
        sum[id2] = 0;
    }

    void move(int i, int j) {
        int id1 = place[i];
        int id2 = place[j];

        if (id1 == id2) {
            return;
        }

        sets[id1].erase(i);
        sets[id2].insert(i);
        place[i] = id2;

        sum[id1] -= i;
        sum[id2] += i;
    }

    void print(int i) {
        int id = place[i];
        std::cout << sets[id].size() << ' ' << sum[id] << std::endl;
    }
};


int main() {
    int n, cmds;

    while (std::cin >> n && std::cin >> cmds) {
        disjoint_set set(n + 1);

        for (int i = 0; i < cmds; i++) {
            int cmd = 0;
            std::cin >> cmd;

            switch (cmd) {
                case 1: {
                    int a, b;
                    std::cin >> a >> b;
                    set.merge(a, b);
                    break;
                }
                case 2: {
                    int a, b;
                    std::cin >> a >> b;
                    set.move(a, b);
                    break;
                }
                case 3: {
                    int a;
                    std::cin >> a;
                    set.print(a);
                    break;
                }
            }
        }
    }

    return 0;
}
