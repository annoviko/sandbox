#include <fstream>
#include <iostream>
#include <memory>
#include <limits>
#include <list>
#include <string>
#include <vector>


class pair_node {
public:
    using ptr = std::shared_ptr<pair_node>;

private:
    pair_node* m_parent = nullptr;
    pair_node::ptr m_left = nullptr;
    pair_node::ptr m_right = nullptr;

    std::uint64_t m_value = -1;

public:
    pair_node() = default;

    pair_node(const std::uint64_t p_value) : m_value(p_value) { }

public:
    bool is_leaf() const {
        return (m_left == nullptr) && (m_right == nullptr);
    }

    std::uint64_t get_value() const {
        return m_value;
    }

    void set_parent(const pair_node::ptr& p_parent) {
        m_parent = p_parent.get();
    }

    void set_parent(const pair_node* p_parent) {
        m_parent = (pair_node*) p_parent;
    }

    pair_node::ptr get_left() {
        return m_left;
    }

    void set_left(const pair_node::ptr& p_left) {
        m_left = p_left;
    }

    pair_node::ptr get_right() {
        return m_right;
    }

    void set_right(const pair_node::ptr& p_right) {
        m_right = p_right;
    }

    std::uint64_t get_magnitude() {
        if (is_leaf()) {
            return m_value;
        }

        return 3 * m_left->get_magnitude() + 2 * m_right->get_magnitude();
    }


    bool check_and_split_stop() {
        if (this == nullptr) { return false; }

        if (is_leaf() && m_value >= 10) {
            split();
            return true;
        }

        bool result = m_left->check_and_split_stop();
        if (!result) {
            result = m_right->check_and_split_stop();
        }

        return result;
    }


    void check_and_split_reduce(const std::size_t p_height = 0) {
        if (this == nullptr) { return; }

        if (is_leaf() && m_value >= 10) {
            split();

            if (p_height >= 4) {
                explode(p_height, true);
            }
        }

        m_left->check_and_split_reduce(p_height + 1);
        m_right->check_and_split_reduce(p_height + 1);
    }

    std::size_t check_and_reduce(const std::size_t p_height = 0) {
        if (this == nullptr || is_leaf()) {
            return 0;
        }

        std::size_t count = 0;

        if (p_height >= 4) {
            explode(p_height, false);
            count++;
        }

        count += m_left->check_and_reduce(p_height + 1);
        count += m_right->check_and_reduce(p_height + 1);
        return count;
    }

    void add_to_parent_left_leaf(const std::uint64_t p_value, const pair_node* p_source, const std::size_t p_height, bool allow_split) {
        if (m_left.get() == p_source) {
            if (m_parent) {
                m_parent->add_to_parent_left_leaf(p_value, this, p_height - 1, allow_split);
            }

            return;
        }
        else {
            //std::cout << to_string() << ": go to left node to add value at the right child-node." << std::endl;
            m_left->add_to_right_leaf(p_value, p_height + 1, allow_split);
        }
    }

    void add_to_left_leaf(const std::uint64_t p_value, const std::size_t p_height, bool allow_split) {
        if (is_leaf()) {
            m_value += p_value;

            if (allow_split && m_value >= 10) {
                split();

                if (p_height >= 4) {
                    explode(p_height, true);
                }
            }
        }
        else {
            m_left->add_to_left_leaf(p_value, p_height + 1, allow_split);
        }
    }

    void add_to_parent_right_leaf(const std::uint64_t p_value, const pair_node* p_source, const std::size_t p_height, bool allow_split) {
        if (m_right.get() == p_source) {
            if (m_parent) {
                m_parent->add_to_parent_right_leaf(p_value, this, p_height - 1, allow_split);
            }
        }
        else {
            //std::cout << to_string() << ": go to right node to add value at the left child-node." << std::endl;
            m_right->add_to_left_leaf(p_value, p_height + 1, allow_split);
        }
    }

    void add_to_right_leaf(const std::uint64_t p_value, const std::size_t p_height, bool allow_split) {
        if (is_leaf()) {
            m_value += p_value;

            if (allow_split && m_value >= 10) {
                split();

                if (p_height >= 4) {
                    explode(p_height, true);
                }
            }
        }
        else {
            m_right->add_to_right_leaf(p_value, p_height + 1, allow_split);
        }
    }

    void explode(std::size_t p_height, bool allow_split) {
        //std::cout << to_string() << " after explode: ";

        auto left_value = m_left->get_value();
        auto right_value = m_right->get_value();

        m_left = nullptr;
        m_right = nullptr;
        m_value = 0;

        m_parent->add_to_parent_left_leaf(left_value, this, p_height - 1, allow_split);
        m_parent->add_to_parent_right_leaf(right_value, this, p_height - 1, allow_split);

        //show_whole_number(this);
    }

    void split() {
        if (!is_leaf()) {
            throw std::exception("split is allowed for leaves only");
        }

        //std::cout << to_string() << " after split: ";

        double result = (double) m_value / 2.0;
        std::uint64_t left_value = std::floor(result);
        std::uint64_t right_value = std::ceil(result);

        m_value = -1;
        m_left = std::make_shared<pair_node>(left_value);
        m_right = std::make_shared<pair_node>(right_value);

        m_left->set_parent(this);
        m_right->set_parent(this);

        //show_whole_number(this);
    }

    std::string to_string() const {
        if (is_leaf()) {
            return std::to_string(m_value);
        }

        return "[" + m_left->to_string() + "," + m_right->to_string() + "]";
    }

    static void show_whole_number(pair_node* p_node) {
        pair_node *cur = p_node;
        while(cur->m_parent != nullptr) {
            cur = cur->m_parent;
        }

        std::cout << cur->to_string() << std::endl;
    }

public:
    static pair_node::ptr create(const std::string& p_str) {
        std::size_t position = 1;
        return create(p_str, position);
    }


private:
    static pair_node::ptr create(const std::string& p_str, std::size_t& p_pos) {
        pair_node::ptr node = std::make_shared<pair_node>();

        if (p_str[p_pos] == '[') {
            p_pos++;
            node->set_left(create(p_str, p_pos));
            p_pos++; // skip comma
        }
        else if (std::isdigit(p_str[p_pos])) {
            node->set_left(std::make_shared<pair_node>(p_str[p_pos] - '0'));
            p_pos += 2; // skip digit and comma
        }
        else {
            throw std::exception("unexpected characted");
        }

        node->get_left()->set_parent(node);

        if (p_str[p_pos] == '[') {
            p_pos++;
            node->set_right(create(p_str, p_pos));
            p_pos++; // skip closed bracket
        }
        else if (std::isdigit(p_str[p_pos])) {
            node->set_right(std::make_shared<pair_node>(p_str[p_pos] - '0'));
            p_pos += 2; // skip digit and closed bracket
        }
        else {
            throw std::exception("unexpected characted");
        }

        node->get_right()->set_parent(node);

        return node;
    }
};


pair_node::ptr operator+(const pair_node::ptr& p_left, const pair_node::ptr& p_right) {
    pair_node::ptr node = std::make_shared<pair_node>();

    node->set_left(p_left);
    node->set_right(p_right);
    p_left->set_parent(node);
    p_right->set_parent(node);

    //std::cout << "initial: " << node->to_string() << std::endl;

#if 1
    while (node->check_and_reduce() > 0 || node->check_and_split_stop()) { } // O(n^2)
#else
    node->check_and_reduce(); // O(n)
    node->check_and_split_reduce(); // O(n) <- does not work
#endif

    return node;
}


std::vector<pair_node::ptr> read_numbers() { 
    std::ifstream stream("input.txt");

    std::vector<pair_node::ptr> result = { };
    for (std::string number; std::getline(stream, number); ) {
        result.push_back(pair_node::create(number));
    }

    return result;
}


std::vector<std::string> read_numbers_as_strings() {
    std::ifstream stream("input.txt");

    std::vector<std::string> result = { };
    for (std::string number; std::getline(stream, number); ) {
        result.push_back(number);
    }

    return result;
}


std::uint64_t get_maximum_sum(const std::vector<std::string>& p_numbers) {
    std::uint64_t result = 0;

    for (std::size_t i = 0; i < p_numbers.size(); i++) {
        for (std::size_t j = i + 1; j < p_numbers.size(); j++) {
            auto sum = pair_node::create(p_numbers[i]) + pair_node::create(p_numbers[j]);
            result = std::max(sum->get_magnitude(), result);


            sum = pair_node::create(p_numbers[j]) + pair_node::create(p_numbers[i]);
            result = std::max(sum->get_magnitude(), result);
        }
    }

    return result;
}


int main() {
    auto snailfish_numbers = read_numbers();
    auto result = snailfish_numbers[0];

    std::cout << result->to_string() << std::endl << std::endl;

    for (std::size_t i = 1; i < snailfish_numbers.size(); i++) {
        result = result + snailfish_numbers[i];
    }

    std::cout << "Final snailfish number: " << result->to_string() << std::endl;
    std::cout << "Magnitude: " << result->get_magnitude() << std::endl;

    auto original_snailfish_numbers = read_numbers_as_strings();
    std::cout << "The largest magnitude: " << get_maximum_sum(original_snailfish_numbers) << std::endl;

    return 0;
}