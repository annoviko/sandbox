#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <string>


struct edge {
public:
	using ptr = std::shared_ptr<edge>;

public:
	const std::string * m_text;
	std::size_t m_begin = 0;
	std::size_t m_length = 0;

public:
	edge() = default;

	edge(const std::string * p_text, std::size_t p_begin, std::size_t p_length) :
		m_text(p_text),
		m_begin(p_begin),
		m_length(p_length)
	{ }

public:
	std::size_t match(std::size_t p_begin, std::size_t p_length) const {
		const std::string & text = *m_text;

		std::size_t result = 0;
		std::size_t length = std::min(p_length, m_length);

		for (; (result < length) && (text[result + p_begin] == text[result + m_begin]); result++) { }
		return result;
	}

	std::string get_string() const {
		return m_text->substr(m_begin, m_length);
	}

public:
	bool operator<(const edge & p_edge) const {
		const std::string & text = *m_text;
		const std::size_t length = std::min(p_edge.m_length, m_length);

		for (std::size_t i = 0; i < length; i++) {
			const char right = text[i + m_begin], left = text[i + p_edge.m_begin];
			if (right < left) {
				return true;
			}
			else if (right > left) {
				return false;
			}
		}

		/* if the same, then compare size */
		return m_length < p_edge.m_length;
	}
};


struct node {
public:
	using ptr = std::shared_ptr<node>;
	using successors = std::list<std::pair<edge, node::ptr>>;

public:
	successors	m_children;

public:
	node() = default;
};


class suffix_tree {
private:
	node::ptr m_root = std::make_shared<node>();
	std::string m_text;

public:
	suffix_tree(const std::string & p_text) : 
		m_text(p_text) 
	{
		for (std::size_t i = 0; i < m_text.size(); i++) {
			node::ptr cursor = m_root;
			std::size_t position = i;
			while (position != m_text.size()) {
				position = add_pattern(position, cursor);
			}
		}
	}

public:
	node::ptr get_root() const { return m_root; }

private:
	std::size_t add_pattern(std::size_t p_begin, node::ptr & p_cursor) {
		for (auto iter_child = p_cursor->m_children.begin(); iter_child != p_cursor->m_children.end(); iter_child++) {	/* check each edge */
			const edge & child_edge = iter_child->first;
			const std::size_t overlap = child_edge.match(p_begin, m_text.size() - p_begin);

			if (overlap > 0 && overlap == child_edge.m_length) {	/* if there is total matching then go the next node */
				p_begin += overlap;

				if (p_begin >= m_text.size()) {	/* there no need to go further - mark this node as an ending */
					return m_text.size();	/* no need to continue */
				}

				p_cursor = iter_child->second;
				return p_begin;	/* consider next nodes */
			}
			else if (overlap > 0) { /* if there is just partial match then split the edge and create new node */
				/* create new node in the middle */
				std::size_t lower_length = child_edge.m_length - overlap;
				std::size_t lower_begin = child_edge.m_begin + overlap;
				edge lower_edge(&m_text, lower_begin, lower_length);

				node::ptr middle_node = std::make_shared<node>();	/* this is not an ending */
				middle_node->m_children.push_back(std::pair<edge, node::ptr>(lower_edge, iter_child->second));

				/* insert upper edge that leads to middle node */
				edge upper_edge(&m_text, child_edge.m_begin, overlap);
				p_cursor->m_children.push_back(std::pair<edge, node::ptr>(upper_edge, middle_node));

				/* insert rest part of the pattern to complitely new node that will belong to middle node */
				std::size_t new_begin = p_begin + overlap;
				std::size_t new_length = m_text.size() - new_begin;
				edge new_edge(&m_text, new_begin, new_length);
				node::ptr new_node = std::make_shared<node>();
				middle_node->m_children.push_back(std::pair<edge, node::ptr>(new_edge, new_node));

				/* remove current child */
				p_cursor->m_children.erase(iter_child);

				/* no need to continue */
				return m_text.size();
			}
		}

		/* no matches at current node - then add new node with new edge */
		edge new_edge(&m_text, p_begin, m_text.size() - p_begin);
		node::ptr new_node = std::make_shared<node>();	/* this is a node with ending */
		p_cursor->m_children.push_back(std::pair<edge, node::ptr>(new_edge, new_node));

		return m_text.size();
	}
};


std::ostream & operator<<(std::ostream & p_stream, const suffix_tree & p_tree) {
	std::list<node::ptr> to_visit = { p_tree.get_root() };
	while (!to_visit.empty()) {
		node::ptr cur = to_visit.front();
		to_visit.pop_front();

		for (auto & child : cur->m_children) {
			p_stream << child.first.get_string() << std::endl;
			to_visit.push_back(child.second);
		}
	}

	return p_stream;
}


int main() {
	std::string text;
	std::getline(std::cin, text);

	suffix_tree tree(text);
	std::cout << tree << std::endl;

	return 0;
}
