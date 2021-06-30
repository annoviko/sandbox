#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>


struct node {
public:
	using ptr = std::shared_ptr<node>;

public:
	std::size_t m_id;
	std::unordered_map<char, node::ptr> m_children = { };

public:
	node(const std::size_t p_id) : m_id(p_id) { }
};


class trie {
private:
	node::ptr	m_root = std::make_shared<node>(0);
	std::size_t m_size = 0;

public:
	trie(const std::vector<std::string> & p_patterns) {
		for (const auto & pattern : p_patterns) {
			add_pattern(pattern);
		}
	}

public:
	const node::ptr get_root() const {
		return m_root;
	}

private:
	void add_pattern(const std::string & p_pattern) {
		node::ptr cursor = m_root;
		for (auto symbol : p_pattern) {
			cursor = add_symbol(cursor, symbol);
		}
	}

	node::ptr add_symbol(node::ptr p_node, const char p_symbol) {
		for (const auto & child : p_node->m_children) {
			if (child.first == p_symbol) {
				return child.second;
			}
		}

		node::ptr child = std::make_shared<node>(++m_size);
		p_node->m_children[p_symbol] = child;
		return child;
	}
};


std::ostream & operator<<(std::ostream & p_stream, trie & p_trie) {
	std::list<node::ptr> to_print = { p_trie.get_root() };
	while (!to_print.empty()) {
		node::ptr node = to_print.front();
		to_print.pop_front();

		for (const auto & child : node->m_children) {
			p_stream << node->m_id << "->" << child.second->m_id << ":" << child.first << std::endl;
			to_print.push_back(child.second);
		}
	}

	return p_stream;
}


int main() {
	std::size_t length;
	std::cin >> length;
	std::cin.ignore(1);

	std::vector<std::string> patterns(length);
	for (std::size_t i = 0; i < length; ++i) {
		std::getline(std::cin, patterns[i]);
	}

	trie tree(patterns);
	std::cout << tree;

	return 0;
}