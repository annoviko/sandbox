#include <fstream>
#include <iostream>
#include <list>
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>


struct node_info {
    std::string id;
    std::string orbit_id;
};


struct node {
    using ptr = std::shared_ptr<node>;

    std::string id;
    node::ptr parent;
    std::list<node::ptr> childs;

    bool visited = false;
};


std::list<node_info> read_map() {
    std::list<node_info> result;

    std::ifstream infile("input.txt");
    std::string description;
    while (infile >> description) {
        node_info info;
        std::size_t separate_position = description.find(")");

        info.orbit_id = description.substr(0, separate_position);
        info.id = description.substr(separate_position + 1, 3);

        result.push_back(info);
    }
    infile.close();

    return result;
}


class orbit_map {
private:
    std::unordered_map<std::string, node::ptr> m_map;
    node::ptr m_root;
    node::ptr m_spaceship_mine;
    node::ptr m_spaceship_santa;

public:
    void build(const std::list<node_info> & p_map) {
        for (const auto & item : p_map) {
            node::ptr child_node;
            auto child_node_iter = m_map.find(item.id);
            if (child_node_iter == m_map.end()) {
                child_node = std::make_shared<node>();
                child_node->id = item.id;

                m_map[item.id] = child_node;

                if (item.id == "YOU") {
                    m_spaceship_mine = child_node;
                }
                else if (item.id == "SAN") {
                    m_spaceship_santa = child_node;
                }
            }
            else {
                child_node = child_node_iter->second;
            }

            node::ptr parent_node;
            auto parent_node_iter = m_map.find(item.orbit_id);
            if (parent_node_iter == m_map.end()) {
                parent_node = std::make_shared<node>();
                parent_node->id = item.orbit_id;

                m_map[item.orbit_id] = parent_node;
                if (item.orbit_id == "COM") {
                    m_root = parent_node;
                }
            }
            else {
                parent_node = parent_node_iter->second;
            }

            child_node->parent = parent_node;
            parent_node->childs.push_back(child_node);
        }
    }

    std::size_t total_orbits_amount() {
        if (m_root == nullptr) {
            throw std::exception("No COM on the map.");
        }

        return total_orbits_amount(m_root, 0);
    }

    std::size_t minimum_path_to_santa() {
        if (m_spaceship_mine == nullptr) {
            throw std::exception("Error: Location our ship is not found on the map.");
        }

        if (m_spaceship_santa == nullptr) {
            throw std::exception("Error: Location Santa ship is not found on the map.");
        }

        std::list<node::ptr> current_level;
        std::list<node::ptr> next_level;

        std::size_t distance = 0;

        m_spaceship_mine->visited = true;

        if (m_spaceship_mine->parent) {
            m_spaceship_mine->parent->visited = true;

            if (m_spaceship_mine->parent->id == "SAN") {
                return 0;
            }

            next_level.push_back(m_spaceship_mine->parent);
        }

        while (!next_level.empty()) {
            current_level = std::move(next_level);

            for (auto& orbit : current_level) {
                if (orbit->parent != nullptr) {
                    if (orbit->parent->id == "SAN") {
                        return distance;
                    }
                    else if (!orbit->parent->visited) {
                        next_level.push_back(orbit->parent);
                    }
                }

                for (auto& child : orbit->childs) {
                    if (child->id == "SAN") {
                        return distance;
                    }

                    if (!child->visited) {
                        child->visited = true;
                        next_level.push_back(child);
                    }
                }
            }

            distance++;
        }

        throw std::exception("Error: Route to the Santa is not found.");
    }

private:
    std::size_t total_orbits_amount(node::ptr & node, std::size_t orbit_number) {
        if (node->childs.empty()) {
            return orbit_number;
        }

        std::size_t result = orbit_number;
        for (auto& child : node->childs) {
            result += total_orbits_amount(child, orbit_number + 1);
        }

        return result;
    }
};


int main() {
    auto map_info = read_map();

    try {
        orbit_map map;
        map.build(map_info);

        std::cout << "Total number of direct and indirect orbits: " << map.total_orbits_amount() << std::endl;
        std::cout << "The shortest to Santa spaceship: " << map.minimum_path_to_santa() << std::endl;
    }
    catch (std::exception & error) {
        std::cout << error.what() << std::endl;
        return -1;
    }

    return 0;
}
