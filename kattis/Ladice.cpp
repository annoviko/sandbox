#include <vector>


class disjoint_set {
private:
    std::vector<int> ids;
    std::vector<int> sizes;
    std::vector<int> items;

public:
    disjoint_set(int len) :
        ids(len, 0),
        sizes(len, 1),
        items(len, 0)
    {
        for (int i = 0; i < len; i++) {
            ids[i] = i;
        }
    }

private:
    int get_set_id(int i) {
        if (i != ids[i]) {
            ids[i] = get_set_id(ids[i]);
        }

        return ids[i];
    }

    void merge(int i, int j) {
        int id1 = get_set_id(i);
        int id2 = get_set_id(j);

        if (id1 == id2) {
            return;
        }

        if (sizes[id1] >= sizes[id2]) {
            ids[id2] = id1;
            sizes[id1] += sizes[id2];
            items[id1] += items[id2];
        }
        else {
            ids[id1] = id2;
            sizes[id2] += sizes[id1];
            items[id2] += items[id1];
        }
    }

public:
    bool add_item(int a, int b) {
        merge(a, b);

        int id = get_set_id(a);
        if (items[id] == sizes[id]) {
            return false;
        }

        items[id]++;
        return true;
    }
};


#include <iostream>


int main() {
    int n, l;
    std::cin >> n >> l;

    disjoint_set set(l);

    for (int i = 0; i < n; i++) {
        int a, b;
        std::cin >> a >> b;

        a--; b--;

        if (set.add_item(a, b)) {
            std::cout << "LADICA" << std::endl;
        }
        else {
            std::cout << "SMECE" << std::endl;
        }
    }

    return 0;
}
