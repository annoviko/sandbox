#include <cstdio>
#include <iostream>
#include <queue>
#include <vector>
#include <unordered_set>


struct patient {
    std::int64_t p = 0;
    std::string name;
};


struct patient_comparator {
    bool operator()(const patient& l, const patient& r) {
        if (l.p == r.p) {
            return l.name > r.name;
        }

        return l.p < r.p;
    }
};


int main(){
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::int64_t n, k;
    std::cin >> n >> k;

    std::unordered_set<std::string> patients;
    std::priority_queue<patient, std::vector<patient>, patient_comparator> queue;

    for (std::int64_t i = 0; i < n; i++) {
        std::int64_t type = 0, t;
        std::cin >> type >> t;

        switch (type) {
            case 1: {
                std::string name;
                std::int64_t s;

                std::cin >> name >> s;

                patients.insert(name);
                queue.push({ s - t * k, name });

                break;
            }

            case 2: {
                bool is_found = false;

                while (!queue.empty()) {
                    patient p = queue.top();
                    queue.pop();

                    auto iter = patients.find(p.name);
                    if (iter == patients.cend()) {
                        continue;
                    }

                    is_found = true;
                    patients.erase(iter);
                    std::cout << p.name << std::endl;
                    break;
                }

                if (!is_found) {
                    std::cout << "doctor takes a break" << std::endl;
                }
                break;
            }

            case 3: {
                std::string name;
                std::cin >> name;

                patients.erase(name);

                break;
            }
        }
    }

    return 0;
}
