#include <iostream>
#include <map>
#include <vector>
#include <unordered_map>


class disjoint_sets {
private:
    std::vector<int> m_ids;
    std::vector<int> m_sizes;

public:
    disjoint_sets(const int n) :
        m_ids(n, 0),
        m_sizes(n, 1)
    {
        for (int i = 0; i < n; i++) {
            m_ids[i] = i;
        }
    }

    int get_set_id(const int i) {
        if (m_ids[i] != i) {
            m_ids[i] = get_set_id(m_ids[i]);
        }

        return m_ids[i];
    }

    void merge(const int i, const int j) {
        int id1 = get_set_id(i);
        int id2 = get_set_id(j);

        if (id1 == id2) {
            return;
        }

        if (m_sizes[id1] > m_sizes[id2]) {
            m_ids[id2] = id1;
            m_sizes[id1] += m_sizes[id2];
        }
        else {
            m_ids[id1] = id2;
            m_sizes[id2] += m_sizes[id1];
        }
    }

    std::map<int, int> get_sizes() {
        std::unordered_map<int, int> sets;
        for (int i = 0; i < m_ids.size(); i++) {
            int id = get_set_id(i);
            sets[id]++;
        }

        std::map<int, int> sizes;
        for (auto& pair : sets) {
            sizes[pair.second]++;
        }

        return sizes;
    }
};


class random_generator {
private:
    int r, a, b, c;

public:
    random_generator(const int _r, const int _a, const int _b, const int _c) :
        r(_r), a(_a), b(_b), c(_c)
    { }

public:
    int generate() {
        r = (r * a + b) % c;
        return r;
    }
};


void chat_connection(const int n, const int r, const int a, const int b, const int c) {
    disjoint_sets sets(n);
    random_generator engine(r, a, b, c);

    for (int i = 0; i < n; i++) {
        int x = 0, y = 0;

        while (x == y) {
            x = engine.generate() % n;
            y = engine.generate() % n;
        }

        sets.merge(x, y);
    }

    auto sizes = sets.get_sizes();

    int groups = 0;
    for (auto& pair : sizes) {
        groups += pair.second;
    }

    std::cout << groups;

    for (auto iter = sizes.rbegin(); iter != sizes.rend(); iter++) {
        std::cout << ' ' << iter->first;
        if (iter->second > 1) {
            std::cout << 'x' << iter->second;
        }
    }

    std::cout << std::endl;
}


int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);

    int n;
    while (std::cin >> n) {
        int r, a, b, c;
        std::cin >> r >> a >> b >> c;

        chat_connection(n, r, a, b, c);
    }

    return 0;
}