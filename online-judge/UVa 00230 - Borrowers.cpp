#include <algorithm>
#include <map>
#include <string>
#include <vector>
#include <regex>
#include <stack>
#include <iostream>
#include <exception>


enum class state {
    BORROW,
    RETURN,
    FREE
};


struct book_entry {
    std::string title;
    std::string author;
    state status;

    book_entry(const std::string& p_title, const std::string& p_author, const state& p_status) :
        title(p_title),
        author(p_author),
        status(p_status)
    { }
};


class library_adviser {
private:
    std::vector<book_entry> m_books;
    std::map<std::string, int> m_title_refs;

public:
    library_adviser(const std::vector<std::string>& p_books) {
        for (const auto& book : p_books) {
            std::regex pattern("\"(.*)\" by (.*)");
            std::smatch groups;

            std::regex_search(book, groups, pattern);

            std::string book_title = groups[1].str();
            std::string book_author = groups[2].str();

            m_books.push_back(book_entry(book_title, book_author, state::FREE));
        }

        std::sort(m_books.begin(), m_books.end(), [](const book_entry& r, const book_entry& l) {
            if (r.author != l.author) {
                return r.author < l.author;
            }

            return r.title < l.title;
        });

        for (int i = 0; i < m_books.size(); i++) {
            m_title_refs[m_books[i].title] = i;
        }
    }

public:
    void advise(const std::vector<std::string>& p_instructions) {
        for (const auto& instr : p_instructions) {
            if (instr == "END") {
                break;
            }
            else if (instr == "SHELVE") {
                int prev_book_index = -1;
                for (int book_index = 0; book_index < m_books.size(); book_index++) {
                    if (m_books[book_index].status == state::FREE) {
                        prev_book_index = book_index;
                        continue;
                    }
                    else if (m_books[book_index].status == state::BORROW) {
                        continue;
                    }

                    std::cout << "Put \"" << m_books[book_index].title << "\"";

                    if (prev_book_index == -1) {
                        std::cout << " first" << std::endl;
                    }
                    else {
                        std::cout << " after \"" << m_books[prev_book_index].title << "\"" << std::endl;
                    }

                    m_books[book_index].status = state::FREE;
                    prev_book_index = book_index;
                }

                std::cout << "END" << std::endl;
            }

            std::regex pattern("(.*) \"(.*)\"");
            std::smatch groups;

            std::regex_search(instr, groups, pattern);

            std::string command = groups[1].str();
            std::string book = groups[2].str();

            if (command == "BORROW") {
                int index = m_title_refs[book];
                m_books[index].status = state::BORROW;
            }
            else if (command == "RETURN") {
                int index = m_title_refs[book];
                m_books[index].status = state::RETURN;
            }
        }
    }
};


int main() {
    std::string line;
    std::vector<std::string> books;

    for (std::getline(std::cin, line); line != "END"; std::getline(std::cin, line)) {
        books.push_back(line);
    }

    std::vector<std::string> commands;
    for (std::getline(std::cin, line); line != "END"; std::getline(std::cin, line)) {
        commands.push_back(line);
    }

    library_adviser(books).advise(commands);

    return 0;
}
