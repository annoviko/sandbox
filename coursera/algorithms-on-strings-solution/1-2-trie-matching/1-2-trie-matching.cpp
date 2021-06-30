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

public:
	bool is_leaf() {
		return m_children.empty();
	}
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

	std::list<std::size_t> match(const std::string::const_iterator p_begin, const std::string::const_iterator p_end) {
		std::list<std::size_t> result;

		for (std::string::const_iterator begin = p_begin; begin != p_end; begin++) {
			if (match_text(begin, p_end)) {
				result.push_back(begin - p_begin);
			}
		}

		return result;
	}

private:
	bool match_text(const std::string::const_iterator & p_begin, const std::string::const_iterator & p_end) {
		node::ptr cursor = m_root;
		for (std::string::const_iterator begin = p_begin; begin != p_end && !cursor->is_leaf(); begin++) {
			auto iter = cursor->m_children.find(*begin);
			if (iter == cursor->m_children.end()) {
				return false;
			}

			cursor = iter->second;
		}

		return cursor->is_leaf() ? true : false;
	}

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
	std::string text;
	std::getline(std::cin, text);

	std::size_t length;
	std::cin >> length;
	std::cin.ignore(1);

	std::vector<std::string> patterns(length);
	for (std::size_t i = 0; i < length; ++i) {
		std::getline(std::cin, patterns[i]);
	}

	trie tree(patterns);

	auto matches = tree.match(text.cbegin(), text.cend());
	for (auto position : matches) {
		std::cout << position << " ";
	}
	std::cout << std::endl;

	return 0;
}