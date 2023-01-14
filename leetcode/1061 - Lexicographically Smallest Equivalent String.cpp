#include <string>
#include <set>
#include <unordered_map>


class disjoint_sets {
private:
    char ids[26];

public:
    disjoint_sets() {
        std::memset(&ids, 0x00, sizeof(ids));

        for (int i = 0; i < 26; i++) {
            ids[i] = i;
        }
    }

public:
    char get_set_id(const char id) {
        if (ids[id] != id) {
            ids[id] = get_set_id(ids[id]);
        }

        return ids[id];
    }

    bool is_same_set(const char id1, const char id2) {
        const char set_id1 = get_set_id(id1);
        const char set_id2 = get_set_id(id2);

        return set_id1 == set_id2;
    }

    char merge(const char id1, const char id2) {
        const char set_id1 = get_set_id(id1);
        const char set_id2 = get_set_id(id2);

        if (set_id1 < set_id2) {
            ids[set_id2] = set_id1;
            return set_id1;
        }
        else {
            ids[set_id1] = set_id2;
            return set_id2;
        }
    }
};


class Solution {
public:
    std::string smallestEquivalentString(const std::string& s1, const std::string& s2, const std::string& baseStr) {
        disjoint_sets sets;

        for (int i = 0; i < s1.size(); i++) {
            char set_id = sets.merge(s1[i] - 'a', s2[i] - 'a');
        }

        std::string smallest;
        for (int i = 0; i < baseStr.size(); i++) {
            char smallest_value = sets.get_set_id(baseStr[i] - 'a');
            smallest.push_back(smallest_value + 'a');
        }

        return smallest;
    }
};
