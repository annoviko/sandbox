#include <iostream>
#include <list>
#include <limits>
#include <memory>
#include <string>
#include <unordered_map>


struct node {
public:
	using ptr = std::shared_ptr<node>;

public:
	node::ptr m_parent = nullptr;
	char m_symbol = 0;
	std::unordered_map<char, node::ptr> m_children = { };
	bool m_shared = false;
};


class trie {
private:
	node::ptr m_root = std::make_shared<node>();

public:
	trie(const std::string & p_text) {
		insert(p_text, false);
	}

public:
	void insert(const std::string & p_text, const bool p_detect_sharing) {
		for (auto begin = p_text.begin(); begin != p_text.end(); begin++) {
			add_pattern(m_root, begin, p_text.end(), p_detect_sharing);
		}
	}

	std::string get_shortest_non_shared() {
		//return dfs(m_root, "");
		return bfs(m_root);
	}

private:
	std::string bfs(const node::ptr p_parent) {
		std::list<node::ptr> cur_level = { m_root };
		std::list<node::ptr> next_level;
		std::size_t level = 0;

		std::string shortest = "";
		std::size_t shortest_length = std::numeric_limits<std::size_t>::max();

		while (!cur_level.empty() && shortest.empty()) {
			while (!cur_level.empty()) {
				auto node = cur_level.front();
				cur_level.pop_front();

				if (!node->m_shared) {
					std::string candidate = get_substring(node, level);

					if (!candidate.empty()) {
						if (shortest_length > candidate.length()) {
							shortest = std::move(candidate);
							shortest_length = shortest.size();
						}
						else if (shortest_length == candidate.length()) {	/* lexical comparison */
							if (shortest < candidate) {
								std::move(candidate);
							}
						}
					}
				}

				for (auto & child : node->m_children) {
					next_level.push_back(child.second);
				}
			}
			
			level++;
			cur_level = std::move(next_level);
		}

		return shortest;
	}

	std::string get_substring(const node::ptr p_node, const std::size_t p_level) {
		std::string result(p_level, ' ');
		
		node::ptr cur = p_node;
		for (long i = static_cast<int>(p_level) - 1; i >= 0; i--) {
			result[i] = cur->m_symbol;
			cur = cur->m_parent;
		}

		return result;
	}

	std::string dfs(const node::ptr p_parent, const std::string & p_value) {
		if (!p_parent->m_shared) {
			return p_value;
		}

		std::string shortest = "";
		std::size_t shortest_length = std::numeric_limits<std::size_t>::max();

		for (const auto & child : p_parent->m_children) {
			std::string candidate = dfs(child.second, p_value + child.first);

			if (!candidate.empty()) {
				if (shortest_length > candidate.length()) {
					shortest = std::move(candidate);
					shortest_length = shortest.size();
				}
				else if (shortest_length == candidate.length()) {	/* lexical comparison */
					shortest = shortest < candidate ? shortest : candidate;
				}
			}
		}

		return shortest;
	}

	void add_pattern(node::ptr p_parent, std::string::const_iterator p_begin, std::string::const_iterator p_end, const bool p_detect_sharing) {
		p_parent->m_shared = p_detect_sharing;

		if (p_begin == p_end) {
			return;		/* the pattern is covered by existed nodes */
		}
		
		auto iter_next_node = p_parent->m_children.find(*p_begin);
		if (iter_next_node == p_parent->m_children.cend()) {
			/* nothing is found insert all element to the current parent */
			for (; p_begin != p_end; p_begin++) {
				node::ptr next_node = std::make_shared<node>();
				next_node->m_symbol = *p_begin;
				next_node->m_parent = p_parent;
				next_node->m_shared = p_detect_sharing;

				p_parent->m_children.insert({ *p_begin, next_node });
				p_parent = next_node;
			}
		}
		else {
			/* go further */
			add_pattern(iter_next_node->second, p_begin + 1, p_end, p_detect_sharing);
		}
	}
};


int main() {
	std::string text1, text2;
	std::getline(std::cin, text1);
	std::getline(std::cin, text2);

	trie tree(text1);
	tree.insert(text2, true);

	std::string result = tree.get_shortest_non_shared();
	std::cout << result << std::endl;

	return 0;
}
