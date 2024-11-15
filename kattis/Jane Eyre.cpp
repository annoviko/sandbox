#include <iostream>
#include <queue>
#include <list>
#include <regex>
#include <string>
#include <sstream>


struct book {
    std::string name;
    std::int64_t pages;
    std::int64_t time = 0;
};


struct book_compare {
    bool operator()(const book& l, const book& r) {
        return l.name > r.name;
    }
};


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::int64_t n1, n2, gp;

    std::string line;
    std::getline(std::cin, line);

    std::stringstream ss(line);
    ss >> n1 >> n2 >> gp;

    book desire = { "Jane Eyre", gp };

    std::priority_queue<book, std::vector<book>, book_compare> pq;
    pq.push(desire);

    for (int i = 0; i < n1; i++) {
        book b;

        std::getline(std::cin, line);
        std::stringstream stream(line);

        std::getline(stream, b.name, '"');
        std::getline(stream, b.name, '"');

        stream >> b.pages;

        pq.push(b);
    }

    std::list<book> to_receive;
    for (int i = 0; i < n2; i++) {
        book b;

        std::getline(std::cin, line);
        std::stringstream stream(line);

        stream >> b.time;

        stream.ignore();
        std::getline(stream, b.name, '"');
        std::getline(stream, b.name, '"');

        stream >> b.pages;

        to_receive.push_back(b);
    }

    to_receive.sort([](const book& l, const book& r) { return l.time < r.time; });

    std::int64_t cur_time = 0;
    while (!pq.empty()) {
        cur_time += pq.top().pages;

        if (pq.top().name == "Jane Eyre") {
            std::cout << cur_time << std::endl;
            return 0;
        }

        pq.pop();

        while (!to_receive.empty() && to_receive.front().time <= cur_time) {
            pq.push(to_receive.front());
            to_receive.pop_front();
        }
    }

    return 0;
}
